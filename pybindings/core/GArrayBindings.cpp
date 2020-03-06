/*! \file GArrayBindings.cpp
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

 Created by Roelof Rietbroek,  2019

 */

#include <boost/python.hpp>
#include "core/GArrayDense.hpp"
#include <boost/core/noncopyable.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/return_value_policy.hpp>
#include "GuidePackBindings.hpp"
namespace p = boost::python;


namespace frommle{
    namespace core{

        ///@biref converts a Guided array to an xarray DataArray
        template<class T, int n>
        struct garr_to_xarDataArray{
            static PyObject *convert(GArrayDense<T,n> const  & garin) {
                //create a list of dimension names
                p::list dims;
                p::dict coords;
                for(int i=0;i<n;++i){
                    std::string name=garin.gp()[i]->name();
                    dims.append(name);
                    coords[name]=guides::getcoords<n>(garin.gp(),i);
                }
                //also pack the entire C++guidepack as an attribute
                p::dict attrs;
                attrs["GuidePack"]=garin.gp();

                //construct an xarray-> DataArray
                auto xar=p::import("xarray");
                auto DataArray=xar.attr("DataArray")(garin.mat(),coords,dims,garin.name(),attrs);
                return p::incref(p::object(DataArray).ptr());

            }
        };


        template<class T, int n>
        struct register_dyngar{
            using gbase=GArrayBase<T,n>;
            using gdense=GArrayDense<T,n>;
            static void reg(const std::string & tname){

//                typename dense::arr & (dense::*matf)()=&dense::mat;
                const typename gbase::gp_ptr_t & (gbase::*gpfc)()const=&gbase::gpp;

                //register base array
                p::class_<gbase,p::bases<Frommle>,boost::noncopyable>(
                        std::string("GArrayBase_").append(tname).append("_").append(std::to_string(n)).c_str(), p::no_init)
                        .add_property("gp",p::make_function(gpfc,p::return_value_policy<p::copy_const_reference>()));
//                        .def("gp",gpfc,p::return_value_policy<p::copy_const_reference>());
//                        .def< const typename gbase::g_ptr_t & (gbase*::)()const>("gp",&gbase::gpp,p::return_value_policy<p::copy_const_reference>());

                //register densearray
                p::class_<gdense,p::bases<gbase>>(std::string("GArray_").append(tname).append("_").append(std::to_string(n)).c_str())
                 .def(p::init<const guides::GuidePackDyn<n> &>())
                        .add_property("mat",&register_dyngar::getndarray)
                        .def("DataArray",&garr_to_xarDataArray<T,n>::convert);

            //also register a shared_ptr
            p::register_ptr_to_python< std::shared_ptr<GArrayDense<T,n>>>();
            
            p::def(std::string("makeGArray_").append(tname).c_str(),&register_dyngar<T,n>::makedense);
            register_dyngar<T,n-1>::reg(tname);
            }

        static  std::shared_ptr<GArrayDense<T,n>> makedense(const guides::GuidePackDyn<n> & gpin,const std::string name){
            return std::make_shared<GArrayDense<T,n>>(gpin,name);
        }


        static np::ndarray getndarray(const gdense & garr){
                return py::marrayref_to_ndarray<T,n>::get(garr.mat());
        }
//        static void setndarray(dyngar & garr,PyObject * pyob){
//                //check if it is a ndarray
//
//                auto ndar = p::extract<np::ndarray&>(pyob);
//                if (ndar.check()) {
//                    //compare dimensions (should be the same)
//                    if (ndar.get_nd() != n){
//                        THROWINPUTEXCEPTION("input ndarray  has non-matching dimension");
//                    }
//                    auto ext=dyngar.gp().extent();
//                    for(int i=0;i<n;++i){
//                        if (ndar.shape(i) != ext[i]){
//                            THROWINPUTEXCEPTION("input ndarray  has non-matching shape");
//                        }
//                    }
//
//                    //if we arrive here we can copy/move the data
//                    auto dstart=reinterpret_cast<T const *>(ndar.get_data());
//
//                }else{
//                    THROWINPUTEXCEPTION("input is not a ndarray");
//
//                }
////            return py::marrayref_to_ndarray<T,n>::get(garr.mat());
//        }
        };

        template<class T>
        struct register_dyngar<T,-1>{
            static void reg(const std::string & tname){
                //does nothing (stops recursion)
            }
        };



        void registerGArrays(){
            

            //register dynamic versions
            register_dyngar<double,6>::reg("float64");
            register_dyngar<size_t,6>::reg("uint64");
        }


    }

}
