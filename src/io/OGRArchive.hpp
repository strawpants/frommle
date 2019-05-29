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
#include "io/VarItem.hpp"
#include "io/ValueItem.hpp"
#include "io/OGRIOArchives.hpp"
//#include "core/Logging.hpp"
#include <ogrsf_frmts.h>
#include <memory>
#include "geometry/OGRiteratorBase.hpp"
#include "core/UserSettings.hpp"

#ifndef SRC_CORE_OGRIARCHIVE_HPP_
#define SRC_CORE_OGRIARCHIVE_HPP_




namespace frommle {
	namespace io {

	class OGRArchive : public ArchiveBase {
		public:
			OGRArchive(){}
			//default constructor opens a source location in the form of string (e.g. directory containing shapefiles)
			OGRArchive(const std::string &sourceName) {
				setOpts(ArchiveOpts(), sourceName);
			};
			OGRArchive(const ArchiveOpts & Opts) {
				setOpts(Opts,"");
			}


		//possibly parse options first before opening a file
			OGRArchive(const std::string &sourceName, const ArchiveOpts &Opts) {
				//Process options and open files
				setOpts(Opts, sourceName);
			}

			~OGRArchive();

//			using InputArchiveBase::operator>>;

			OGRSpatialReference *getOGRspatialRef();

		private:
			friend OGRGroup;
			GrpRef  at(const std::string & groupName)const;
			GrpRef  at(const int nGroup)const;

			void setOpts(const ArchiveOpts &Opts, const std::string sourceName);
			GDALDataset *poDS = nullptr;
		};

	}
}

#endif /* SRC_CORE_OGRIARCHIVE_HPP_ */