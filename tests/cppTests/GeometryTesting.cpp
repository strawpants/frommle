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

#include <boost/variant.hpp>
using namespace frommle::geometry;
using namespace frommle;
namespace utf=boost::unit_test;
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

OGRGuide<OGRPolygon> makeTestOGRGuide(){
    OGRGuide<OGRPolygon> polyGuide{};
    //add a test polygon with 2 inner rings
    polyGuide.push_back("POLYGON((-10 50, 20 45, 19.3 -2.4, -9 10),(-2 45, 2 40, 1 30, -1 38),(5 30, 10 29, 9 15, 4 18))");
    return polyGuide;
}

//Test writing & reading OGR geometries from shapefiles / database
BOOST_AUTO_TEST_CASE(OGRArchive){
    auto PolyGd=makeTestOGRGuide();
    std::string gdalfile("OGRtestdata");

    {//open an gdal file for writing
        io::OGRArchive oAr(gdalfile, {{"mode",   "w"},
                                      {"Driver", "ESRI Shapefile"}});
        auto &grp = oAr.getGroup("newlayer");
        grp << PolyGd;
    }

    OGRGuide<OGRPolygon> PolyGdTest{};
    //now read the same stuff backin
    {

        io::OGRArchive iAr(gdalfile, {{"mode",   "r"}});
        auto &grp = iAr.getGroup("newlayer");
        grp >> PolyGdTest;

    }



    //check whether the 2 OGR guides have the same length
    BOOST_TEST(PolyGd.size() == PolyGdTest.size());
    //check the actual polygons
    auto tgeo=PolyGdTest.begin();
    bool GeomsAretheSame=true;

    for(const auto geo:PolyGd ){
        GeomsAretheSame=(*geo == **tgeo);
        if (not GeomsAretheSame){
            break;
        }
        ++tgeo;
    }

    BOOST_TEST(GeomsAretheSame);


    //delete dataset




}

//@brief test the retrieval of OGR geometries from a PostGIS-enabled database
BOOST_AUTO_TEST_CASE(OGRPostGIS,* utf::disabled()){

}

