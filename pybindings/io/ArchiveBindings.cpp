/*! \file ArchiveBindings.cpp
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
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/return_value_policy.hpp>
#include "io/ArchiveBase.hpp"
namespace p = boost::python;


namespace frommle{
    namespace io {
//        ///@brief Groupwrapper which is needed to allow dynamic python overloading
//        class VariableWrapper:public VariableBaseGroup,public p::wrapper<GroupWrapper>{
//        public:
//            void write(const Frommle & frobj){
//                frobj.save(static_cast<Group&>(*this));
//            }
//
//            void read(Frommle & frobj){
//                frobj.load(static_cast<Group&>(*this));
//            }
//
//
//        private:
//        };


        void registerArchives(){



            p::class_<Group,p::bases<core::Frommle>>("Group")
            .def("readable",&Group::readable)
            .def("writeable",&Group::writable)
            .def("getGroup",&Group::getGroup,p::return_value_policy<p::reference_existing_object>());

            p::class_<ArchiveBase,p::bases<Group>>("Archive")
            .def(p::init<std::string>());

//            .def("__getitem__",);
//            .def("__getitem__",);
        }
    }
}
