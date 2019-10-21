/*! \file
 \brief Testing main for routines in the sh module of the toolbox
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

#define BOOST_TEST_MODULE SHtesting
#include <boost/test/unit_test.hpp>
#include "sh/Legendre_nm.hpp"
#include "sh/SHGuide.hpp"
#include "core/IndexGuide.hpp"
#include "sh/Ynm.hpp"
#include <math.h>
#include <iomanip>
#include <algorithm>
#include "geometry/GuideMakerTools.hpp"
using namespace frommle;
using namespace frommle::guides;
using namespace frommle::sh;


double P52(double theta) {
    double sc = sqrt(242550.0 / 860160);
    return sc * (2 * cos(theta) + cos(3 * theta) - 3 * cos(5 * theta));
}


BOOST_AUTO_TEST_CASE(SHguidetest){
    int nmax=5;
    int n,m;
    SHGuideBase::trig t;
    SHtmnGuide shg(nmax);
    int ii=0;
    //internal  consistency check
    bool checkDegOrdTrigIndex;
    for(const auto & tpl:shg){
        std::tie(n,m,t)=tpl;
        std::cout << ii<<" " << n<<" "<<m<<" "<<t<<std::endl;
        checkDegOrdTrigIndex = shg[ii++]==tpl;
        BOOST_TEST(checkDegOrdTrigIndex);
    }
    //also check whether the iterator covered al of the coefficients
    BOOST_TEST(ii==shg.size());



}


BOOST_AUTO_TEST_CASE(assocLegendre,*boost::unit_test::tolerance(1e-11))
{
    const double d2r=std::atan(1.0)*4/180;
    Legendre_nm_d Pnm(500);
    double theta;
    int nsteps=180/0.25;
    double dt=180.0/nsteps;

    BOOST_TEST_MESSAGE("Testing associated Legendre polynomials against analytical P52 solution");
    for(int i=0;i<=nsteps+1;i++){
        theta=dt*i*d2r;
        Pnm.set(cos(theta));
        auto indx=SHnmHalfGuide::Element(5,2);
        BOOST_TEST(Pnm[indx]==P52(theta));
    }

}


//ADD numeric test (close to pole and for high degrees) compare double against long double implementation
BOOST_AUTO_TEST_CASE(stabilityAssocLegendre)
{

    if (sizeof(double) == sizeof(long double)){
        BOOST_TEST_MESSAGE("double and long double have the same size for this compiler skipping stability test");
        return;
    }


    const double d2r=std::atan(1.0)*4/180;
    int nmax=2700;
    Legendre_nm_d Pnm(nmax);
    Legendre_nm_ld Pnmld(nmax);


    //only check colatitudes close (within a degree) to the pole
    int nsteps=100;
    double dt=55555.0/nsteps;

    long double z;
    double dif,reldif;
    double maxv;
    double tol=5e-6;
    double zeroCutoff=1e-50;
    double valld,val;
    bool RELERR_islessthan_TOL;
    BOOST_TEST_MESSAGE("Testing relative stability against long double version (relative error <5e-6 for absolute values > 1e-100) of associated Legendre polynomials within 5 deg of North Pole");
    for(int i=0;i<=nsteps+1;i++){
        z=cos(dt*i*d2r);
        Pnm.set(z);
        Pnmld.set(z);

        for (int m = 0; m <= nmax; ++m) {
            for (int n = m; n <= nmax; ++n) {
                auto idx = Pnm.shg().idx(n, m);
                val=Pnm.mat()[idx];
                //convert to double to compare properly
                valld=Pnmld.mat()[idx];

                dif = abs(val-valld);
                maxv=std::max(abs(val),abs(valld));

                reldif=dif/maxv;
                if( maxv > zeroCutoff ) {
                    //test for relative accuracy
                    RELERR_islessthan_TOL= (reldif < tol)?true:false;
//                    if(not RELERR_islessthan_TOL) {
//                        std::cout <<RELERR_islessthan_TOL<<" " << reldif << " " << val<<" "<< valld<<std::endl;
//                    }
                    BOOST_TEST(RELERR_islessthan_TOL) ;
                }
            }
        }

    }

}


BOOST_AUTO_TEST_CASE(YNMtest,*boost::unit_test::tolerance(1e-11)){
    int nmax=5;
    Ynm<double,SHtmnGuide,OGRPGuide> ynmop(nmax);

    std::vector<double> lon={-179.3,61.0,156.0};
    std::vector<double> lat={-87.0,1.0,32.0};
    auto geoguide=geometry::makePointGuide(lon,lat);
    IndexGuide iguide(4);
    auto outar_multi=ynmop(GA_ones_d(geoguide,iguide));

    auto outar_single=ynmop(GA_ones_d(geoguide));
    LOGINFO << std::string(outar_single.gp()[0]->hash())<< std::endl;
    //sanity check n=5 m=2 solutions
    int n=5;
    int m=2;
    SHGuideBase::trig t=SHGuideBase::trig::C;
    double y52c=0.0;
    double y52s=0.0;
    for(auto &pnt:geoguide) {
        auto p52val = P52(D2R * (90 - pnt->getY()));
        y52c+= p52val * cos(m * D2R * pnt->getX());
        y52s+=p52val * sin(m * D2R * pnt->getX());

//        LOGINFO << n << " " << m <<" " << " " << 0 << " "<< y52c << std::endl;
//        LOGINFO << n << " " << m <<" " << " " << 1 << " "<< y52s << std::endl;
    }
        auto idxc=typename SHtmnGuide::Element(n,m,SHGuideBase::trig::C);
        auto idxs=typename SHtmnGuide::Element(n,m,SHGuideBase::trig::S);

//        for(auto & idxi:iguide){
//            LOGINFO << idxi << std::endl;
//        }

        BOOST_TEST(outar_single[idxc] == y52c);
        BOOST_TEST(outar_single[idxs] == y52s);

        //also check auxiliary columns of outar_multi


//        LOGINFO << n << "" << m<<" "<< outar_single[idxc] << " " << outar_single[idxs];

        auto shg=outar_single.gpp()->as<SHtmnGuide>(0);


        int idx=0;
//        for(auto & el:*shg){
//            std::tie(n,m,t)=el;
//            LOGINFO<<idx++<<" "<<n<<" "<<m<<" "<<t<<std::endl;
//        }
//        idx=0;
//        for(auto & val:outar_single.mat()){
//            LOGINFO << idx++ <<" " << val << std::endl;
//        }


    //    for(int i=0;i<outar.gp()[0])


}
