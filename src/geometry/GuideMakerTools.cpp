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

#include "geometry/GuideMakerTools.hpp"
#include "core/Logging.hpp"
#include "core/MacroMagic.hpp"
#include <cmath>
namespace frommle{

    namespace geometry{


        guides::OGRGuide<OGRPoint> makeFibonacciGrid(const size_t npoints){
            size_t np=npoints;
            if (npoints%2 == 0){
                LOGWARNING << "Adding an additional point to the Fibonacci grid";
                np+=1;
            }

            ptrdiff_t Nhalf=(np-1)/2;
            const double PHI= (1+sqrt(5.0))/2.0;

            guides::OGRGuide<OGRPoint> fibgrid{"PointGuide",core::typehash("FibGuide").add(np)};
            for(ptrdiff_t i=-Nhalf;i<Nhalf+1;++i){
                double lat=asin((2.0*i)/(2*Nhalf+1))/D2R;
                double lon=fmod(i,PHI)*(360/PHI);
                if (lon < -180){
                    lon+=360;
                }else if (lon > 180){
                    lon-=360;
                }
                fibgrid.push_back(OGRPoint(lon,lat));

            }
            return fibgrid;
        }

        ///@brief create a OGRPoint guide from two containers of longitude and latitude
        template<class Container>
        guides::OGRGuide<OGRPoint> makePointGuide(const Container & lon, const Container & lat){
            assert(lon.size()==lat.size());
            using GeoPoints=guides::OGRGuide<OGRPoint>;
            GeoPoints geopnts=GeoPoints("PointGuide");
            size_t sz=lon.size();
            for(int i=0;i<sz;++i){
                geopnts.push_back(OGRPoint(lon[i],lat[i]));
            }
            return geopnts;
        }



        template guides::OGRGuide<OGRPoint> makePointGuide<std::vector<double>>(const std::vector<double> &lon,const std::vector<double> & lat);

    }
}

