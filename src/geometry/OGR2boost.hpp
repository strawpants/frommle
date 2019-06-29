/*! \file
 \brief adapted from https://www.boost.org/doc/libs/1_70_0/libs/geometry/doc/html/geometry/examples/example_source_code__adapting_a_legacy_geometry_object_model.html and https://github.com/eveith/ogr-boost-adapter
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
#include <boost/geometry/geometries/register/linestring.hpp>
//#include <boost/geometry/index/rtree.hpp>
#include <boost/range.hpp>
#include <ogr_geometry.h>
#include "geometry/OGRiteratorBase.hpp"

namespace bg = boost::geometry;
//namespace bgi = boost::geometry::index;

#ifndef FROMMLE_OGR2BOOST_HPP
#define FROMMLE_OGR2BOOST_HPP

//Register OGR types to be understood by boos t geometry

////register OGRpoint class so that it is understood by boost geometry
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(OGRPoint,double, bg::cs::geographic<bg::degree>, getX, getY, setX, setY);
//BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(OGRPoint,double, bg::cs::spherical_equatorial<bg::degree>, getX, getY, setX, setY);
BOOST_GEOMETRY_REGISTER_BOX_2D_4VALUES(OGREnvelope, OGRPoint, MinX, MinY, MaxX, MaxY);



namespace fg=frommle::geometry;
namespace frommle{
    namespace geometry {

//register the boost range iterator to loop over the points in a linestring
        using OGRLineIter=fg::OGRPiterator<OGRPoint,OGRLineString>;
        using const_OGRLineIter=fg::OGRPiterator<const OGRPoint,OGRLineString>;
    }
}

namespace boost
{

    template <>
    struct range_iterator<OGRLineString>
    { typedef fg::OGRLineIter type; };

    template<>
    struct range_const_iterator<OGRLineString>
    { typedef fg::const_OGRLineIter type; };
}


        inline fg::OGRLineIter range_begin(OGRLineString& ogrls) {
            auto it=fg::OGRLineIter(&ogrls);
            return it;
        }
        inline fg::OGRLineIter range_end(OGRLineString& ogrls) {
            auto it=fg::OGRLineIter(&ogrls);
            it+=it.size();
        }

        inline fg::const_OGRLineIter range_begin(const OGRLineString& ogrls) {
            auto it=fg::const_OGRLineIter(&ogrls);
            return it;}
        inline fg::const_OGRLineIter range_end(const OGRLineString& ogrls) {
            auto it=fg::const_OGRLineIter(&ogrls);
            it+=it.size();
            return it;
        }

BOOST_GEOMETRY_REGISTER_LINESTRING(OGRLineString);

///register linear ring

namespace frommle{
    namespace geometry {

//register the boost range iterator to loop over the points in a linestring
        using OGRRingIter=fg::OGRPiterator<OGRPoint,OGRLinearRing>;
        using const_OGRRingIter=fg::OGRPiterator<const OGRPoint,const OGRLinearRing>;
    }
}

namespace boost
{

    template <>
    struct range_iterator<OGRLinearRing>
    { typedef fg::OGRRingIter type; };

    template<>
    struct range_const_iterator<OGRLinearRing>
    { typedef fg::const_OGRRingIter type; };
}


inline fg::OGRRingIter range_begin(OGRLinearRing& ogrls) {
    return fg::OGRRingIter(&ogrls);
}

inline fg::OGRRingIter range_end(OGRLinearRing& ogrls) {
    auto it=fg::OGRRingIter(&ogrls);
    it+=it.size();
}

inline fg::const_OGRRingIter range_begin(const OGRLinearRing& ogrls) {
    return fg::const_OGRRingIter(&ogrls);
    }
inline fg::const_OGRRingIter range_end(const OGRLinearRing& ogrls) {
    auto it=fg::const_OGRRingIter(&ogrls);
    it+=it.size();
    return it;
}

namespace boost {
    namespace geometry {
        namespace traits {


//            template<>
//            struct tag<OGRLinearRing *>
//            {
//                typedef ring_tag type;
//            };


            template<>
            struct tag<OGRLinearRing>
            {
                typedef ring_tag type;
            };
        } // namespace traits
    } // namespace geometry
} // namespace boost

//Register the OGRPolygon

class OGRPolyRange{
public:
    using ringiterator=fg::OGRRingIterator<OGRLinearRing,OGRPolygon>;
    using const_ringiterator=fg::OGRRingIterator<const OGRLinearRing,const OGRPolygon>;
    ringiterator begin(){assert(!citer);return begin_;}
    ringiterator end(){assert(!citer);return end_;}
    const_ringiterator cbegin()const{assert(citer);return cbegin_;}
    const_ringiterator cend()const{assert(citer);return cend_;}

    OGRPolyRange(ringiterator & start , ringiterator & end):citer(false),begin_(start),end_(end){
    }

    OGRPolyRange(const_ringiterator & start,const_ringiterator & end):citer(true),cbegin_(start),cend_(end){

    }
    OGRPolyRange(const OGRPolyRange & crange){

    }

    const OGRPolyRange& operator =(const OGRPolyRange & crange){
        return *this;
    }


private:
    bool citer=false;
    ringiterator begin_{};
    ringiterator end_{};
    const_ringiterator cbegin_{};
    const_ringiterator cend_{};
};


namespace frommle{
    namespace geometry{

    //register the boost range iterator to loop over the points in a linearRing
    using OGRPolyIter=frommle::geometry::OGRRingIterator<OGRLinearRing,OGRPolygon>;
    using const_OGRPolyIter=frommle::geometry::OGRRingIterator<const OGRLinearRing,const OGRPolygon>;

    }
}
namespace boost {
    template <>
    struct range_iterator<OGRPolygon>
    {
        typedef fg::OGRPolyIter type;
    };


    template <>
    struct range_const_iterator<OGRPolygon>
    {
        typedef fg::const_OGRPolyIter type;
    };
}


inline fg::OGRPolyIter range_begin(OGRPolygon& ogrls) {
    return fg::OGRPolyIter(&ogrls);
}

inline fg::OGRPolyIter range_end(OGRPolygon& ogrls) {
    auto it=fg::OGRPolyIter(&ogrls);
    it+=it.size();
    return it;
}

inline fg::const_OGRPolyIter range_begin(const OGRPolygon& ogrls) {
    return fg::const_OGRPolyIter(&ogrls);
}

inline fg::const_OGRPolyIter range_end(const OGRPolygon& ogrls) {
    auto it=fg::const_OGRPolyIter(&ogrls);
    it+=it.size();
    return it;
}


namespace boost{
    namespace geometry {
        namespace traits {

            template<>
            struct tag<OGRPolygon> {
                typedef polygon_tag type;
            };


            template<>
            struct ring_const_type<OGRPolygon> {
                typedef const OGRLinearRing & type;
            };


            template<>
            struct ring_mutable_type<OGRPolygon> {
                typedef OGRLinearRing & type;
            };


            template<>
            struct interior_const_type<OGRPolygon> {
                typedef const OGRPolyRange  type;
            };


            template<>
            struct interior_mutable_type<OGRPolygon> {
                typedef OGRPolyRange  type;
            };


            template<>
            struct exterior_ring<OGRPolygon> {
                static OGRLinearRing &get(OGRPolygon &p) {
                    return *(p.getExteriorRing());
                }

                static const OGRLinearRing &get(OGRPolygon const &p) {
                    return *(p.getExteriorRing());
                }
            };


            template<>
            struct interior_rings<OGRPolygon> {
                static OGRPolyRange get(OGRPolygon &p) {
                    fg::OGRPolyIter start(&p);
                    fg::OGRPolyIter end(&p);
                    if(start.size()) {
                        start += 1;//exclude 0 (exterior ring)
                        end += end.size();
                    }
                    return OGRPolyRange(start,end);
                }

                static const OGRPolyRange get(const OGRPolygon & p){
                    fg::const_OGRPolyIter start(&p);
                    fg::const_OGRPolyIter end(&p);
                    if(start.size()) {
                        start += 1;//exclude 0 (exterior ring)
                        end += end.size();
                    }
                    return OGRPolyRange(start,end);
                }
            };
        }

    }
}

namespace boost
{
    template <>
    struct range_iterator<OGRPolyRange> { typedef fg::OGRPolyIter type; };

    template<>
    struct range_const_iterator<OGRPolyRange> { typedef fg::const_OGRPolyIter type;};

} // namespace 'boost'

inline fg::OGRPolyIter range_begin(OGRPolyRange & r)
{return r.begin();}

inline fg::const_OGRPolyIter range_begin(const OGRPolyRange& r)
{return r.cbegin();}

inline fg::OGRPolyIter range_end(OGRPolyRange& r)
{return r.end();}

inline fg::const_OGRPolyIter range_end(const OGRPolyRange& r)
{return r.cend();}


#endif //FROMMLE_OGR2BOOST_HPP
