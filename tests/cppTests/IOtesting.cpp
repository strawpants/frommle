/*! \file
 \brief Testing IO routines of the toolbox
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

 Created by Roelof Rietbroek, 2019

 */

#define BOOST_TEST_MODULE IOtesting
#include <boost/test/unit_test.hpp>
#include <cstdio>
#include "core/Exceptions.hpp"

#include "io/OGRArchive.hpp"
#include "io/OGRIOArchives.hpp"
#include "geometry/OGRGuide.hpp"
#include <boost/filesystem.hpp>
#include "io/LineBuffer.hpp"
#include "geometry/fibonacciGrid.hpp"
#include "core/GArrayBase.hpp"
#include "io/NetCDFIO.hpp"
#include "core/IndexGuide.hpp"

using namespace frommle::io;

using namespace frommle;

namespace utf=boost::unit_test;


//BOOST_AUTO_TEST_CASE(GroupsnVar){
//    core::TreeNodeRef root(new Group("rootgroup",{{"Testattr",3},{"other","attval"}}));
//
//    //add a subgroup by name
//    root["subgroup"]=Group();
//
//    //add a variable by index
//    root[3]=Variable("var1",{{"long_name",std::string("longitude")},{"values",5}});
//
//    BOOST_TEST(root->getName() == "rootgroup");
//    BOOST_TEST(root[0]->getName() == "subgroup");
//
//    BOOST_CHECK_THROW(root["var1"]["thisshouldfail"],core::MethodException);
//
//    //check for correct parenting assignment
//    BOOST_TEST(root["var1"]->getParent()->ref()->getName() == "rootgroup");
//
//    //test for attribute retrieval
//    BOOST_TEST(root->getAttribute<int>("Testattr")==3);
//    BOOST_TEST(root["var1"]->getAttribute<std::string>("long_name") == "longitude");
//    }
//
//}

geometry::OGRGuide<OGRPolygon> makeTestOGRGuide(){
    geometry::OGRGuide<OGRPolygon> polyGuide{};
    //open file with test wkt polygons
    LineBuffer lbuf("data/testpolygonswkt.txt");

    for(auto & ln:lbuf){
        polyGuide.push_back(ln.str());

    }
    //add a test polygon with 2 inner rings (note the provided polygon rings are sorted in the way which is expected in an esri shapefile (outer: counter clockwise, inner: clockwise)
//    polyGuide.push_back("POLYGON ((-10 50,20 45,19.3 -2.4,-9 10, -10 50),(-1 38,1 30,2 40,-2 45, -1 38),(4 18,9 15,10 29,5 30, 4 18))");
    return polyGuide;

}

//Test writing & reading OGR geometries from shapefiles / database
BOOST_AUTO_TEST_CASE(RWOGRArchive){
    auto PolyGd=makeTestOGRGuide();
    std::string gdalfile("OGRtestdata");

    {//open an gdal file for writing
        io::OGRArchive oAr(gdalfile, {{"mode",   "w"},
                                      {"Driver", "ESRI Shapefile"}});
        auto &grp = oAr.getGroup("newlayer");
        grp << PolyGd;

        auto &grp2=oAr.getGroup("fibonacci");
        grp2 <<geometry::makeFibonacciGrid(10000);

    }

    geometry::OGRGuide<OGRPolygon> PolyGdTest{};
    //now read the same stuff backi in
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
    char ** wkt =new char*;
    char ** wkt2 =new char*;
    for(const auto geo:PolyGd ){
        geo->exportToWkt(wkt);
        (*tgeo)->exportToWkt(wkt2);

        GeomsAretheSame=(*geo == **tgeo);
        if (not GeomsAretheSame){
            break;
        }
        ++tgeo;
    }

    BOOST_TEST(GeomsAretheSame);


    //delete dataset
    boost::filesystem::remove_all(boost::filesystem::path(gdalfile));



}

///@brief create a test guided array
core::Garray<double,core::IndexGuide,core::IndexGuide> createTestGarray(){
    auto garr=core::make_garray(core::IndexGuide(13),core::IndexGuide(97));
    garr=22.0;

    return garr;
}

BOOST_AUTO_TEST_CASE(RWNetCDFArchive){
    std::string fout("Testnc.nc");
    {
        NetCDFArchive oAr(fout,{{"mode","w"},{"title","Test dataset created in the IOtesting suite"}});

        auto & grp =oAr.getGroup("subgroup1");

        grp << createTestGarray();
        
    }



}

//@brief test the retrieval of OGR geometries from a PostGIS-enabled database
BOOST_AUTO_TEST_CASE(OGRPostGIS,* utf::disabled()){

}


