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
#include <boost/filesystem.hpp>
namespace frommle {
    namespace io {


        ///@brief Destructor needs to take care on closing the OGR datasource correctly
        OGRArchive::~OGRArchive() {
            if (size() >0 ){
                //delete children first
                deleteCollection();
            }
            if (poDS) {
                GDALClose(poDS);
                poDS = nullptr;
            }
        }


        void OGRArchive::init() {
            //first all OGR data formats must be dynamically initiated (this needs to be done only once per program call)
            //So we use a singleton for that which calls it's constructor only once
            GDALinit::get();


            GDALDriver* gdalDriver=nullptr;

            if (getAttributeCount("Driver") > 0) {
                auto driver = getAttribute<std::string>("Driver");
                if (driver == "PostGIS") {
                    //modify the input sourceName to refer to a valid POSTGIS resource
                    setName(GDALPOSTGISSource(name()));
                }
                gdalDriver = GetGDALDriverManager()->GetDriverByName(driver.c_str());
            }

            if(writable() and not readable()){
                //remove resource if it is a regular file or directory
                boost::filesystem::path path(name());
                if (boost::filesystem::is_directory(path) or boost::filesystem::is_regular_file(path)){
                    boost::filesystem::remove_all(path);
                }
            }

            if (writable() and  readable()) {
                auto gdalaccess = GDAL_OF_UPDATE | GDAL_OF_VECTOR;
                //try opening existing file
                poDS = static_cast<GDALDataset *>(GDALOpenEx(name().c_str(), gdalaccess, NULL, NULL, NULL));
            }else if (writable()){
                    if (!gdalDriver) {
                        throw core::InputException(
                                "A GDAL driver must be set explicitly when creating files. Set the 'Driver' attribute of the Archive to a valid GDAL driver");
                    }
                    poDS = gdalDriver->Create(name().c_str(), 0, 0, 0, GDT_Unknown, NULL);
            }else if (readable()){
                auto gdalaccess = GDAL_OF_READONLY | GDAL_OF_VECTOR;
                poDS = static_cast<GDALDataset *>(GDALOpenEx(name().c_str(), gdalaccess, NULL, NULL, NULL));
            }


            if (!poDS) {
                throw core::IOException("Error opening OGR source");
            }
        }


        ///@brief indexes all available layers
        void OGRArchive::loadCollection() {
            //We just load all available layers
            //@todo only load layer on demand
            if( size() != 0){
            //nothing to be done (do a quick return)
                return;
            }
            auto nlyrs = poDS->GetLayerCount();
            for (int i = 0; i < nlyrs; ++i) {
                upsertChild(i,OGRGroup(poDS->GetLayer(i)));
            }
        }

        OGRLayer *OGRArchive::createLayer(const std::string &layername,const OGRwkbGeometryType geotype)  {
            if(!poDS){
                throw core::IOException("No datasource open");
            }
            return poDS->CreateLayer(layername.c_str(),&poRef,geotype,NULL);

        }

        OGRLayer *OGRArchive::loadLayer(const std::string & layername) {
            if(!poDS){
                throw core::IOException("No datasource open");
            }
            return poDS->GetLayerByName(layername.c_str());
        }

        OGRSpatialReference *OGRArchive::getOGRspatialRef() {}
    }
}



