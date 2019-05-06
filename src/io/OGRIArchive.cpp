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
#include <io/OGRIArchive.hpp>
#include "core/Logging.hpp"
#include <gdal.h>
namespace frommle {
	namespace io {
		///@brief Destructor needs to take care on closing the OGR datasource correctly
		OGRIArchive::~OGRIArchive() {
			GDALClose(poDS);
//			delete poDS;
			poDS = nullptr;
		}


		void OGRIArchive::setOpts(const ArchiveOpts &Opts, const std::string sourceName) {
			//first all OGR data formats must be dynamically initiated (this needs to be done only once per program call)
			GDALinit::get();

			//open the ogr source (e.g. a directory containing shapefiles)
			poDS = (GDALDataset *) GDALOpenEx(sourceName.c_str(), GDAL_OF_VECTOR,NULL,NULL,NULL);
			if (!poDS) {
				throw core::IOException("Error opening OGR source");
			}
			//default opens the first layer
			poLayer = poDS->GetLayer(0);
			poFDefn = poLayer->GetLayerDefn();


		}

///@brief opens up a OGR layer by name
		void OGRIArchive::changeGroup(const std::string &GroupName){
			InputArchiveBase::changeGroup(GroupName);
			poLayer = poDS->GetLayerByName(GroupName.c_str());
			poFDefn = poLayer->GetLayerDefn();
		}

/////@brief loads the next feature in the layer (note that the variable name is irrelevant)
//		void OGRIArchive::changeVar(const std::string &VarName){
//			InputArchiveBase::changeVar(VarName);
//
////			poFeat = poLayer->GetNextFeature();
//			//also load the attribute field descriptions
////			poFDefn = poLayer->GetLayerDefn();
//		}

		void OGRIArchive::listLayers() {
			for( auto&& layer: poDS->GetLayers() )
			{
				LOGINFO << "Layer:  "<< layer->GetName() << " type: " << OGRGeometryTypeToName(layer->GetGeomType()) << std::endl;
			}

		}

//		///@brief reads in a attributmap from the current feature
//		OGRIArchive &OGRIArchive::operator>>(frommle::AttribsMap &out) {
//			if (!poFeat) {
//				//reads in the next feature if not already doen so
//				varHook();
//			}
//			for (int iField = 0; iField < poFDefn->GetFieldCount(); iField++) {
//				OGRFieldDefn *poField = poFDefn->GetFieldDefn(iField);
//				boost::any tmp;
//				if (poField->GetType() == OFTInteger)
//					tmp = boost::any(poFeat->GetFieldAsInteger(iField));
//				else if (poField->GetType() == OFTInteger64)
//					tmp = boost::any(poFeat->GetFieldAsInteger64(iField));
//				else if (poField->GetType() == OFTReal)
//					tmp = boost::any(poFeat->GetFieldAsDouble(iField));
//				else if (poField->GetType() == OFTString)
//					tmp = boost::any(std::string(poFeat->GetFieldAsString(iField)));
//				else
//					tmp = boost::any(std::string(poFeat->GetFieldAsString(iField)));
//
//				out[poField->GetNameRef()] = tmp;
//			}
//		}
//
//		///@brief returns a reference to the current geometry object
//		OGRGeometry *OGRIArchive::getOGRGeometry() {
//			if (!poFeat) {
//				varHook();
//			}
//			OGRGeometry *tmp = nullptr;
//			if (poFeat) {
//				tmp = poFeat->GetGeometryRef();
//			}
//
//			return tmp;
//		}

		///@brief returns the spatial reference of the current layer (group)
		OGRSpatialReference *OGRIArchive::getOGRspatialRef() {
			if (poLayer) {
				return poLayer->GetSpatialRef();
			} else {
				throw core::IOException("Cannot get spatial reference as layer is inactive");
			}
		}
	}
}




