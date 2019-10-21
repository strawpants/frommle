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


        void operator()(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> &gout){
                auto &shg=*(gout.gp().template  as<SHG>(0));
                auto &geog=*(gin.gp().template as<OGRPGuide>(0));

                //initialize associated Legendre functionss
                Legendre_nm<T> Pnm(shg.nmax());
                int n,m;
                SHGuideBase::trig t;
//                int idx=0;
//                for(auto & el:shg){
//                        std::tie(n,m,t)=el;
//                        LOGINFO<<"internal"<<idx++<<" "<<n<<" "<<m<<" "<<t<<std::endl;
//                }

                T ynm;
                T cosmlon;
                T sinmlon;
                int mprevc=INT_MIN;
                int mprevs=INT_MIN;
                using nmelem=typename Legendre_nm<T>::shg_t::Element;

                gout=0;

                size_t pidx=0;

                //denotes all elements in a dimension
                boost::multi_array_types::index_range allinrange;
                typename GArrayDyn<T,ndim_o+1>::arr::index_gen indices_o;
                typename GArrayDyn<T,ndim_i+1>::arr::index_gen indices_i;

                //loop over input points
                for(auto & pnt:geog ){
                        Pnm.set(std::sin(pnt->getY()*D2R));

                        auto irow=gin.mat()[indices_i[pidx++][allinrange]];
                        size_t idx=0;
                        for(auto & nmt:shg){
                                std::tie(n,m,t)=nmt;

                                auto orow_it=gout.mat()[indices_o[idx++][allinrange]].begin();
                                if (t == SHGuideBase::trig::C){
                                        if( mprevc != m){
                                            cosmlon=std::cos(m*D2R*pnt->getX());
                                            mprevc=m;
                                        }
                                        ynm = Pnm[nmelem(n, m)] * cosmlon;
                                }else{
                                        //quick return when adding sin(0* lon)=0 is expeccted
                                        if (m == 0) continue;

                                        if( mprevs != m){
                                                sinmlon=std::sin(m*D2R*pnt->getX());
                                                mprevs=m;
                                        }

                                        ynm = Pnm[nmelem(n, m)] * sinmlon;

                                }
//                                if(n ==5 & m==2){
//                                    LOGINFO <<"internal2 "<<idx << " " << n << " " << m <<" " << " " << t << " "<< ynm << std::endl;
//                                }

//                                auto orow=gout.mat()[indices_o[idx++][allinrange]];
//                                auto irow=gin.mat()[indices_i[pidx][allinrange]];

                                std::for_each(irow.begin(),irow.end(),[&orow_it,&ynm](const T & in){
                                    (*orow_it++)+=in*ynm;
                                });
//                                std::transform(irow.begin(),irow.end(),orow.begin(),orow.begin(),[&ynm](
//                                        const T & in,const T &addTo){
//                                                return addTo+ynm*in;
//                                });
//                                gout.mat()[idx++]+=ynm*gin.mat()[pidx];
                        }

//                        LOGINFO << "x " << pnt->getX()<< " y " << pnt->getY() <<std::endl;

                }





        }

//        //implement forward operator (non extended version)
//        void operator()(const GArrayDyn<T,ndim_i> & gin, GArrayDyn<T,ndim_o> & gout){
//                auto &shg=*(gout.gp().template as<SHG>(0));
//                auto &geog=*(gin.gp().template as<OGRPGuide>(0));
//
//                //initialize associated Legendre functionss
//                Legendre_nm<T> Pnm(shg.nmax());
//                int n,m;
//                SHGuideBase::trig t;
//                T ynm;
//                using nmelem=typename Legendre_nm<T>::shg_t::Element;
//
//                gout=0;
//
//                size_t pidx=0;
//                //loop over input points
//                for(auto & pnt:geog ){
//                        Pnm.set(std::sin(pnt->getX()*D2R));
//                        size_t idx=0;
//                        for(auto & nmt:shg){
//                                std::tie(n,m,t)=nmt;
//                                if (t == SHGuideBase::trig::C){
//                                        ynm = Pnm[nmelem(n, m)] * std::cos(m * D2R * pnt->getY());
//                                }else{
//                                        ynm = (m==0)?0.0:Pnm[nmelem(n, m)] * std::sin(m * D2R * pnt->getY());
//                                }
//                                gout.mat()[idx++]+=ynm*gin.mat()[pidx];
//                        }
//                        ++pidx;
//
////                        LOGINFO << "x " << pnt->getX()<< " y " << pnt->getY() <<std::endl;
//
//                }
//
//
//
//
//        }

    private:


};



}
}

#endif  /*SRC_SH_YNM_HPP_*/

