/*! \file
 \brief Holds  an input archive to load OGR geometries (uses GDAL/OGR in the background)
 \copyright Roelof Rietbroek 20189
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

#include "core/Exceptions.hpp"
#include "core/InputArchiveBase.hpp"
# include "core/Singleton.hpp"
//#include "core/Logging.hpp"
#include <ogrsf_frmts.h>
#include <memory>
#include "geometry/OGRiteratorBase.hpp"
#include "core/UserSettings.hpp"

#ifndef SRC_CORE_OGRIARCHIVE_HPP_
#define SRC_CORE_OGRIARCHIVE_HPP_




namespace frommle {
	namespace io {

		class OGRGroup;


		std::string GDALPOSTGISSource(const std::string PGname="",const std::string schemas="") {
			using us=core::UserSettings;
			std::string pgname_=PGname;
			if (PGname.empty()){
				//search for a default postgis database entry
				pgname_=us::at("Defaultdb").as<std::string>();
			}
			std::string source = std::string("PG:dbname='") + us::at(pgname_)["db"].as<std::string>() +
								 "' host='" + us::at(pgname_)["host"].as<std::string>() +
								 "' port='" + us::at(pgname_)["port"].as<std::string>() +
								 "' user='" + us::at(pgname_)["user"].as<std::string>() +
								 "' password='" + us::getAuth(pgname_) + "'";
			if(!schemas.empty()){
				source+= "schemas="+schemas;
			}
			return source;
		}

		///@brief small singleton class which calls GDAL initialization routines (needs to be called only once)
	class GDALinit : public core::Singleton<GDALinit> {
		private:
			friend class core::Singleton<GDALinit>;

			GDALinit() {
				GDALAllRegister();
//				OGRRegisterAll();
			}

		};

	class OGRIArchive : public InputArchiveBase {
		public:
			OGRIArchive(){}
			//default constructor opens a source location in the form of string (e.g. directory containing shapefiles)
			OGRIArchive(const std::string &sourceName) {
				setOpts(ArchiveOpts(), sourceName);
			};
			OGRIArchive(const ArchiveOpts & Opts) {
				setOpts(Opts,"");
			}

			OGRIArchive(const std::string sourceName, const ArchiveOpts & Opts) {
				setOpts(Opts,sourceName);
			}



		//possibly parse options first before opening a file
			OGRIArchive(const std::string &sourceName, const ArchiveOpts &Opts) {
				//Process options and open files
				setOpts(Opts, sourceName);
			}

			~OGRIArchive();

//			using InputArchiveBase::operator>>;

			OGRSpatialReference *getOGRspatialRef();

//			//@brief nested iterator class
//			class ogriterator:public geometry::OGRiteratorBase{
//			public:
//				ogriterator(){}
//				~ogriterator(){
//					freefeat();
//				}
//				ogriterator(const ogriterator & ogrother)=delete;
//				ogriterator & operator=(const ogriterator & rhs)=delete;
//				ogriterator(OGRIArchive * in):ArPtr_(in){
//					//load the first geeometry by incrementing
//					++*this;
//				}
//				ogriterator& operator++(){
//					//destroy feature if not nullptr
//					freefeat();
//					//get next feature
//					poFeat_ = ArPtr_->poLayer->GetNextFeature();
//					if (poFeat_  != NULL) {
//						//set pointer to OGRGeometry
//						elVal = poFeat_->GetGeometryRef();
//					}else{
//						elVal= nullptr;
//					}
//					return *this;
//				};
//			private:
//				void freefeat(){
//					if (poFeat_) {
//						OGRFeature::DestroyFeature(poFeat_);
//					}
//				}
//				OGRIArchive * ArPtr_=nullptr;
//				OGRFeature *poFeat_ = nullptr;
//			};
//			std::shared_ptr<geometry::OGRiteratorBase>  ogrbegin(){
//				return std::shared_ptr<ogriterator>(new ogriterator(this));
//			}
//
//			std::shared_ptr<geometry::OGRiteratorBase>  ogrend()const{
//				return std::shared_ptr<ogriterator>(new ogriterator());
//			}

		private:
			friend OGRGroup;
			GroupRef  at(const std::string & groupName)const;
			GroupRef  at(const int nGroup)const;

			void setOpts(const ArchiveOpts &Opts, const std::string sourceName);
			GDALDataset *poDS = nullptr;
		};

		class OGRGroup:public GroupBase{
		public:
			OGRGroup(const std::string & gname, const OGRIArchive *  const inAr):GroupBase(gname,inAr){}
			OGRGroup(const OGRIArchive * const inAr):GroupBase(inAr){

			}
			//@brief load the next layer
			OGRGroup & operator++(){
				auto lyr=static_cast<const OGRIArchive*>(Arptr_)->poDS->GetLayer(++gid_);
				if (lyr){
					loadlayer(lyr);
				}else{
					//reset layer to end
					gid_=-1;
				}
				return *this;
			}
			OGRSpatialReference * getOGRspatialRef()const;
		private:
			friend OGRIArchive;
			void loadlayer( OGRLayer * const lyr,const int gid=-1);
			OGRLayer *layer_ = nullptr;
			OGRFeatureDefn *layerdef_=nullptr;
		};
	}
}

#endif /* SRC_CORE_OGRIARCHIVE_HPP_ */