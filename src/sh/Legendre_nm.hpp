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

#include <cmath>
#include <vector>
#include <assert.h>
#include "sh/SHGuide.hpp"
#include "core/GArrayBase.hpp"
//#include <boost/multiprecision/float128.hpp>
//#include "core/GuideAppender_impl.hpp"
#ifndef FROMMLE_SHLEGE_HPP
#define FROMMLE_SHLEGE_HPP
using namespace frommle::guides;
namespace frommle{
    namespace sh{
        template<class ftype>
    class Legendre_nm:public frommle::core::GArray<ftype,guides::GuidePack<guides::SHnmHalfGuide>>{
        public:
            using shg_t=guides::SHnmHalfGuide;
            using Garr=frommle::core::GArray<ftype,guides::GuidePack<shg_t>>;
            using Garr::mat;
            inline const shg_t & shg()const{return *(Garr::template g<0>());}
            //for future note: finding out that one need to insert'template' has costed multiple hours of my life
//            inline std::shared_ptr<guides::SHnmHalfGuide> shg()const{return gp()->template as<guides::SHnmHalfGuide>(0);}
            Legendre_nm(const int nmax);
            Legendre_nm()=default;
            Legendre_nm & set(const ftype costheta);

//            std::vector<ftype> operator()(const ftype costheta)const;
//            std::vector<ftype> d1at(const ftype costheta)const;
//            inline size_t indxnm(const int n, const int m)const{
//                return SHtmnGuide::i_from_mn(n,m,nmax_);
//            }
        private:
//            int nmax_=0;
            std::vector<ftype> wnn_={};
            std::vector<ftype> wnm_ = {};
            ftype costheta_=2; //impossible value to initiate values upon first call to set
        };

//explicitly initialize types for double and (long double) precision
        using Legendre_nm_d=Legendre_nm<double>;
        using Legendre_nm_ld=Legendre_nm<long double>;
//        using Legendre_nm_dd=Legendre_nm<boost::multiprecision::float128>;

    }



}


#endif //FROMMLE_SHLEGE_HPP
