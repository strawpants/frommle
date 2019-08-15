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
#include "core/Constants.hpp"
#include "core/Logging.hpp"

using namespace frommle::core;
namespace bg=boost::gregorian;
BOOST_AUTO_TEST_CASE(Garray1n2n3d){

    auto TGuide=make_trange(bg::date(2002,12,1),bg::date(2003,3,30),bg::days(1));
    auto garr1d=make_garray(std::move(TGuide));
    garr1d.setName("Blah");
    garr1d=M_PI;
    std::cout << garr1d.name()<<" "<<garr1d.mat()[0] << " " << std::string(garr1d.g(0)->hash())<< std::endl;

    //2D example
    size_t nrows=23;
    size_t ncols=4;
    double value=3.0;
    auto garr2d=make_garray(IndexGuide(nrows),IndexGuide(ncols));

    //now try to change the values through an eigen matrix wrapper
    auto eigm=garr2d.eig();
    eigm.array()=value;

    BOOST_TEST(eigm.sum() == nrows*ncols*value);
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
    UserSettings::setAuth("Frommledummy","fuser",secretpassword);
    auto checkpassword=UserSettings::getAuth("Frommledummy","fuser");

    BOOST_TEST(secretpassword == checkpassword);

    //the same but now stored unsecurely
    UserSettings::at("Authstore")="unsecure";
    UserSettings::setAuth("Frommledummy","fuser",secretpassword);
    auto checkunsecurepassword=UserSettings::getAuth("Frommledummy","fuser");
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
class handbag;
class wallet;
class coin;

class money:public TreeNodeItem{
public:
    money(int val):val_(val){}
    int val_=0;
    virtual bool iscoin(){return false;}
private:
    void parentHook();
};

class coin:public money{
private:
    virtual bool iscoin(){return true;}
public:
    coin(int value): money(value){}
};

class note:public money{
private:
public:
    note(int value): money(value){}
};



class handbag:public TreeNodeCollection{
public:
    handbag(const std::string && name):TreeNodeCollection(name){}
    void add(int val){loose_value+=val;}
    int loose_value=0;
    int value();
private:
};


class wallet:public TreeNodeCollection{
public:
    void add(int val){val_+=val;}

    int val_=0;
private:
    virtual TreeNodeRef convertChild(TreeNodeRef && in);
};


TreeNodeRef wallet::convertChild(TreeNodeRef &&in) {

    //sepcialize whether we have a coin or  note
    int val = in.as<money>().val_;
    if (val < 5) {
        return TreeNodeRef(coin(val));
    } else {
        return TreeNodeRef(note(val));
    }
}


void money::parentHook() {
    //add the amount of this coin to the parent
    auto walletptr=dynamic_cast<wallet*>(getParent());
    if(walletptr){
        //add value to the wallet
        walletptr->add(val_);
        return;
    }

    auto handbagptr=dynamic_cast<handbag*>(getParent());
    if(handbagptr){
        //add value to the handbag
        handbagptr->add(val_);
        return;
    }



}


int handbag::value() {
    int val=0;
    //loop over wallets and coins
    for(auto &wl:*this){
        if( wl->isCollection()) {
            val += wl.as<wallet>().val_;
        }else{
            val += wl.as<money>().val_;
        }
    }
    return val;
}


BOOST_AUTO_TEST_CASE(TreeNodes){

    handbag bag("handbag");
    bool handbagwithwallet=bool(bag["wallet"]);

    BOOST_TEST(handbagwithwallet == false);

    //add a wallet in the handbag
    bag["wallet"]=wallet();

    //also add a 2euro  coin in the handbag
    bag["euro2"]=coin(2);

    //add two generic money items in the wallet
    bag["wallet"]["euro1"]=money(1);
    bag["wallet"]["euro5"]=money(5);

    bool walletwitheurocoin=bool(bag["wallet"]["euro1"]);
    //test if the wallet contains  a 1 euro coin
    BOOST_TEST(walletwitheurocoin);

    auto & euro1 = bag["wallet"]["euro1"].as<money>();

    BOOST_TEST(euro1.iscoin());


    //check the value of the loose  amount of money in the handbag
    BOOST_TEST(bag.loose_value == 2);

    BOOST_TEST(bag.value() == 8 );


}
