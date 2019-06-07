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
#include "io/Group.hpp"
#include "core/Exceptions.hpp"
using namespace frommle::io;

using namespace frommle;


BOOST_AUTO_TEST_CASE(GroupsnVar){
    core::TreeNodeRef root(new Group("rootgroup",{{"Testattr",3},{"other","attval"}}));

    //add a subgroup by name
    root["subgroup"]=Group();

    //add a variable by index
    root[3]=Variable("var1",{{"long_name",std::string("longitude")},{"values",5}});

    BOOST_TEST(root->getName() == "rootgroup");
    BOOST_TEST(root[0]->getName() == "subgroup");

    BOOST_CHECK_THROW(root["var1"]["thisshouldfail"],core::MethodException);

    //check for correct parenting assignment
    BOOST_TEST(root["var1"]->getParent()->ref()->getName() == "rootgroup");

    //test for attribute retrieval
    BOOST_TEST(root->getAttribute<int>("Testattr")==3);
    BOOST_TEST(root["var1"]->getAttribute<std::string>("long_name") == "longitude");
    }




