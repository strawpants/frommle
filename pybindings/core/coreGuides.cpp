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

#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include <boost/python.hpp>
#include <boost/python/copy_const_reference.hpp>

namespace p = boost::python;

namespace frommle{

    namespace guides{
        void register_coreGuides(){

    //IndexGuide
    p::class_<IndexGuide,p::bases<GuideBase>>("IndexGuide").def(p::init<size_t>());
    
    //TimeGuides
    //TO pinpoint boost python to the correct overload we need to define specific function pointers
    void (DateGuide::*pbdg)(const gregdate )=&DateGuide::push_back;
    const gregdate & (DateGuide::*igetdt)(const size_t)const=&DateGuide::operator[];

    
    p::class_<DateGuide,p::bases<GuideBase>>("DateGuide")
    .def("append",pbdg)
    .def("__getitem__",igetdt,p::return_value_policy<p::copy_const_reference>())
    .def("__iter__",p::iterator<const DateGuide>());

    void (PTimeGuide::*pbpt)(const ptime )=&PTimeGuide::push_back;
    const ptime & (PTimeGuide::*igetpt)(const size_t)const=&PTimeGuide::operator[];
    p::class_<PTimeGuide,p::bases<GuideBase>>("PTimeGuide")
    .def("append",pbpt)
    .def("__getitem__",igetpt,p::return_value_policy<p::copy_const_reference>())
    .def("__iter__",p::iterator<const PTimeGuide>());

    p::def("monthlyRange",&monthlyRange);
        }

    }

}

