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



#include "core/GuideBase.hpp"
#include <boost/python.hpp>

using namespace frommle::guides;
using namespace frommle::core;

namespace p = boost::python;

namespace frommle{

    namespace guides{
    
    void register_GuideBase(){
    ///registration of Guides defined in frommle::core


    //GuideBase
    p::class_<GuideBase,p::bases<Frommle>>("GuideBase")
//            .add_property("name",&GuideBase::name, &GuideBase::name)
            .def("size",&GuideBase::size);
            //.add_static_property("ndim",p::make_getter(&GuideBase::ndim))
//            .def_readonly("ndim",make_getter(&GuideBase::ndim, p::return_value_policy<p::reference_existing_object>()));

    //we also want to be able to pass shared_pointers to and fromm python
    p::register_ptr_to_python< constGuideBasePtr >();
    p::register_ptr_to_python< GuideBasePtr >();

    }

    }


}
