/*! \file
 \brief Computation of (associated) Legendre functions (fast version)
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

#include "core/GuideBase.hpp"
#include <math.h>
#include <vector>
#include <assert.h>
#include "sh/SHGuide.hpp"
//#include <boost/multiprecision/float128.hpp>

#ifndef FROMMLE_SHLEGE_HPP
#define FROMMLE_SHLEGE_HPP

namespace frommle{
    namespace sh{
        template<class ftype>
        class Legendre_nm{
        public:
            Legendre_nm(const int nmax);

            std::vector<ftype> operator()(const ftype costheta)const;
            std::vector<ftype> d1at(const ftype costheta)const;
            inline size_t indxnm(const int n, const int m)const{
                return SHtmnGuide::i_from_mn(n,m,nmax_);
            }
        private:
            int nmax_=0;
            std::vector<ftype> wnn_={};
            std::vector<ftype> wnm_ = {};
        };

//explicitly initialize types for double and (long double) precision
        using Legendre_nm_d=Legendre_nm<double>;
        using Legendre_nm_ld=Legendre_nm<long double>;
//        using Legendre_nm_dd=Legendre_nm<boost::multiprecision::float128>;

    }
}


#endif //FROMMLE_SHLEGE_HPP
