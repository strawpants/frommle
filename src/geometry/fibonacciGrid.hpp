/*! \file create a fibonacci grid based on González, Á., 2009. Measurement of Areas on a Sphere Using Fibonacci and Latitude–Longitude Lattices. Math Geosci 42, 49. https://doi.org/10.1007/s11004-009-9257-x

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
 */

#include "geometry/OGRGuide.hpp"
#ifndef FROMMLE_FIBONACCIGRID_HPP
#define FROMMLE_FIBONACCIGRID_HPP

namespace frommle{

    namespace guides{

        OGRGuide<OGRPoint> makeFibonacciGrid(const size_t npoints);


    }
}

#endif //FROMMLE_FIBONACCIGRID_HPP
