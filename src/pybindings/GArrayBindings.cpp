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
#include "core/GArrayBase.hpp"
//#include <boost/core/noncopyable.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/return_value_policy.hpp>

namespace p = boost::python;


namespace frommle{
    namespace core{

        template<class T, int n>
        struct register_dyngar{
            static void reg(const std::string & basename){

                using dyngar=GArrayDyn<T,n>;
                typename dyngar::arr & (dyngar::*matf)()=&dyngar::mat;

                const typename dyngar::gp_tptr & (dyngar::*gpfc)()const=&dyngar::gpp;

                p::class_<dyngar,p::bases<GArrayBase>>(std::string(basename).append("_").append(std::to_string(n)).c_str())
            .def(p::init<const guides::GuidePackDyn<n> &>())
            .def("gp",gpfc,p::return_value_policy<p::copy_const_reference>())
            .def("mat",matf,p::return_value_policy<p::copy_non_const_reference>());
            
            //also register a shared_ptr
            p::register_ptr_to_python< std::shared_ptr<GArrayDyn<T,n>>>();
            
            p::def(std::string("make").append(basename).c_str(),&register_dyngar<T,n>::makegarr);
            register_dyngar<T,n-1>::reg(basename);
            }

        static  std::shared_ptr<GArrayDyn<T,n>> makegarr(const guides::GuidePackDyn<n> & gpin){
            return std::make_shared<GArrayDyn<T,n>>(gpin);
        }

        };

        template<class T>
        struct register_dyngar<T,-1>{
            static void reg(const std::string & basename){
                //does nothing (stops recursion)
            }
        };



        void registerGArrays(){

            p::class_<GArrayBase,boost::noncopyable>("GArrayBase").def("name",&GArrayBase::name);

            //register dynamic versions
            register_dyngar<double,5>::reg("GArray_float64");
            register_dyngar<size_t,5>::reg("GArray_uint64");

        }


    }

}
