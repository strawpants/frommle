/*! \file
 \brief Testing main for routines in the core module of the toolbox
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

#define BOOST_TEST_MODULE GuideTesting
#include <boost/test/unit_test.hpp>
#include "core/IndexedGuide.hpp"
#include "core/Logging.hpp"
//#include "core/GuidePack.hpp"

using namespace frommle::guides;

    struct TestElement{
        TestElement(size_t i):id(i){}
        size_t id;    };

BOOST_AUTO_TEST_CASE(Indexed){



    IndexedGuide<TestElement> IGuide;

    for(int i=20;i>3;i-=2){
        IGuide.push_back(TestElement(i));
    }

    //mask the guide
    IGuide.mask([](const TestElement & el)->bool{
            return (el.id >10)?true:false;
            });
    
    for(auto & it:IGuide){
        LOGINFO << "entry "<< it.i <<" " <<it.el.id <<  std::endl;
    }
    
    IGuide.unmask();

    for(auto & it:IGuide){
        LOGINFO << "unmasked entry "<< it.i <<" revin" << IGuide.memindx()[it.i].i <<" "<<it.el.id <<  std::endl;
    }
}


//BOOST_AUTO_TEST_CASE(GuideBase){


    //BOOST_TEST(1 == 1);



//}



//BOOST_AUTO_TEST_CASE(GuidePack){


    //BOOST_TEST(1 == 1);



//}

