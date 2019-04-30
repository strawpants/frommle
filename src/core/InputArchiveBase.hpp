/*! \file
 \brief Holds the base for the inputArchive class
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

#ifndef SRC_CPP_INARCHIVEBASE_HPP_
#define SRC_CPP_INARCHIVEBASE_HPP_

#include<string>
#include <map>
#include <boost/any.hpp>

namespace frommle {
	namespace io {
		using ArchiveOpts=std::map<std::string,boost::any>;
/*!
 * \brief Abstract base class for an input archive
 */
		class InputArchiveBase {
		public:
			virtual ~InputArchiveBase() {
			}
			virtual void changeGroup(const std::string & GroupName){
				currentgroup_=GroupName;
				//reset current variable name
				currentvar_="";
			}
			virtual void changeVar(const std::string & VarName){
				currentvar_=VarName;
			}
		protected:
			std::string currentgroup_{};
			std::string currentvar_{};
		private:
		};

	}
}
#endif /* SRC_CPP_INARCHIVEBASE_HPP_*/
