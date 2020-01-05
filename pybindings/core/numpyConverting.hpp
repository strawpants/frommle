/*! \file
 \brief 
 \copyright Roelof Rietbroek 2019
 \license
 This file is part of Frommle.
 frommle is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 Frommle is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with Frommle; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/slice.hpp>
#include <boost/multi_array.hpp>
#include "core/Hyperslab.hpp"
#include "tupleconversion.hpp"
#include "core/GuideRegistry.hpp"

namespace p = boost::python;
namespace np = boost::python::numpy;
using namespace frommle;
#ifndef FROMMLE_NUMPYCONVERTERS_HPP
#define FROMMLE_NUMPYCONVERTERS_HPP

namespace frommle{

    namespace py{
        template<class T>
        void nullDeleter(T* p) {
            LOGINFO << "Deleting" <<  std::endl;
        };

        ///@brief convert an element to a numpy dtype (specialize for each non built in type)
        
        template<class T>
        struct np_dtype{
            static np::dtype get(){
                auto numpy=p::import("numpy");
                return p::extract<np::dtype>(numpy.attr("dtype")("object"));
            }
            static  constexpr bool isobject(){return true;}

        };


        template<>
        struct np_dtype<long long unsigned int>{
            static np::dtype get(){
                return np::dtype::get_builtin<long long unsigned >();
            }
            static constexpr bool isobject(){return false;}
        };

        template<>
        struct np_dtype< long unsigned int>{
            static np::dtype get(){
                return np::dtype::get_builtin<long unsigned int>();
            }
            static constexpr bool isobject(){return false;}
        };

        template<>
        struct np_dtype<long long int>{
            static np::dtype get(){
                return np::dtype::get_builtin<long long int>();
            }
            static constexpr bool isobject(){return false;}
        };

        template<>
        struct np_dtype< long int>{
            static np::dtype get(){
                return np::dtype::get_builtin<long int>();
            }
            static constexpr bool isobject(){return false;}
        };


        template<>
        struct np_dtype<double>{
            static np::dtype get(){
                return np::dtype::get_builtin<double>();
            }
            static constexpr bool isobject(){return false;}
        };


        template<class T>
        struct vec_to_ndarray{
            static PyObject* convert( std::vector<T> const & invec){
                p::tuple shape = p::make_tuple(invec.size());
                np::dtype dtype = np_dtype<T>::get();
                np::ndarray py_array=np::from_data(const_cast<T *>(invec.data()),dtype,shape,p::make_tuple(sizeof(T)),p::object());
                return p::incref(py_array.ptr());
            }
        };

        template<class T>
        struct ndarray_to_vec {
        public:
                ndarray_to_vec(){
                p::converter::registry::push_back(&convertible, &construct,p::type_id<std::vector<T>>());
            }

            static void* convertible(PyObject * py_obj){
                np::ndarray ndar = p::extract<np::ndarray>(py_obj);
                //cehck if a type holds a one dimensional array (i.e. vector)
                if ( ndar.get_nd() != 1 ){
                    return 0;
                }
                return py_obj;
            }

            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {

                //setup memory for C++ class
                typedef p::converter::rvalue_from_python_storage<std::vector<T>> storage_t;
                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
                void *memory_chunk = the_storage->storage.bytes;

                np::ndarray ndar = static_cast<np::ndarray>(p::extract<np::ndarray>(py_obj)).astype(np_dtype<T>::get());
                size_t sz= ndar.shape(0);
                new(memory_chunk) std::vector<T>(sz);
                std::vector<T> * vecptr=static_cast<std::vector<T>*>(memory_chunk);
                auto dstart=reinterpret_cast<T const *>(ndar.get_data());
                //copy data
                std::copy(dstart,dstart+sz,vecptr->begin());

//                for(size_t i=0;i<sz;++i){
//                    (*vecptr)[i]=*dstart;
//                    ++dstart;
//                }
                data->convertible = memory_chunk;
            }
        };
        template<typename T, size_t ndim>
        struct marrayref_to_ndarray{
            using bma=boost::multi_array_ref<T,ndim>;
            using szt=typename bma::size_type;
            using strt=typename bma::index;
            static PyObject* convert (const bma & marr){
                return p::incref(get(marr).ptr());
            }
            static np::ndarray get(const bma & marr){
                p::tuple shape = arr_to_ptuple<ndim,const szt*>::make(marr.shape());
                np::dtype dtype = np_dtype<T>::get();
                int itemsize=dtype.get_itemsize();
                
                //note: strides must be given in terms of bytes (itemsize)
                strt stridear[ndim];
                const strt* sbegin=marr.strides();
                std::transform(sbegin,sbegin+ndim,stridear,[&itemsize](const strt & in){
                        return in*itemsize;
                        });
                
                p::tuple strides = arr_to_ptuple<ndim,const strt*>::make(stridear);
                 
                //note: that we're removing the constness of the data because we want to allow modifications from python
                return np::from_data(const_cast<T *>(marr.data()),dtype,shape,strides,p::object());
            }
        };


        template<class T>
        np::ndarray guide_to_ndarray(const T & guide){
            using Element=typename T::Element;
            np::dtype dtype = py::np_dtype<Element>::get();
            //create an numpy array
            p::tuple shape=p::make_tuple(guide.size());
            np::ndarray py_array = np::empty(shape, dtype);
            if (py::np_dtype<Element>::isobject()) {
                std::transform(guide.begin(), guide.end(), reinterpret_cast<p::object *>(py_array.get_data()),
                               [](const Element &el) {
                                   return p::object(el);
                               });
            }else{
                //copy values not object (pointers)
                std::copy(guide.begin(),guide.end(),reinterpret_cast<Element*>(py_array.get_data()));

            }
            return py_array;
        }

        template<class T, int n>
        struct register_mar{
            register_mar(){
                p::to_python_converter<boost::multi_array_ref<T,n>, marrayref_to_ndarray<T,n> >();
                register_mar<T,n-1>();
            }
        };

        template<class T>
        struct register_mar<T,-1>{
            register_mar(){
                //do nothing (stops recursion)
            }
        };

        
    template<class T>
    struct register_vector{
        register_vector(){
            p::to_python_converter< std::vector<T> , vec_to_ndarray <T>> ();
            ndarray_to_vec<T>();
            
        }
    };
    //template<class G>
    //np::ndarray guide_to_ndarray(const G & gd){
        //using Element=typename G::Element;
        //np::dtype dtype=np_dtype<Element>::get();            
        ////create an numpy array
        //p::tuple shape(gd.size()); 
        //np::ndarray py_array = np::empty(shape, dtype);
        //std::copy(gd.begin(),gd.end(),reinterpret_cast<Element*>(py_array.get_data())); 
        //return py_array;
    //}

        ///@brief hyperslab <-> ndarray converter (dimension is dynamic)
        template<typename T>
        struct hslab_to_ndarray{
            using HS=core::HyperSlabBase<T>;
            static PyObject* convert (const HS & hslab){
                return p::incref(get(hslab).ptr());
            }
            static np::ndarray get(const HS & hslab){
                np::dtype dtype = np_dtype<T>::get();
                if (! hslab.data()){
                    THROWINPUTEXCEPTION("Cannot convert hslab to ndarray without data assignment");
                }
                //note: that we're removing the constness of the data because we want to allow modifications from python
                return np::from_data(const_cast<T *>(hslab.data()),dtype,hslab.shape(),hslab.bytestride(),p::object());

            }
        };

        template<class T>
        struct ndarray_to_hslab{
        public:
            ndarray_to_hslab(){
                p::converter::registry::push_back(&convertible, &construct,p::type_id<core::HyperSlabRef<T>>());
            }

            static void* convertible(PyObject * py_obj){
                auto ndar = p::extract<np::ndarray>(py_obj);
                //check if a type holds a ndarray
                if ( not ndar.check() ){
                    return 0;
                }
                return py_obj;
            }

            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {


                np::ndarray ndar = static_cast<np::ndarray>(p::extract<np::ndarray>(py_obj)).astype(np_dtype<T>::get());


                //setup memory for C++ class
                typedef p::converter::rvalue_from_python_storage<core::HyperSlabRef<T>> storage_t;
                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
                void *memory_chunk = the_storage->storage.bytes;

                //obtain the slices from the input array
                size_t nd=ndar.get_nd();

                auto slices=std::vector<core::slice>();
                for(int i=0;i<nd;++i){
                    slices.push_back(core::make_slice(0,ndar.shape(i),ndar.strides(i)/sizeof(T)));
                }

                //we want to create a shared_ptr to the data which is owned by python so we need to extract the
                // owner of the data and apply an aliased shared_ptr contructor so the content of the shared_ptr does
                //  not get deleted when the shared pointer is destructed
//                p::incref(ndar.ptr());
//                p::incref(ndar.get_base().ptr());

//                std::shared_ptr<int> owner=std::make_shared<int>(1);

//                std::shared_ptr<T[]> sharedData(reinterpret_cast<T*>(ndar.get_data()),&nullDeleter<T>);
//                new(memory_chunk) core::HyperSlab<T>(slices,sharedData);

                new(memory_chunk) core::HyperSlabRef<T>(slices,reinterpret_cast<T*>(ndar.get_data()));

                //                core::HyperSlabRef<T> * hslabptr=static_cast<core::HyperSlabRef<T>*>(memory_chunk);
//                hslabptr->setData(reinterpret_cast<T *>(ndar.get_data()));
//
//                std::shared_ptr<PyObject> owner(p::incref(ndar.get_base().ptr()));
//                //use aliased shared_ptr constructor
//                std::shared_ptr<T[]> sharedData(owner,reinterpret_cast<T*>(ndar.get_data()));
//                core::HyperSlab<T> hslab(slices,sharedData);

                data->convertible = memory_chunk;
            }
        };


        template<class T>
        p::slice slice_from_hslab(const core::HyperSlabBase<T>  & hslab,size_t idx=0){
            return p::slice(hslab.offset()[idx],hslab.shape()[idx],hslab.stride()[idx]);
        }

        core::slice convPyslice(p::slice pslice);



        template<class T>
        core::HyperSlab<T> hslab_from_slice(const p::slice & slc){

            return core::HyperSlab<T>(convPyslice(slc));
        }

        template<class T>
        core::HyperSlab<T> hslab_from_slices(const p::tuple & slc){
            int ndim=p::len(slc);
            std::vector<core::slice> slices(ndim);
            for(int i=0;i<ndim;++i){
                slices[i]=convPyslice(p::extract<p::slice>(slc[i]));
            }
            return core::HyperSlab<T>(slices);
        }


        template<class T>
        p::list slices_from_hslab(const core::HyperSlabBase<T>  & hslab){
            p::list slices;
            for(int i=0;i<hslab.ndim();++i){
                slices.append(slice_from_hslab(hslab,i));
            }
            return slices;
        }

        template<class T>
        void register_hslab(){
            p::to_python_converter<core::HyperSlabBase<T>, hslab_to_ndarray <T>> ();
            ndarray_to_hslab<T>();
        }

//    void register_numpy_converters();

    }

}

#endif
