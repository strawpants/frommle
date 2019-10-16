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
#include <boost/multi_array.hpp>
#include "pybindings/tupleconversion.hpp"
namespace p = boost::python;
namespace np = boost::python::numpy;

#ifndef FROMMLE_NUMPYCONVERTERS_HPP
#define FROMMLE_NUMPYCONVERTERS_HPP

namespace frommle{

    namespace py{

        ///@brief convert an element to a numpy dtype (specialize for each non built in type)
        
        template<class T>
        struct np_dtype{
            static np::dtype get(){
                return np::dtype::get_builtin<T>();
        }

        };

        


        template<class T>
        struct vec_to_ndarray{
            static PyObject* convert( std::vector<T> const & invec){
                p::tuple shape = p::make_tuple(invec.size());
                np::dtype dtype = np_dtype<T>::get();
//        np::ndarray py_array = np::from_data(invec.data(), dtype, p::make_tuple(invec.size()),p::make_tuple(sizeof(ftype)),p::object());
                np::ndarray py_array = np::empty(shape, dtype);
                T * dptr = reinterpret_cast<T*> (py_array.get_data());
                //copy values (not very efficient for large vectors)
                for (auto & el: invec){
                    *dptr=el;
                    dptr++;
                }
                return p::incref(py_array.ptr());
            }
        };




        template<typename T, size_t ndim>
        struct marray_to_ndarray{
            using bma=boost::multi_array<T,ndim>;
            using szt=typename bma::size_type;
            using strt=typename bma::index;
            static PyObject* convert ( bma & marr){
                return p::incref(get(marr).ptr());
            }
            static np::ndarray get(bma & marr){
                p::tuple shape = arr_to_ptuple<ndim,const szt*>::make(marr.shape());
                p::tuple strides = arr_to_ptuple<ndim,const strt*>::make(marr.strides());
                np::dtype dtype = np_dtype<T>::get();
                return np::from_data(marr.data(),dtype,shape,strides,p::object());
            }
        };


    template<class G>
    np::ndarray guide_to_ndarray(const G & gd){
        using Element=typename G::Element;
        np::dtype dtype=np_dtype<Element>::get();            
        //create an numpy array
        p::tuple shape(gd.size()); 
        np::ndarray py_array = np::empty(shape, dtype);
        std::copy(gd.begin(),gd.end(),reinterpret_cast<Element*>(py_array.get_data())); 
        return py_array;
    }

    void register_numpy_converters(){
        
        //register vector to ndarray converter at runtime
        p::to_python_converter< std::vector<double> , vec_to_ndarray <double>> ();
        
    
    }        

    }

}

#endif
