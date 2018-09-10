/*! \file
 \brief Definition file for the TimeDimension class
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
#ifndef SRC_CPP_TIMEDIMENSION_HPP_
#define SRC_CPP_TIMEDIMENSION_HPP_
#include <chrono>
#include <vector>
#include "src/cpp/DimensionBase.hpp"


namespace frommle{

/*! \brief Holds a vector of time stamps (possibly irregular)
 * The TimeDimension is essentially a vector with time stamps. Internally it stores a vector of std::chrono::time_point
 * This can then be converted to different kinds of time formats
 */
class TimeDimension: public DimensionBase {
public:
	using timepoint=std::chrono::time_point<std::chrono::steady_clock>;
	TimeDimension(){}

	inline size_t size()const{return TimeVec_.size();}
private:
	std::vector<timepoint> TimeVec_={};
};

}

#endif /*SRC_CPP_TIMEDIMENSION_HPP_*/
