/*! \file
 \brief 
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
 */

#include <core/GArrayBase.hpp>
#include "core/GuideBase.hpp"
#include "core/GOperatorBase.hpp"
#include "sh/SHGuide.hpp"
#include "core/Exceptions.hpp"
#include "core/MacroMagic.hpp"
#include "sh/Legendre_nm.hpp"
#ifndef FROMMLE_SHANALYSIS_HPP
#define FROMMLE_SHANALYSIS_HPP

namespace frommle{
    namespace sh{
        template<class spatialG,class SHG>
        class SHanalysis:public GOperator<spatialG,SHG>{
        public:
            SHanalysis(spatialG && spatg, SHG && shg ){
                GOperator(std::move(spatg),std::move(shg));
            }
            frommle::core::Garray<spatialG> operator() (const frommle::core::Garray<SHG> & in ){
                if( g<1>() != in.g<0>()){
                    throw frommle::core::GuideMatchingException(HERE+"SH dimension");
                }

//                construct an object to be filled
                frommle::core::Garray<spatialG> out(g<0>());

                //loop over locations (latitude loops slowest to make best use of associated Legendre polynomials)
                double lon,lat;
                double latold=1000*D2R; //set to out of range value on purpose

                Legendre_nm_d Pnm(g<1>().nmax()));
                std::vector<double> pnmvals;
                for ( auto & loc:g<0>().geoms()){
                    std::tie(lon,lat)=loc;
                    if (latold != lat){
                        pnmvals=Pnm(std::sin(lat*D2R));
                    }

                    //Multiply  spherical harmonics with coefficients
//                    out[]


                }
            }

        };

    }
}


#endif //FROMMLE_SHANALYSIS_HPP
