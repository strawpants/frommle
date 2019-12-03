/*! \file coreBindings.cpp
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

#include "coreBindings.hpp"
using namespace frommle::py;
using namespace frommle::core;
using namespace frommle::guides;

void pyexport_core(){

    //Register c++ to python conversions

    register_tuple_converters();
    
    register_numpy_converters();

    //register to/from date converters
    register_datetime();

    registerFrommle();

//    registerHyperSlab();

    registerTreeNodes();

    register_GuideBase();

    register_coreGuides();

    
    
    //register the GuidePack
    registerGuidePack();


    //register GArrays
    registerGArrays();

    
    registerGOperators();

    register_logger();

}

