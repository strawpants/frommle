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
#include "io/ArchiveBase.hpp"
#include "io/OGRIOArchives.hpp"
//#include "core/Logging.hpp"
#include <ogrsf_frmts.h>
#include <memory>
#include "geometry/OGRiteratorBase.hpp"
#include "core/UserSettings.hpp"

#ifndef SRC_CORE_OGRARCHIVE_HPP_
#define SRC_CORE_OGRARCHIVE_HPP_




namespace frommle {
	namespace io {

	class OGRArchive : public ArchiveBase {
		public:
			OGRArchive():ArchiveBase(){

			}
			OGRArchive(const std::string name,core::Attribs && attr):ArchiveBase(name,std::move(attr)){
				init();
			}

			~OGRArchive();

			OGRSpatialReference *getOGRspatialRef();;

			OGRLayer* loadLayer(const std::string & layername);
			OGRLayer * createLayer(const std::string & layername, const OGRwkbGeometryType geotype=wkbUnknown);

//			virtual core::TreeNodeRef & operator[](const std::string & name){
//				return upsertChild(name,OGRGroup());
//			}
//			virtual core::TreeNodeRef & operator[](const size_t & idx){
//				return upsertChild(idx,OGRGroup());
//			}

			core::TreeNodeRef convertChild(core::TreeNodeRef &&in){
				return core::TreeNodeRef(OGRGroup(std::move(in)));
			}

		private:
			void init();
			void loadCollection();
			GDALDataset *poDS = nullptr;
			//I want to initialize the spatialreference to that of an 4326 SRID,
			// but since the importfromEPSG function is a member which does not
			// return an OGRSpatialreference instance I need to wrap it in a
			// (lambda) function in the initializer list
			OGRSpatialReference poRef{
				[]()->OGRSpatialReference{
				auto ref=OGRSpatialReference();
				ref.importFromEPSG(4326);
				return ref;}()
			};
		};

	}
}

#endif /* SRC_CORE_OGRARCHIVE_HPP_ */