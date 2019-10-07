/*! \file
 \brief surface spherical harmonics 
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

#include "geometry/OGRGuide.hpp"
#include "sh/SHGuide.hpp"
#include "core/GOperatorBase.hpp"
#include "sh/Legendre_nm.hpp"

#ifndef SRC_SH_YNM_HPP_
#define SRC_SH_YNM_HPP_
namespace frommle{

    namespace sh{

template<class SH, class GEO>
class Ynm{};


//template specializations   
using OGRPGuide=geometry::OGRGuide<OGRPoint>;
template<>
class Ynm<SHtmnGuide,OGRPGuide>:public core::GOperator<double,core::GuidePack<SHtmnGuide>,core::GuidePack<OGRPGuide>> {
    public:
        using GOpBase=core::GOperator<double,core::GuidePack<SHtmnGuide>,core::GuidePack<OGRPGuide>>;
        Ynm(const int nmax,std::shared_ptr<OGRPGuide> & OGRptr):GOpBase(GOpBase::outgp(nmax),GOBase::ingp(OGRptr)){
            
        }
    using GOpBase::marin;
    using GOpBase::maroutv;
    void fwdOp(const marin & in,maroutv & out){
        //apply Spherical harmonics to values 
    }

    private:


};



}
}

#endif  /*SRC_SH_YNM_HPP_*/

