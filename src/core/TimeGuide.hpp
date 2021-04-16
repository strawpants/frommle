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
#include "core/IndexedGuide.hpp"
#include <boost/date_time.hpp>
#include "io/Group.hpp"
#include "io/Variable.hpp"

namespace frommle {
	namespace guides {
            
        using gregdate=boost::gregorian::date;
        using ptime=boost::posix_time::ptime;

/*! \brief Holds a guide of time stamps (possibly irregular)
*/

        template<class Tp>
        class TimeGuide:public IndexedGuide<Tp>{
            public:
                using IGd=IndexedGuide<Tp>;
                using Element=Tp;
                using IGd::name;
                using IGd::shared_from_this;
                TimeGuide(std::string name="time"):IGd(name){}
                core::typehash hash()const override{return core::typehash("TimeGuide_t");}
                //using IndexedGuide<Tp>::load;
                //void load(const std::shared_ptr<core::Frommle> &frptr)override;

        };



        //template <class Tp>
        //void TimeGuide<Tp>::load(io::Group & Ar){

           ////retrieve the variable which holds the time info
            //auto &tvar=Ar.template createVariable<Tp>(name());
            //for (auto t:tvar ){
                //this->push_back(*t);
            //}
        //}

        //template<class Tp>
        //void TimeGuide<Tp>::load(const std::shared_ptr<core::Frommle> &frptr){
            //core::loadFromFrommlePtr<TimeGuide<Tp>>(frptr,shared_from_this());
        //}

    //template <class Tp>
        //void TimeGuide<Tp>::save(io::Group & Ar)const{
////            //create a new variable holding the geometry
            //Ar[name()]=io::Variable<Tp>();
            //auto tvar= dynamic_cast<io::Variable<Tp>*>(Ar[name()].get());
            //for (auto const & t:*this){
                //tvar->setValue(t,-1);
            //}

        //}


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
            auto tg = TimeGuide<T>();
            auto t=from;
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
