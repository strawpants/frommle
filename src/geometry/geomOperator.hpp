/*! \file
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
 */

#include "geometry/OGR2boost.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/strategies/agnostic/point_in_poly_winding.hpp>
#include "core/GOperatorBase.hpp"
namespace bgi=boost::geometry::index;

#ifndef FROMMLE_GEOMOPERATOR_HPP
#define FROMMLE_GEOMOPERATOR_HPP
namespace frommle{
    namespace geometry{

    template<class OGR,class G>
    class withinOperator:public core::GOperator<core::MaskedGuide<G>,G>{
    public:
        using ogrcont=OGRGuide<OGR>;
        withinOperator(std::shared_ptr<ogrcont> & in):cont_(in){}
        withinOperator(){};
        template<class T,class Grhs>
        using gar=core::Garray<T,typename core::GuidePack<G,Grhs>>;

        template<class T,class Grhs>
        using garmask=core::Garray<T,typename core::GuidePack<G,Grhs>::template maskpack<0>::type>;

        template<class T,class Grhs>
        garmask<T,Grhs> operator()(gar<T,Grhs> & in)const{
            //get a reference to an array which has a masked dimension
            auto masked=in.template maskdim<0>();
            auto gm=masked.template g<0>();
            //first mask all entries in the output
            gm->mask();

            auto g1=in.template g<0>();
            //do a first scan using the boundng boxes from the boost:rtee
            auto g0=g1->cbegin();
            size_t idx=0;
            for(const auto & geom:*g1){
                auto itbegin=cont_->getRtree().qbegin(bgi::within(*geom));
                //auto itend=cont_->getRtree().qend();
//                for(auto it=itbegin;it!=itend;++it){
//                    //do a full geometry check on the hits from the rtree
//                    if(geom->Within(&(*cont_)[it->second])){
//                            //unmask the entry which has a hit
//                           gm->unmask(idx);
//                    }
//                }
                ++idx;
            }


            return masked;
        }

    private:
        std::shared_ptr<ogrcont> cont_{};
    };

    }


}

#endif //FROMMLE_GEOMOPERATOR_HPP
