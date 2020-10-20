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

#include "geometry/GeoGrid.hpp"
#include "core/Exceptions.hpp"
#include <assert.h>
namespace frommle {
    namespace guides {

        GeoGrid::GeoGrid(double west, double east, double south, double north, double dlon, double dlat, const gridreg reg)
                : GuideBase(typehash("GeoGrid")) {
            /*The input may not be perfectly constistent so we will respect the resolution, but
             possibly snap the max values of the bounding box to appropriate values
             */
            if (east < west) {
                THROWINPUTEXCEPTION("Erronous input bounding box east < west");
            }

            if (north < south) {
                THROWINPUTEXCEPTION("Erronous input bounding box north < south");
            }


            reg_ = reg;
            dx_ = dlon;
            dy_ = dlat;
            nlon_ = (east - west) / dlon + reg_;
            nlat_ = (north - south) / dlat + reg_;
            bbox_.MinX = west;
            bbox_.MinY = south;
            bbox_.MaxX = bbox_.MinX + dx_ * (nlon_ - reg_);
            bbox_.MaxY = bbox_.MinY + dy_ * (nlat_ - reg_);

        }

        size_t GeoGrid::idx(const lint ilon, const lint ilat) const {
            //note that we index such that the latitude loops slowest
            return ilat * nlon_ + ilon;
        }

        std::tuple<long unsigned int, long unsigned int> GeoGrid::ilonilat(const size_t idx) const {
            lint ilat = idx / nlon_;
            return std::make_tuple(idx - ilat * nlon_, ilat);
        }

        std::tuple<double, double> GeoGrid::lonlat(const lint ilon, const lint ilat) const {
            if ( ilon >= nlon_ || ilat >= nlat_){
                THROWINPUTEXCEPTION("invalid longitude and latitude index");
            }
            return std::make_tuple(bbox_.MinX+dx_*(ilon+0.5*(1-reg_)),bbox_.MinY+dy_*(ilat+0.5*(1-reg_)));
        }



        GeoGrid::const_iterator::const_iterator(){
            ilat=-1;
            ilon=-1;
        }

        GeoGrid::const_iterator::const_iterator(const GeoGrid * in){
            gridPtr=in;
            ilon=0;
            ilat=0;

            //set first point
            double lon,lat;
            std::tie(lon,lat)=gridPtr->lonlat(ilon,ilat);
            currentpoint_=geometry::geopoint(lon,lat);
        }

        void GeoGrid::const_iterator::increment(){
            //assert -> why the heck are you incrementing a finished iterator?
            assert(ilon!=-1 and ilat !=-1);

            ++ilon;
            if (ilon >= (unsigned int) gridPtr->nlon_){
                ilon=0;
                ++ilat;

                if (gridPtr->nlat_ <= ilat){
                    //we reached the end of the iterator (reset to -1)
                    ilat=-1;
                    ilon=-1;
                    return;
                }
            }
            //create the new point
            double lon,lat;
            std::tie(lon,lat)=gridPtr->lonlat(ilon,ilat);
            currentpoint_=geometry::geopoint(lon,lat);

        }

    }
}
