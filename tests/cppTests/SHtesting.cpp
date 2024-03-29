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
#include "sh/Legendre.hpp"
#include "sh/SHGuide.hpp"
#include "core/IndexGuide.hpp"
#include "sh/Ynm.hpp"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "geometry/GuideMakerTools.hpp"
#include "sh/SHisoOperator.hpp"
using namespace frommle;
using namespace frommle::guides;
using namespace frommle::sh;


double P52(double theta) {
    double sc = sqrt(242550.0 / 860160);
    return sc * (2 * cos(theta) + cos(3 * theta) - 3 * cos(5 * theta));
}


//@brief test function generated with sagemath
double P7(double x){
    return 429/16.0*std::pow(x,7) - 693/16.0*std::pow(x,5) + 315/16.0*std::pow(x,3)- 35/16.0*x;
}

//BOOST_AUTO_TEST_CASE(SHguidetest){
    //int nmax=5;
    //int n,m;
    //SHGuideBase::trig t;
    //SHtmnGuide shg(nmax);
    //int ii=0;
    ////internal  consistency check
    //bool checkDegOrdTrigIndex;
    //for(const auto & tpl:shg){
        //std::tie(n,m,t)=tpl;
        //std::cout << ii<<" " << n<<" "<<m<<" "<<t<<std::endl;
        //checkDegOrdTrigIndex = shg[ii++]==tpl;
        //BOOST_TEST(checkDegOrdTrigIndex);
    //}
    ////also check whether the iterator covered al of the coefficients
    //BOOST_TEST(ii==shg.size());



//}
BOOST_AUTO_TEST_CASE(SHguidetest){
    int nmax=5;
    int n,m;
    SHGuideBase::trig t;
    using tpl=SHGuide::Element;
    //construct using the defaulkt sorting scheme
    SHGuide shg(nmax);
    int ii=0;
    ////internal  consistency check
    bool checkDegOrdTrigIndex;
    for(const auto & eli:shg){
        std::tie(n,m,t)=eli;
        //std::cout << eli.i <<" "<< ii<<" " << n<<" "<<m<<" "<<t<<std::endl;
        checkDegOrdTrigIndex = shg[ii++] == eli;
        BOOST_TEST(checkDegOrdTrigIndex);
    }
    //also check whether the iterator covered al of the coefficients
    BOOST_TEST(ii==shg.size());


    //now do some masking (only select degree 1 coefficients)
    shg.mask([](const tpl& tpin)->bool{
                return std::get<0>(tpin) != 1;
            });
    

    for(const auto & eli:shg){
        std::tie(n,m,t)=eli;
        BOOST_TEST(n==1);
    }

    BOOST_TEST(4==shg.size());
    

    //unmask again
    shg.unmask();


    BOOST_TEST(ii==shg.size());


}

BOOST_AUTO_TEST_CASE(LegendrePoly,*boost::unit_test::tolerance(1e-11)){
    int nmax=50000;
    Legendre<double> Pn(nmax);
    int nsamples=1000;
    for(int i=0;i<nsamples;++i){
        double x=-1+(2.0*i)/nsamples;
        Pn.set(x);
        //LOGINFO << (5*std::pow(x,3)-3*x)/2<< " "<<Pn.get()[3] <<std::endl;
        BOOST_TEST(P7(x) == Pn[7]);
    }
    

}

BOOST_AUTO_TEST_CASE(assocLegendre,*boost::unit_test::tolerance(1e-11))
{
    //const double d2r=std::atan(1.0)*4/180;
    Legendre_nm_d Pnm(500);
    double theta;
    int nsteps=180/0.25;
    double dt=180.0/nsteps;

    BOOST_TEST_MESSAGE("Testing associated Legendre polynomials against analytical P52 solution");
    for(int i=0;i<=nsteps+1;i++){
        theta=dt*i*D2R;
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


//BOOST_AUTO_TEST_CASE(YNMtest,*boost::unit_test::tolerance(1e-11)){
    //int nmax=1000;
    //Ynm<double,SHnmtGuide,OGRPGuide> ynmop(nmax);

    //std::vector<double> lon={-179.3,61.0,156.0};
    //std::vector<double> lat={-87.0,1.0,32.0};
    //auto geoguide=geometry::makePointGuide(lon,lat);
    //int naux=4;
    
    //IndexGuide iguide(naux);
    
    ////apply operator for single column input
    //auto outar_single=ynmop(GA_ones_d(geoguide));
    
    
    ////to compute the result for input with multiple columns first construct an array with ones
    ////auto onesar=GA_ones_d(geoguide,iguide);
    //auto onesar=GAconstruct<double>::zeros(geoguide,iguide);

    ////modify the array with ones so that the columns have distinct (but predictable) values 
    ////boost marray types used for slicing
    //auto allrows=onesar.mrange();
    //auto indices=onesar.mindices(); 
    //for(int i = 0; i< naux;++i){
        ////create a subview of the underlying column
        //auto colview=onesar.mat()[ indices[allrows][i]];
        //std::fill(colview.begin(),colview.end(),i+1);
    //}

    ////pply the forward operator
    //auto outar_multi=ynmop(onesar);

    ////sanity check n=5 m=2 solutions
    //int n=5;
    //int m=2;
    //double y52c=0.0;
    //double y52s=0.0;
    //for(auto &pnt:geoguide) {
        //auto p52val = P52(D2R * (90 - pnt->getY()));
        //y52c+= p52val * cos(m * D2R * pnt->getX());
        //y52s+=p52val * sin(m * D2R * pnt->getX());

////        LOGINFO << n << " " << m <<" " << " " << 0 << " "<< y52c << std::endl;
////        LOGINFO << n << " " << m <<" " << " " << 1 << " "<< y52s << std::endl;
    //}
        //auto idxc=typename SHGuideBase::Element(n,m,SHGuideBase::trig::C);
        //auto idxs=typename SHGuideBase::Element(n,m,SHGuideBase::trig::S);

        ////for(auto const & gd:outar_multi.gp()){
            ////LOGINFO << gd->hash() << std::endl;
        ////}
        ////auto ig1=onesar.gp().as<IndexGuide>(1);        
        ////auto ig2=outar_multi.gp().as<IndexGuide>(1);        

        //BOOST_TEST(outar_single[idxc] == y52c);
        //BOOST_TEST(outar_single[idxs] == y52s);

        ////alos test whether all the columns spanned by Indexguide  yield the appropriate result
        //for(auto & idxi:*(outar_multi.gp().as<IndexGuide>(1))){
            ////LOGINFO << idxi << " ";
            ////LOGINFO << outar_multi[idxc][idxi] << std::endl;
            //BOOST_TEST(outar_multi[idxc][idxi] == (idxi+1)*y52c);
            //BOOST_TEST(outar_multi[idxs][idxi] == (idxi+1)*y52s);
        //}


        ////also check auxiliary columns of outar_multi


////        LOGINFO << n << "" << m<<" "<< outar_single[idxc] << " " << outar_single[idxs];

////        for(auto & el:*shg){
////            std::tie(n,m,t)=el;
////            LOGINFO<<idx++<<" "<<n<<" "<<m<<" "<<t<<std::endl;
////        }
////        idx=0;
////        for(auto & val:outar_single.mat()){
////            LOGINFO << idx++ <<" " << val << std::endl;
////        }


    ////    for(int i=0;i<outar.gp()[0])


//}


///@brief test the isotropic kernels in the spherical harmonic domain
//BOOST_AUTO_TEST_CASE(shisokernel){
    //int nmax=20;
        //guides::SHtmnGuide shgd(nmax);

        //guides::IndexGuide igd(7);
        //auto onesar=GAconstruct<double>::ones(shgd,igd);

        ////create a kernel which just has the degrees as scale factors
        //std::vector<double> kernel(nmax+1);
        //std::iota(kernel.begin(),kernel.end(),1.0);

        ////create an isostropic diagonal operator
        //sh::SHisoOperator<double> shkernel(kernel,shgd);

        //auto shout=shkernel(onesar);
    
        ////also create the inverse operator in combination with the original operator (should return thee input again)
        
        //auto shoutrecover=shkernel(shkernel.inverse())(onesar);

        //for (auto &idxi:*(shout.gp().as<IndexGuide>(1))) {
            //for(auto &nmt: shgd) {
                //int n,m;
                //guides::SHGuideBase::trig t;
                //std::tie(n,m,t)=nmt;
                //BOOST_TEST(shout[nmt][idxi] == n+1 );
                //BOOST_TEST(shoutrecover[nmt][idxi] == 1.0);

            //}
        //}



//}
