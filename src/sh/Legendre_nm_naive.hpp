/*! \file
 \brief Computation of (associated) Legendre functions
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

 Created by Roelof Rietbroek, 2018

 */

#include "src/core/DimensionBase.hpp"
#include <math.h>
#include <vector>

#ifndef FROMMLE_SHDIMENSION_HPP
#define FROMMLE_SHDIMENSION_HPP

namespace frommle{
    namespace sh{
        class Legendre_nm_naive{
        public:
            Legendre_nm_naive(const int nmax);
            ~Legendre_nm_naive();

            std::vector<double> operator()(const double costheta)const;
            std::vector<double> d1at(const double costheta)const;
        private:
            int nmax_=0;
            std::vector<double> wnn_={};
            std::vector<std::vector<double>> wnm_ = {};
            static int inm(const int n, const int m);
        };


    }
}


#endif //FROMMLE_SHDIMENSION_HPP
