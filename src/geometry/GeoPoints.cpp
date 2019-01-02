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

#include "geometry/GeoPoints.hpp"
#include "assert.h"

namespace frommle{
    namespace geometry{

        GeoPoints::GeoPoints() : GuideBase("GeoPoints") {

        }

        void GeoPoints::push_back(geopoint &&pnt) {
            points_.push_back(std::move(pnt));
            ++size_;
        }

        void GeoPoints::push_back(const geopoint &pnt) {
            points_.push_back(geopoint(pnt));
            ++size_;
        }

        void GeoPoints::push_back(const double lon, const double lat) {
            points_.push_back(geopoint(lon, lat));
            ++size_;
        }

        GeoPoints::const_iterator::const_iterator(const GeoPoints::const_iterator &in) {
            currentpoint_ = in.currentpoint_;
            idx = in.idx;
            GeoPointPtr = in.GeoPointPtr;
        }

        GeoPoints::const_iterator::const_iterator(const GeoPoints *in) {
            GeoPointPtr=in;
            idx=0;
        }

        void GeoPoints::const_iterator::increment() {
            assert(idx != -1);
            ++idx;
            if (idx >= GeoPointPtr->size_){
                idx=-1;
            }
        }
    }
}
