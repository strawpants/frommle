/*! \file
 \brief adapted from https://www.boost.org/doc/libs/1_70_0/libs/geometry/doc/html/geometry/examples/example_source_code__adapting_a_legacy_geometry_object_model.html
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
#include <boost/geometry/geometries/register/box.hpp>

//#include <boost/geometry/index/rtree.hpp>
#include <boost/range.hpp>
#include <ogr_geometry.h>
#include "geometry/OGRiteratorBase.hpp"

namespace bg = boost::geometry;
//namespace bgi = boost::geometry::index;

#ifndef FROMMLE_OGR2BOOST_HPP
#define FROMMLE_OGR2BOOST_HPP

//Register OGR types to be understood by boos t geometry
namespace boost
{
    namespace geometry
    {
        namespace traits {
//            template<>
//            struct tag<OGRPoint> {
//                typedef point_tag type;
//            };
//
//            template<>
//            struct coordinate_type<OGRPoint> {
//                typedef double type;
//            };
//
//            template<>
//            struct coordinate_system<OGRPoint> {
//                typedef bg::cs::geographic<bg::degree> type;
//            };
//
//            template<>
//            struct dimension<OGRPoint> : boost::mpl::int_<2> {
//            };
//
//            template<>
//            struct access<OGRPoint, 0> {
//                static double get(OGRPoint const &p) {
//                    return p.getX();
//                }
//
//                static void set(OGRPoint &p, double const &value) {
//                    p.setX(value);
//                }
//            };
//
//            template<>
//            struct access<OGRPoint, 2> {
//                static double get(OGRPoint const &p) {
//                    return p.getY();
//                }
//
//                static void set(OGRPoint &p, double const &value) {
//                    p.setY(value);
//                }
//            };


//register line segment
            template<>
            struct tag<OGRLineString> {
                typedef linestring_tag type;
            };


            template<>
            struct tag<OGRLinearRing>
            {
                typedef ring_tag type;
            };
        }
    }
} // namespace boost::geometry::traits

////register OGRpoint class so that it is understood by boost geometry
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(OGRPoint,double, bg::cs::geographic<bg::degree>, getX, getY, setX, setY);
BOOST_GEOMETRY_REGISTER_BOX_2D_4VALUES(OGREnvelope, OGRPoint, MinX, MinY, MaxX, MaxY);

//register the boost range iterator to loop over the points in a linestring
using OGRLineIter=frommle::geometry::OGRiterator<OGRLineString>;
using const_OGRLineIter=frommle::geometry::const_OGRiterator<OGRLineString>;

namespace boost
{
    template <>
    struct range_iterator<OGRLineString>
    { typedef OGRLineIter type; };

    template<>
    struct range_const_iterator<OGRLineString>
    { typedef const_OGRLineIter type; };
}

//namespace frommle{
//    namespace geometry{

        inline OGRLineIter range_begin(OGRLineString& ogrls) {OGRLineIter begin(&ogrls);return ++begin;}
        inline OGRLineIter range_end(OGRLineString& ogrls) {OGRLineIter end(&ogrls);return end;}

        inline const_OGRLineIter range_begin(const OGRLineString& ogrls) {const_OGRLineIter cbegin(&ogrls);return ++cbegin;}
        inline const_OGRLineIter range_end(const OGRLineString& ogrls) {const_OGRLineIter cend(&ogrls); return cend;}

//    }
//}

//register the boost range iterator to loop over the points in a linearRing
using OGRRingIter=frommle::geometry::OGRiterator<OGRLinearRing>;
using const_OGRRingIter=frommle::geometry::const_OGRiterator<OGRLinearRing>;

namespace boost
{
    template <>
    struct range_iterator<OGRLinearRing>
    { typedef OGRRingIter type; };

    template<>
    struct range_const_iterator<OGRLinearRing>
    { typedef const_OGRRingIter type; };
}

//namespace frommle{
//    namespace geometry{

inline OGRRingIter range_begin(OGRLinearRing& ogrls) {OGRRingIter begin(&ogrls);return ++begin;}
inline OGRRingIter range_end(OGRLinearRing& ogrls) {OGRRingIter end(&ogrls);return end;}

inline const_OGRRingIter range_begin(const OGRLinearRing& ogrls) {const_OGRRingIter cbegin(&ogrls);return ++cbegin;}
inline const_OGRRingIter range_end(const OGRLinearRing& ogrls) {const_OGRRingIter cend(&ogrls); return cend;}

//    }
//}


using OGRpolyiter=frommle::geometry::OGRiterator<OGRPolygon>;
using const_OGRpolyiter=frommle::geometry::const_OGRiterator<OGRPolygon>;

using OGRpolyRange=frommle::geometry::OGRiterRange<OGRPolygon>;
//same but will use a different constructor
using const_OGRpolyRange=frommle::geometry::OGRiterRange<OGRPolygon>;


namespace boost {
    namespace geometry {
        namespace traits {
            template<> struct tag<OGRPolygon> { typedef polygon_tag type; };
            template<> struct ring_const_type<OGRPolygon> { typedef const OGRLinearRing& type; };
            template<> struct ring_mutable_type<OGRPolygon> { typedef OGRLinearRing& type; };
            template<> struct interior_const_type<OGRPolygon> { typedef const OGRpolyRange type; };
            template<> struct interior_mutable_type<OGRPolygon> { typedef OGRpolyRange type; };

            template<> struct exterior_ring<OGRPolygon>
            {
                static OGRLinearRing& get(OGRPolygon& p)
                {
                    return *(p.getExteriorRing());
                }
                static OGRLinearRing const& get(OGRPolygon const& p)
                {
                    return *(p.getExteriorRing());
                }
            };

            template<> struct interior_rings<OGRPolygon>
            {
                static OGRpolyRange get(OGRPolygon& p)
                {
                    return OGRpolyRange(p);
                }
                static const const_OGRpolyRange get(OGRPolygon const& p)
                {
                    return const_OGRpolyRange(p);
                }
            };
        }
    }
} // namespace boost::geometry::traits

namespace boost
{
    // Specialize metafunctions. We must include the range.hpp header.
    // We must open the 'boost' namespace.

    template <>
    struct range_iterator<OGRpolyRange> { typedef OGRpolyiter type; };

    template<>
    struct range_const_iterator<OGRpolyRange> { typedef const_OGRpolyiter type; };

} // namespace 'boost'


// The required Range functions. These should be defined in the same namespace
// as Ring.

inline OGRpolyiter range_begin(OGRpolyRange & r)
{return r.begin();}

inline const_OGRpolyiter range_begin(const OGRpolyRange & r)
{return r.cbegin();}

inline OGRpolyiter range_end(OGRpolyRange & r)
{return r.end();}

inline const_OGRpolyiter range_end(const OGRpolyRange & r)
{return r.cend();}





#endif //FROMMLE_OGR2BOOST_HPP
