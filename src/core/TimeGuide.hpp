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
#include "core/GuideBase.hpp"
#include <boost/date_time.hpp>


namespace frommle {
	namespace core {
		using time_point=boost::posix_time::ptime;


/*! \brief Holds a vector of time stamps (possibly irregular)
 * The TimeDimension is essentially a vector with time stamps. Internally it stores a vector of std::chrono::time_point
 * This can then be converted to different kinds of time formats
 */

        template<class Tp>
		class TimeGuide : public GuideBase {
		public:
			TimeGuide():GuideBase("TimeGuide"){}

            TimeGuide(Tp && in):GuideBase("TimeGuide"){
                push_back(std::move(in));
            }

            TimeGuide(const Tp in):GuideBase("TimeGuide"){
                push_back(in);
            }

            TimeGuide(std::vector<Tp> && inv){
                TimeVec_=std::move(inv);
            }

            void push_back(const Tp in){
                TimeVec_.push_back(in);
                ++size_;
            }

            void push_back(Tp && in){
                TimeVec_.push_back(in);
                ++size_;
            }
		private:
			std::vector<Tp> TimeVec_ = {};
		};


        /*!brief
         * Create a Time dimension from a date/time range
         * @tparam T
         * @tparam D
         * @param from
         * @param to
         * @param step
         * @return
         */
        template<class T,class D>
        TimeGuide<T> make_trange(const T from, const T to,const D step){
            auto tg = TimeGuide<T>(from);
            auto t=from+step;
            while (t < to){
                tg.push_back(t);
                t+=step;
            }
            return tg;
        }

    }

}
#endif /*SRC_CPP_TIMEDIMENSION_HPP_*/
