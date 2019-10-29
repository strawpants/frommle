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
#define BOOST_TEST_MODULE GeometryTesting
#include <boost/test/unit_test.hpp>
#include "geometry/GeoGrid.hpp"
#include "geometry/OGRGuide.hpp"
//#include "geometry/OGRiteratorBase.hpp"
#include "geometry/OGR2boost.hpp"
#include "core/Logging.hpp"
#include "io/OGRArchive.hpp"
#include "io/OGRIOArchives.hpp"
#include <boost/filesystem.hpp>
#include <boost/variant.hpp>
#include "geometry/GuideMakerTools.hpp"
#include "io/LineBuffer.hpp"
#include "geometry/geomOperator.hpp"
#include <ogr_geometry.h>
#include "core/IndexGuide.hpp"

using namespace frommle::geometry;
using namespace frommle::guides;
using namespace frommle;
namespace utf=boost::unit_test;
namespace tt = boost::test_tools;
BOOST_AUTO_TEST_CASE(GeoGridGuide,*utf::tolerance(1e-11)){
    //Test the functionality of a GeoGrid (equidistant grid)
    std::array<double,4> wesn={-180,180,-90,90};
    double dlon=0.49;//make this increment not fit on purpose
    double dlat=1.0;

    std::vector<GeoGrid::gridreg> regs={GeoGrid::gridreg::pix,GeoGrid::gridreg::grid};

    //test for the two registration forms
    for(auto & reg:regs) {
        auto grd = GeoGrid(wesn[0], wesn[1], wesn[2], wesn[3], dlon, dlat, reg);
        double lat = wesn[2] + (1 - reg) * dlat * 0.5;
        double lon = wesn[0] + (1 - reg) * dlon * 0.5;
        long unsigned int idx=0;
        long unsigned int idxck;
        long unsigned int ilon=0;
        long unsigned int ilat=0;
        long unsigned int ilonck=0;
        long unsigned int ilatck=0;

        for (const auto &loc:grd) {

            auto tpl=grd.ilonilat(idx);
            std::tie(ilonck,ilatck)=tpl;
            idxck=grd.idx(ilon,ilat);
            BOOST_TEST(lon == loc.getX());
            BOOST_TEST(lat == loc.getY());
            BOOST_TEST(ilon == ilonck);
            BOOST_TEST(ilat == ilatck);
            BOOST_TEST(idx==idxck);

//            std::cout << lon << " " << lat << " " << loc.getX() << " " << loc.getY() << std::endl;
            ++idx;
            ++ilon;
            lon += dlon;
            if (lon > wesn[1]-(1-reg)*0.5*dlon) {
                lon = wesn[0]+(1-reg)*0.5*dlon;
                ilon=0;
                ++ilat;
                lat += dlat;
            }
        }
    }
}


BOOST_AUTO_TEST_CASE(GeoPointsGuide){

    std::vector<double> lon={-179.3,60.0,180.0};
    std::vector<double> lat={-87.0,0.0,34};
    using GeoPoints=OGRGuide<geopoint>;
    GeoPoints geopnts=GeoPoints();
    //push back with a OGRPoint
    geopnts.push_back(geopoint(lon[0],lat[0]));
    auto pnt=geopoint(lon[1], lat[1]);
    geopnts.push_back(pnt);
    unsigned int idx=0;
    for (const auto & loc:geopnts){
        BOOST_TEST(lon[idx] == loc->getX());
        BOOST_TEST(lat[idx] == loc->getY());
        ++idx;
    }
}

OGRLineString makeOGRLineStr(){
    OGRSpatialReference *SpatialRef_=OGRSpatialReference::GetWGS84SRS();

    OGRGeometry ** geomptr= new OGRGeometry*;
    std::string WKT("LineString (-35.58577405857741383 21.35146443514642556, -1.58158995815898606 24.11924686192467959, 4.34937238493725431 47.05230125523011964, 31.23640167364015952 53.77405857740585304, 60.49581589958160066 35.98117154811714613)");
    if (OGRERR_NONE != OGRGeometryFactory::createFromWkt(WKT.c_str(),SpatialRef_,geomptr)){
        THROWINPUTEXCEPTION("Failed to create OGR geometry");
    }
    OGRLineString ogrline(static_cast<OGRLineString&>(**geomptr));
    OGRGeometryFactory::destroyGeometry(*geomptr);
    delete geomptr;
    return ogrline;
}


OGRPolygon makeOGRPolygon(){
    OGRSpatialReference *SpatialRef_=OGRSpatialReference::GetWGS84SRS();
    OGRGeometry ** geomptr= new OGRGeometry*;
    std::string WKT("POLYGON ((-10 50,20 45,19.3 -2.4,-9 10, -10 50),(-1 38,1 30,2 40,-2 45,-2 40, -1 38),(4 18,9 15,10 29,5 30, 4 18))");
    if (OGRERR_NONE != OGRGeometryFactory::createFromWkt(WKT.c_str(),SpatialRef_,geomptr)){
        THROWINPUTEXCEPTION("Failed to create OGR geometry");
    }
    OGRPolygon ogrpoly(static_cast<OGRPolygon&>(**geomptr));
    OGRGeometryFactory::destroyGeometry(*geomptr);
    delete geomptr;
    return ogrpoly;

}

OGRLinearRing makeOGRRing(){
    auto poly=makeOGRPolygon();
    OGRLinearRing *ogrring=poly.getExteriorRing();
    return OGRLinearRing(*ogrring);
}

//test running OGR entities through boost geometry functions
BOOST_AUTO_TEST_CASE(OGR2BoostGeometry, *utf::tolerance(1e-4)){
    namespace bg = boost::geometry;

    //check for properly registered box
    OGREnvelope box=bg::make<OGREnvelope>(-1.0,-1.0,1.0,1.0);
    BOOST_TEST(bg::area(box)== 4);

    //check for functioning linestring
    OGRLineString linest(makeOGRLineStr());
    double length=bg::length(linest);
    BOOST_TEST(length==1.1137827094e+07);

    //check for functioning linear ring
    OGRLinearRing ringst(makeOGRRing());
    double ringarea=bg::area(ringst);
    BOOST_TEST(ringarea==13886524452171.0);

    //check for functioning OGRpolygon
    OGRPolygon poly(makeOGRPolygon());
    char ** wkt=new char*;
    poly.exportToWkt(wkt);
    auto end=const_OGRPolyIter(&poly);
    end+=end.size();
    std::string message;
//    LOGINFO << message;
//    for(auto it=const_OGRPolyIter(&poly);it<end;++it){
//      area+=bg::area(*it);
////        LOGINFO << "area: "<< bg::area(*it)<< " " << area;
//    }
    double area=bg::area(poly);

    BOOST_TEST(area==12859038681013.0);


    //currently is_valid is not working.. :(
//    bool valid=bg::is_valid(poly);
}

///Test masking operations of geometries within other geometries
//BOOST_AUTO_TEST_CASE(MaskGeometry){
    //using polyguide=OGRGuide<OGRPolygon>;
    //auto polymask=std::make_shared<polyguide>("poly");

    ////open file with test wkt polygons
    //io::LineBuffer lbuf("data/testpolygonswkt.txt");

    //for(auto & ln:lbuf){
        //polymask->push_back(ln.str());

    //}

    ////create a Fibonaccigrid with assigned values which is going to be masked
    //using pointguide=OGRGuide<OGRPoint>;
    //auto fibarr=core::make_garray<int>(makeFibonacciGrid(10000),core::IndexGuide(1));
    //fibarr=1;

    //withinOperator<OGRPolygon,pointguide> MaskOp(polymask);

    //auto fibmask=MaskOp(fibarr);
    //BOOST_TEST(fibmask.g<0>()->size()==1000);
    ////save to file for checking in Qgis

//}


