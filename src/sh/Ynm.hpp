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
#include "sh/Legendre_nm.hpp"
#include "core/GOperatorBase.hpp"

#ifndef SRC_SH_YNM_HPP_
#define SRC_SH_YNM_HPP_
namespace frommle{
    using namespace guides;
    using namespace core;
    using namespace geometry;
    namespace sh{

template<class T,class SHG, class GEO>
class Ynm{};


//template specializations   
using OGRPGuide=OGRGuide<OGRPoint>;
template<class T,class SHG>
class Ynm<T,SHG,OGRPGuide>:public GOperator<T, GuidePack<SHG>, GuidePack<OGRPGuide> > {
    public:

        using GOpBase=GOperator<T,GuidePack<SHG>,GuidePack<OGRPGuide>>;
        using GOpBase::ndim_o;
        using GOpBase::ndim_i;
        using GOpBase::gpout;
        using GOpBase::gpin;

        Ynm(const int nmax):GOpBase(SHG(nmax)){

        }

        //implement forward operator (non extended version)
        void operator()(const GArrayDyn<T,ndim_i> & gin, GArrayDyn<T,ndim_o> & gout){
                auto shg=gpout_.as<SHG>(0).get();
                auto geog=gin.gp().as<OGRPGuide>(0).get();
                //loop over input points
                for(auto & pnt:*geog ){
                        LOGINFO << "x " << pnt.getX()<< " y " << pnt.getY() <<std::endl;

                }




        }

//    using GOpBase::marin;
//    using GOpBase::maroutv;
//    void fwdOp(const marin & in,maroutv & out){
//        //apply Spherical harmonics to values
//    }

    private:


};



}
}

#endif  /*SRC_SH_YNM_HPP_*/

