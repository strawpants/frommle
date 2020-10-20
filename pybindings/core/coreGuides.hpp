/*! \file
 \brief 
 \copyright Roelof Rietbroek 2020
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
#include "core/GuideBase.hpp"
#include "../core/numpyConverting.hpp"
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_by_value.hpp>
#include <boost/python/return_value_policy.hpp>

namespace p=boost::python;
#ifndef PYBINDINGS_CORE_GUIDES_HPP_
#define PYBINDINGS_CORE_GUIDES_HPP_

namespace frommle{
    
      namespace guides{
        ///templates used to register derived types of indexedguide
      
       
        template<class GD>
        struct pyIndexedGuide{
            using Element=typename GD::Element;
            using bpclass=p::class_<GD,p::bases<GuideBase>>;
            static bpclass reg(std::string name){
                
                return bpclass(name.c_str())
                .def("__getitem__",&GD::operator[],p::return_value_policy<p::copy_const_reference>())
                .def("__iter__",p::iterator<const GD>())
                .def("__array__",&py::guide_to_ndarray<GD>)
                .def("append",&GD::push_back);

            }
            /////@brief factory method to create a guide derived from indexedguide from a listlike object
            //static GD createFromList(const PyObject* listlike){
                ////create a new default Guide
                //GD outguide;
                ////reserve space for the index
                //size_t sz=p::call_method<size_t>(listlike,"__len__");
                //outguide.reserve(sz);
                //for(size_t i=0;i<sz;++i){
                    //outguide.push_back(p::call_method<Element>(listlike,"__getitem__",i));
                //}
                //return outguide;


            //}

        };

        template<class GD>
        struct pyNoIndexedGuide{
            using Element=typename GD::Element;
            using bpclass=p::class_<GD,p::bases<GuideBase>>;
            static bpclass reg(std::string name){
                
                return bpclass(name.c_str())
                .def("__getitem__",&GD::operator[])
                //.def("__iter__",p::iterator<const GD>())
                .def("__array__",&py::guide_to_ndarray<GD>);

            }

            /////@brief factory method to create a guide derived from indexedguide from a listlike object
            //static GD createFromList(const PyObject* listlike){
                ////create a new default Guide
                //GD outguide;
                ////reserve space for the index
                //size_t sz=p::call_method<size_t>(listlike,"__len__");
                //outguide.reserve(sz);
                //for(size_t i=0;i<sz;++i){
                    //outguide.push_back(p::call_method<Element>(listlike,"__getitem__",i));
                //}
                //return outguide;


            //}

        };

    }

}


#endif /* PYBINDINGS_CORE_GUIDES_HPP */
