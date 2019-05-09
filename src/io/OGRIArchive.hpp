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

#ifndef SRC_CORE_OGRIARCHIVE_HPP_
#define SRC_CORE_OGRIARCHIVE_HPP_
namespace frommle {
	namespace io {

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




		//possibly parse options first before opening a file
			OGRIArchive(const std::string &sourceName, const ArchiveOpts &Opts) {
				//Process options and open files
				setOpts(Opts, sourceName);
			}

			~OGRIArchive();

			void listLayers();

//			using InputArchiveBase::operator>>;

			OGRSpatialReference *getOGRspatialRef();

			void changeGroup(const std::string & GroupName);

			//@brief nested iterator class
			class ogriterator:public geometry::OGRiteratorBase{
			public:
				ogriterator(){}
				~ogriterator(){
					freefeat();
				}
				ogriterator(const ogriterator & ogrother)=delete;
				ogriterator & operator=(const ogriterator & rhs)=delete;
				ogriterator(OGRIArchive * in):ArPtr_(in){
					//load the first geeometry by incrementing
					++*this;
				}
				ogriterator& operator++(){
					//destroy feature if not nullptr
					freefeat();
					//get next feature
					poFeat_ = ArPtr_->poLayer->GetNextFeature();
					if (poFeat_  != NULL) {
						//set pointer to OGRGeometry
						elVal = poFeat_->GetGeometryRef();
					}else{
						elVal= nullptr;
					}
					return *this;
				};
			private:
				void freefeat(){
					if (poFeat_) {
						OGRFeature::DestroyFeature(poFeat_);
					}
				}
				OGRIArchive * ArPtr_=nullptr;
				OGRFeature *poFeat_ = nullptr;
			};
			std::shared_ptr<geometry::OGRiteratorBase>  ogrbegin(){
				return std::shared_ptr<ogriterator>(new ogriterator(this));
			}

			std::shared_ptr<geometry::OGRiteratorBase>  ogrend()const{
				return std::shared_ptr<ogriterator>(new ogriterator());
			}

		private:
			void setOpts(const ArchiveOpts &Opts, const std::string sourceName);
			void checkValidity();
			GDALDataset *poDS = nullptr;
			OGRLayer *poLayer = nullptr;
			OGRFeatureDefn *poFDefn=nullptr;
		};

	}
}

#endif /* SRC_CORE_OGRIARCHIVE_HPP_ */