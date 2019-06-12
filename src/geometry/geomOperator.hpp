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
namespace bgi=boost::geometry::index;

#ifndef FROMMLE_GEOMOPERATOR_HPP
#define FROMMLE_GEOMOPERATOR_HPP
namespace frommle{
    namespace geometry{

    template<class G>
    class withinOperator{
    public:
        withinOperator(const std::shared_ptr<G> & in):baseref(in){}
        withinOperator(){};
        template<class Gout>
        std::shared_ptr<Gout> operator()(const std::shared_ptr<Gout> & in)const{
        auto masked=std::make_shared<Gout>("masked");
        //do a first scan using the rtee from the baseref
//        for(const auto & geom:*in){
//            auto itbegin=baseref->getRtree().qbegin(bgi::within(*geom));
//            for(auto it=itbegin;it!=baseref->getRtree();++it){
//                //do a full geometry check on the hits from the rtree
//                if(geom->Within(&(*baseref)[it->second])){
//                    masked->push_back(*geom);
//                }
//            }
//        }
        return masked;
        };

    private:
        std::shared_ptr<G>baseref{};
    };

    }


}

#endif //FROMMLE_GEOMOPERATOR_HPP
