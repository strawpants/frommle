/*! \file
 \brief Holds the base for the Dimension class
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

#ifndef SRC_IO_ARCHIVEBASE_HPP_
#define SRC_IO_ARCHIVEBASE_HPP_

#include<string>
#include "io/Group.hpp"

namespace frommle {
namespace io {

/*!
 * \brief Abstract base class for an output archive
 */

		class ArchiveBase {
		public:
			virtual ~ArchiveBase() = default;

			GrpIterator begin()const{return GrpIterator(this);}
			GrpIterator end()const{return GrpIterator();}


			//Functions which allow to navigate through the groups of an Archive
			GrpRef operator[](const std::string & Groupname)const{return this->at(Groupname);}
			GrpRef operator[](const int nGroup)const{return this->at(nGroup);}

		protected:
			virtual unsigned int file_version(){return 0;};

			virtual GrpRef at(const std::string & groupname)const=0;
			virtual GrpRef at(const int nGroup)const=0;

		private:
		};

}
}

#endif /* SRC_IO_ARCHIVEBASE_HPP_*/
