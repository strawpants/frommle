/*! \file EarthConstants.hpp
 \brief 
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

 Created by Roelof Rietbroek,  2019

 */
#ifndef FROMMLE_EARTHCONSTANTS_HPP
#define FROMMLE_EARTHCONSTANTS_HPP
namespace frommle{
namespace earthmodels{

    //Define some constants

    //standard densities in kg/m^3
    static const double RHO_SEAWATER=1025.0;
    static const double RHO_WATER=1000.0;
    static const double RHO_EARTH=5517.0;
    static const double RHO_ICE=931.0;

    struct WGS84{
        //defining parameters
        static double a=6378137.0;//m
        static finv=289.257223563;//-
        static omegaRate=7292115e-11; //rad/s
        static double GM=3986004.418 108;//m^3/s^2
        //derived parameters

    };



}
}


#endif //FROMMLE_EARTHCONSTANTS_HPP
