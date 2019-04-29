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
#include "core/GArrayBase.hpp"
#include "sh/Legendre_nm.hpp"
#include "core/MacroMagic.hpp"

#ifndef FROMMLE_SHFUNCTIONS_HPP
#define FROMMLE_SHFUNCTIONS_HPP
namespace frommle{
    namespace sh{
        /*@brief
         *Class to compute spherical harmonic functions with
         */
    template<class SHG>
    class Ynm:public frommle::core::Garray<double,SHG>{
    public:
        using Garr=frommle::core::Garray<double,SHG>;
        Ynm(const int nmax):Garr(nmax),Pnm(nmax){}
        void set(const double lon,const double lat){
            if (lat != lat_){
                //we need to (re)compute the Legendre function
                auto pnmval=Pnm(std::sin(lat*D2R)));
            }

            if (lon != lon_){
                //(re)recompute the longitudinal part

            }
        }
    private:
        Legendre_nm_d Pnm{};
        double lat_=1000*D2R;
        double lon_=1000*D2R;
    };

    }
}
#endif //FROMMLE_SHFUNCTIONS_HPP
