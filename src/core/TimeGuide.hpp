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
#include <io/Group.hpp>

namespace frommle {
	namespace guides {
            
        using gregdate=boost::gregorian::date;
        using ptime=boost::posix_time::ptime;

/*! \brief Holds a vector of time stamps (possibly irregular)
 * The TimeDimension is essentially a vector with time stamps. Internally it stores a vector 
 * This can then be converted to different kinds of time formats
 */

        template<class Tp>
		class TimeGuide : public GuideBase {
		public:
		    using Element=Tp;
		    using ElementContainer=std::vector<Tp>;
			TimeGuide():GuideBase("time",typehash("TimeGuide")){}
            TimeGuide(std::string name):GuideBase(name,typehash("TimeGuide")){}
            TimeGuide(Tp && in):GuideBase("time",typehash("TimeGuide")){
                push_back(std::move(in));
            }

            TimeGuide(const Tp in):GuideBase("time",typehash("TimeGuide")){
                push_back(in);
            }

            TimeGuide(std::vector<Tp> && inv){
                TimeVec_=std::move(inv);
            }

            void push_back(const Tp in){
                TimeVec_.push_back(in);
            }

            void push_back(Tp && in){
                TimeVec_.push_back(in);
            }

            void load(io::Group & Ar)override;
            void save(io::Group & Ar)const override;

            using const_iterator=typename ElementContainer::const_iterator;
            using iterator=typename ElementContainer::iterator;
            const_iterator begin() const { return TimeVec_.cbegin(); }
            const_iterator end() const { return TimeVec_.cend(); }

            iterator begin() { return TimeVec_.begin(); }
            iterator end() { return TimeVec_.end(); }
            Element & operator[](const size_t idx){return TimeVec_.at(idx);}
            const Element & operator[](const size_t idx)const{return TimeVec_.at(idx);}
            size_t size()const override{return TimeVec_.size();}
		private:
			std::vector<Tp> TimeVec_ = {};
		};


        template <class Tp>
        void TimeGuide<Tp>::load(io::Group & Ar){

           //retrieve the variable which holds the time info
            auto &tvar=Ar.template getVariable<Tp>(name());
            for (auto t:tvar ){
                this->push_back(*t);
            }
        }

        template <class Tp>
        void TimeGuide<Tp>::save(io::Group & Ar)const{
//            //create a new variable holding the geometry
            Ar[name()]=io::Variable<Tp>();
            auto tvar= dynamic_cast<io::Variable<Tp>*>(Ar[name()].get());
            for (auto const & t:TimeVec_){
                tvar->setValue(t,-1);
            }

        }


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
        

    
	using DateGuide=TimeGuide<gregdate>;
	using PTimeGuide=TimeGuide<ptime>;
    
        //
        DateGuide monthlyRange(const int yearstart, const int monthstart,const int yearend,const int monthend);
        
    }

}
#endif /*SRC_CPP_TIMEDIMENSION_HPP_*/
