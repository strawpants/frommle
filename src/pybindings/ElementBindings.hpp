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

#include "pybindings/numpyConverting.hpp"

#ifndef PYBIND_ELEMENTS_HPP_
#define CPYBIND_ELEMENTS_HPP_

namespace frommle{
namespace guides{


//   template<class Element>
//       struct ElementConverter{
//              static np::dtype dtype(){
//                     return np::dtype::get_builtin<Element>();
//              }
//              using PyElement=Element;
//
//              PyElement convert(const Element & elin){
//                  return PyElement();
//              }
//
//
//       };
//
//
//    template<>
//    struct ElementConverter<std::string>{
//        static np::dtype dtype(){
//            return np::dtype::get_builtin<std::>();
//        }
//        using PyElement=p::str;
//
//        PyElement convert(const Element & elin){
//            return PyElement();
//        }
//
//
//    };





}

}

#endif
