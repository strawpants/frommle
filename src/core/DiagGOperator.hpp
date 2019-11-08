/*! \file DiagGOperator.hpp
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


#include "core/GOperatorbase.hpp"

#ifndef FROMMLE_DIAGGOPERATOR_HPP
#define FROMMLE_DIAGGOPERATOR_HPP

namespace frommle{
    namespace core{

        ///@brief Holds a diagonal matrix to be used as linear operator
        template<class T>
        class DiagGOperator:public GOperatorDyn<T,1,1>{
        public:
            using GOp=GOperatorDyn<T,1,1>;
            DiagGOperator(const GArrayDyn<T,1> diagmat):GOp(diagmat.gpp()){

            }


        };


    }

}
        ]

#endif //FROMMLE_DIAGGOPERATOR_HPP
