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

#include "sh/SHGuide.hpp"

namespace frommle {
    namespace sh {
        using  frommle::core::index;

        SHtmnGuide::SHtmnGuide(const int nmax) :SHGuideBase("SHtmnGuide",2*SHtmnGuide::i_from_mn(nmax,nmax,nmax),nmax,0){

        }

        index SHtmnGuide::idx(const int n, const int m, const SHGuideBase::trig t) const {
            index shft=(t==trig::C)?0:size_/2;
            return SHtmnGuide::i_from_mn(n,m,nmax_)+shft;
        }

        SHGuideBase::Element SHtmnGuide::operator[](const index idx) const {
            int n,m;
            trig t=(idx<size_/2)?trig::C:trig::S;
            index shft=(t==trig::C)?0:size_/2;
            std::tie(n,m)=SHtmnGuide::mn_from_i(idx-shft,nmax_);
            return std::make_tuple(n,m,t);
        }

    }
}


