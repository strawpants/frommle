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

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <ogr_geometry.h>
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

#ifndef FROMMLE_RTREE_HPP
#define FROMMLE_RTREE_HPP

//register OGRpoint class so that it is understood by boost geometry
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(OGRPoint,double, bg::cs::geographic<bg::degree>,
    getX, getY, setX, setY)

namespace frommle{
    namespace geometry{
        using bbox=bg::model::box<OGRPoint>;
        using bbval=std::pair<bbox,long unsigned int>;
        using RtreeDefault=using RstarTree=bgi::rtree<bbval,bgi::rstar<16>>;;
        bbox getBbox(const OGREnvelope & env ){
            return bbox(OGRPoint(env.MinX,env.MinY),OGRPoint(env.MaxX,env.MaxY));
        };

    }
}

#endif //FROMMLE_RTREE_HPP
