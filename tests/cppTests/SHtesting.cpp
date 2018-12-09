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
#include <boost/test/included/unit_test.hpp>
#include "sh/Legendre_nm_naive.hpp"
#include <math.h>
#include <iomanip>
using namespace frommle::sh;


double P52(double theta) {
    double sc = sqrt(242550.0 / 860160);
    return sc * (2 * cos(theta) + cos(3 * theta) - 3 * cos(5 * theta));
}


BOOST_AUTO_TEST_CASE(assocLegendre,*boost::unit_test::tolerance(1e-11))
{
    const double d2r=std::atan(1.0)*4/180;
    Legendre_nm_naive Pnm(1000);

    double theta;
    int nsteps=180/0.25;
    double dt=180.0/nsteps;

    BOOST_TEST_MESSAGE("Testing associated Legendre polynomials against analytical P52 solution");
    for(int i=0;i<=nsteps+1;i++){
        theta=dt*i*d2r;
        auto pout=Pnm(cos(theta));
        BOOST_TEST(pout[5][2]==P52(theta));
    }

}

