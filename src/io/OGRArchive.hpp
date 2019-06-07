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
			OGRArchive():ArchiveBase(){}
			//default constructor opens a source location in the form of string (e.g. directory containing shapefiles)
			OGRArchive(const std::string &sourceName):ArchiveBase(sourceName) {
				init();
			};
			OGRArchive(const std::string sourceName,core::Attribs && attrib ):ArchiveBase(sourceName,std::move(attrib)) {
				init();
			};

			~OGRArchive();

			OGRSpatialReference *getOGRspatialRef();

//        	OGRArchive * getSelf()const { return this;}

		private:
			void init();
			void loadChildren();
			GDALDataset *poDS = nullptr;
		};

	}
}

#endif /* SRC_CORE_OGRARCHIVE_HPP_ */