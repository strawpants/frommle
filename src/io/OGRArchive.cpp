/*!@file
@brief contains the implementation for an archive reading OGR geometries from shapefiles, POSTGIS databases etc
@copyright Roelof Rietbroek 2019
@license
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
#include <io/OGRArchive.hpp>
#include "core/Logging.hpp"
#include "OGRArchive.hpp"
#include <cstring>
#include <gdal.h>
namespace frommle {
    namespace io {


        ///@brief Destructor needs to take care on closing the OGR datasource correctly
        OGRArchive::~OGRArchive() {
            if (poDS) {
                GDALClose(poDS);
                poDS = nullptr;
            }
        }


        void OGRArchive::init() {


            if (getAttributeCount("Driver") > 0) {
                auto driver = getAttribute<std::string>("Driver");
                if (driver == "PostGIS") {
                    //modify the input sourceName to refer to a valid POSTGIS resource
                    setName(GDALPOSTGISSource(getName()));
                }
            }

            //first all OGR data formats must be dynamically initiated (this needs to be done only once per program call)
            //So we use a singleton for that which calls it's constructor only once
            GDALinit::get();
            auto gdalaccess = GDAL_OF_READONLY | GDAL_OF_VECTOR;

            if (writable() and  readable()){
                gdalaccess= GDAL_OF_UPDATE | GDAL_OF_VECTOR;
            }





            //open the ogr source (e.g. a directory containing shapefiles)
            poDS = static_cast<GDALDataset *>(GDALOpenEx(getName().c_str(), gdalaccess, NULL, NULL, NULL));
            if (!poDS) {
                throw core::IOException("Error opening OGR source");
            }
            //load all available layers
            loadChildren();
        }


        ///@brief indexes all available layers
        void OGRArchive::loadChildren() {
            if( size() != 0){
            //nothing to be done (do a quick return)
                return;
            }
            auto nlyrs = poDS->GetLayerCount();
            for (int i = 0; i < nlyrs; ++i) {
                upsertChild(i,OGRGroup(poDS->GetLayer(i)));
            }
        }
    }
}



