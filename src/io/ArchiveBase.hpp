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
 * \brief Abstract base class for an output archive (i.e. a top level group)
 */

class ArchiveBase:public Group {
public:
	ArchiveBase() : Group() {}

	explicit ArchiveBase(const std::string &name) : Group(name) {
	}
	ArchiveBase(std::string name, std::string mode):Group(name){
	    attr().set("mode",mode);
	    setAmode(mode);
	}
	ArchiveBase(std::string name, core::Attributes &&attrib) : Group(name, std::move(attrib)) {
		setAmode();
	}


private:
};


}
}

#endif /* SRC_IO_ARCHIVEBASE_HPP_*/
