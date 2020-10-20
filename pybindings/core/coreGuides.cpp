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
#include "core/StringGuide.hpp"
#include "../core/coreGuides.hpp"
#include <boost/python.hpp>
#include <boost/python/copy_const_reference.hpp>

namespace p = boost::python;

   
//using namespace frommle::guides;

//using namespace frommle::core;
//using namespace frommle;

namespace frommle{
    namespace guides{
        void register_coreGuides(){
        
    //IndexGuide
    p::class_<IndexGuide,p::bases<GuideBase>>("IndexGuide").def(p::init<size_t>())
    .def("append",&IndexGuide::append);
    
//TimeGuides
    pyIndexedGuide<DateGuide>::reg("DateGuide")
        .def(p::init<std::string>());
    
    pyIndexedGuide<PTimeGuide>::reg("TimeGuide")
        .def(p::init<std::string>());
  

    pyIndexedGuide<StringGuide>::reg("StringGuide")
        .def(p::init<std::string>())
        .def("regexmask",&StringGuide::regexmask);

    p::def("monthlyRange",&monthlyRange);
        }

    }
}

