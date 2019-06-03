/*! \file
 \brief Testing main for routines in the core module of the toolbox
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

 Created by Roelof Rietbroek, 2018

 */

#define BOOST_TEST_MODULE coretesting
#include <boost/test/unit_test.hpp>
#include "core/GArrayBase.hpp"
#include "core/TimeGuide.hpp"
#include <boost/date_time.hpp>
#include "core/IndexGuide.hpp"
#include "core/MarrayEig.hpp"
#include "core/UserSettings.hpp"
#include <cstdio>
#include "core/TreeNode.hpp"

using namespace frommle::core;
namespace bg=boost::gregorian;
BOOST_AUTO_TEST_CASE(Garray1n2n3d){

    auto TGuide=make_trange(bg::date(2002,12,1),bg::date(2003,3,30),bg::days(1));
    auto garr1d=make_garray(std::move(TGuide));
    garr1d.setName("Blah");
    std::cout << garr1d.name()<<" "<<garr1d[0] << " " << garr1d.g(0).type()<< std::endl;


    //2D example
    size_t nrows=23;
    size_t ncols=4;
    auto garr2d=make_garray(IndexGuide(nrows),IndexGuide(ncols));

    auto eigm=marray_to_eigen(garr2d);
    eigm.array()=3.0;



//    boost::multi_array<double,1> barr2d(shp);
//    for(auto & el:garr2d){
//        std::cout << el << std::endl;
//    }

    std::cout << garr2d[0][0] << " " << garr2d.g(1).type()<< std::endl;



    BOOST_TEST( "ksmc" == "Blah");
}


BOOST_AUTO_TEST_CASE(Settings) {

    //first create the default template
    std::string yamlfile("default.yaml");
    UserSettings::get().setDefaults();
    UserSettings::write(yamlfile);
    UserSettings::read(yamlfile);

    std::string user = UserSettings::at("User").as<std::string>();
    std::string usercheck = std::string(std::getenv("USER"));

    //first trivial check to see if write -> read cycle went well
    BOOST_TEST(user == usercheck);

    //change some data on the fly
    std::string modentry("Modified");
    UserSettings::at("History").push_back(modentry);

    BOOST_TEST(UserSettings::at("History")[0].as<std::string>() == modentry);
    //write and read file
    UserSettings::write(yamlfile);
    UserSettings::read(yamlfile);

    BOOST_TEST(UserSettings::at("History")[0].as<std::string>() == modentry);

    //store and lookup a password
    std::string secretpassword("Blafjasldkmvapdkva;dkvas");
    UserSettings::setAuth("Frommledummy",secretpassword);
    auto checkpassword=UserSettings::getAuth("Frommledummy");

    BOOST_TEST(secretpassword == checkpassword);

    //the same but now stored unsecurely
    UserSettings::at("Authstore")="unsecure";
    UserSettings::setAuth("Frommledummy",secretpassword);
    auto checkunsecurepassword=UserSettings::getAuth("Frommledummy");
    BOOST_TEST(secretpassword == checkunsecurepassword);


    UserSettings::write(yamlfile);

    //remove file
    if (std::remove(yamlfile.c_str()) != 0){
        BOOST_TEST_MESSAGE("Error deleting file");
        BOOST_TEST( false);
    }
}

//Tree node testing
//forward declare node types
class rootNode;
class subNode;
class endNode;

class rootNode:public TreeNode<rootNode,subNode,void>{
public:
    rootNode(const std::string && name):TreeNode<rootNode,subNode,void>(name){}
    virtual TreeNodeRef<subNode> operator[](const std::string & name);
    virtual TreeNodeRef<subNode> operator[](const size_t & indx);
private:
};


class subNode:public TreeNode<subNode,endNode,rootNode>{
    virtual TreeNodeRef<endNode> operator[](const std::string & name);
    virtual TreeNodeRef<endNode> operator[](const size_t & indx);

};


class endNode:public TreeNode<endNode,void,subNode>{
//    virtual TreeNodeRef<void> operator[](const std::string & name);
//    virtual TreeNodeRef<void> operator[](const size_t & indx);

};


//implementation of the functions (once all classes have been defined)
TreeNodeRef<subNode> rootNode::operator[](const std::string &name) {return TreeNodeRef<subNode>();}

TreeNodeRef<subNode> rootNode::operator[](const size_t &indx) {return TreeNodeRef<subNode>();}

TreeNodeRef<endNode> subNode::operator[](const std::string &name) {
    return TreeNodeRef<endNode>();
}

TreeNodeRef<endNode> subNode::operator[](const size_t &indx) {
    return TreeNodeRef<endNode>();
}


BOOST_AUTO_TEST_CASE(TreeNodes){

    rootNode root("noparentnode");

    //add subnodes to the root node
    root["test"]=subNode();

    root["test2"]=subNode();

    //add endnodes to the subnodes
    root["test"]["end1"]=endNode();

    root["test"]["end2"]=endNode();





    std::cout << root.getName() <<std::endl;



}