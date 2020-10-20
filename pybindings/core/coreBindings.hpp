/*! \file
 \brief 
 \copyright Roelof Rietbroek 2018
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

#ifndef FROMMLE_COREBINDINGS_HPP
#define FROMMLE_COREBINDINGS_HPP
namespace frommle{
    namespace guides {

        void register_GuideBase();
        void register_coreGuides();
        //implementation cna be found in GuidePackBindings.cpp
        void registerGuidePack();
    }

    namespace core{
        void registerExceptions();
        void registerFrommle();
//        void registerHyperSlab();
        void registerTreeNodes();
        void registerGArrays();
        void registerGOperators();
        void register_logger();

    }

    namespace py{
        void register_numpy_converters();
        void register_tuple_converters();
        void register_datetime();

    }
}

#endif 
