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
        using GOpBase::gpout_t;
        using GOpBase::gpin_t;
        using GOpBase::gpo_;
        using GOpBase::operator();

        Ynm(const int nmax):GOpBase(GuidePack<SHG>(SHG(nmax))){

        }


        void fwdOp(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> &gout){
                auto &shg=*(gout.gp().template  as<SHG>(0));
                auto &geog=*(gin.gp().template as<OGRPGuide>(0));

                //initialize associated Legendre functionss
                Ynm_cache<T> Ynm_c(shg.nmax());

                T ynm;

                gout=0;

                size_t pidx=0;

                //denotes all elements in a dimension
                auto allinrange=gout.mrange();
                auto indices_o=gout.mindices();
                auto indices_i=gin.mindices();

                //typename GArrayDyn<T,ndim_i+1>::arr::index_gen indices_i;

                //loop over input points
                for(auto & pnt:geog ){
                        Ynm_c.setlat(pnt->getY()).setlon(pnt->getX());

                        auto irow=gin.mat()[indices_i[pidx++][allinrange]];
                        size_t idx=0;
                        for(auto & nmt:shg){
                                auto orow=gout.mat()[indices_o[idx++][allinrange]];
                                auto orow_it=orow.begin();
                                ynm=Ynm_c[nmt];
                                std::for_each(irow.begin(),irow.end(),[&orow_it, &ynm](const T & in){
                                    *orow_it+=in*ynm;
                                    ++orow_it;
                                });
                        }
                }

        }






    private:


};



}
}

#endif  /*SRC_SH_YNM_HPP_*/

