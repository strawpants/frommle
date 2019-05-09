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
#include "OGRIArchive.hpp"
#include <cstring>
#include <gdal.h>
namespace frommle {
	namespace io {
		///@brief Destructor needs to take care on closing the OGR datasource correctly
		OGRIArchive::~OGRIArchive() {
			if (poDS) {
				GDALClose(poDS);
				poDS = nullptr;
			}
		}


		void OGRIArchive::setOpts(const ArchiveOpts &Opts, const std::string sourceName) {

			std::string source_=sourceName;
			const char ** driver=NULL;

			if (Opts.count("Driver") >0 ){
//				auto driver2=boost::any_cast<char *>(&Opts["Driver"]);
				auto gdaldriver=Opts.at("Driver");
				driver = boost::any_cast<const char *>(&gdaldriver);
			}
			if (driver) {
				if (strcmp(*driver, "PostGIS") == 0) {
					source_ = GDALPOSTGISSource(sourceName);
				}
			}

			//first all OGR data formats must be dynamically initiated (this needs to be done only once per program call)
			GDALinit::get();

			//open the ogr source (e.g. a directory containing shapefiles)
			poDS = static_cast<GDALDataset *>(GDALOpenEx(source_.c_str(), GDAL_OF_VECTOR,NULL,NULL,NULL));
			if (!poDS) {
				throw core::IOException("Error opening OGR source");
			}
		}

/////@brief opens up a OGR layer by name
//		void OGRIArchive::changeGroup(const std::string &GroupName){
//			checkValidity();
//			InputArchiveBase::changeGroup(GroupName);
//			poLayer = poDS->GetLayerByName(GroupName.c_str());
//			poFDefn = poLayer->GetLayerDefn();
//			poLayer->ResetReading();
//		}

/////@brief loads the next feature in the layer (note that the variable name is irrelevant)
//		void OGRIArchive::changeVar(const std::string &VarName){
//			InputArchiveBase::changeVar(VarName);
//
////			poFeat = poLayer->GetNextFeature();
//			//also load the attribute field descriptions
////			poFDefn = poLayer->GetLayerDefn();
//		}

//		void OGRIArchive::listLayers() {
//		    checkValidity();
//			OGRLayer * layer;
//		    for(int i=0;i<poDS->GetLayerCount();++i){
//		        layer=poDS->GetLayer(i);
//				LOGINFO << "Layer:  "<< layer->GetName() << " type: " << OGRGeometryTypeToName(layer->GetGeomType()) << std::endl;
//		    }
//		}

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



        GroupRef OGRIArchive::at(const std::string &groupName)const {
			OGRGroup * grptr = new OGRGroup(groupName,this);
			grptr->loadlayer(poDS->GetLayerByName(groupName.c_str()));
            return GroupRef(new GroupBase());
		}

		GroupRef OGRIArchive::at(const int nGroup)const {
			OGRGroup * grptr = new OGRGroup(this);
			if (nGroup > poDS->GetLayerCount()){
				throw core::IOException("OGRsource does not have layer with this number");
			}
			//Now we load the layer in the
			grptr->loadlayer(poDS->GetLayer(nGroup),nGroup);

			return GroupRef(grptr);
		}

		void OGRGroup::loadlayer( OGRLayer *const lyr, const int gid) {
			if (lyr) {
				layer_ = lyr;
				layerdef_ = layer_->GetLayerDefn();
				layer_->ResetReading();
				//also update name and id
				gname_ = layer_->GetName();
				if (gid >= 0) {
					gid_ = gid;
				}
			}else{
				throw core::IOException("invalid OGRlayer requested");
			}
		}


		///@brief returns the spatial reference of the current layer (group)
		OGRSpatialReference *OGRGroup::getOGRspatialRef()const{

			if (!layer_) {
				throw core::IOException("No layer is opened");
			}
			return layer_->GetSpatialRef();
		}

	}
}




