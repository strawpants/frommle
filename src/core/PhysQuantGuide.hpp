/*! \file
 \brief 
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
#include "core/GuideBase.hpp"
#include "io/Group.hpp"
#include <boost/range.hpp>
#include "core/PhysQuantityBase.hpp"

#ifndef FROMMLE_PHYSQUANTGUIDE_HPP
#define FROMMLE_PHYSQUANTGUIDE_HPP

namespace frommle{
    namespace guides{

        /*!brief
         * A guide which keeps track on (permuted) indices
         */
    class PhysQuantGuide:public GuideBase, public InjectGuideIterator<PhysQuantGuide,core::PhysQuantity*>{
        public:
            using Element=core::PhysQuantity*;

            PhysQuantGuide():GuideBase("PhysQuantGuide",typehash("PhysQuantGuide").add(0)){}
            Element operator[](const size_t idx)const{return state_[idx].get();}
            size_t idx(const Element & el)const {
                for(int i=0; i<state_.size();++i){
                    if ( *el == *state_ ){
                        return i;
                    }
                }

                THROWINDEXEXCEPTION("Cannot find element in PhysQuantGuide");
            }
            using const_iterator=InjectGuideIterator<PhysQuantGuide,Element>::const_iterator;
            using InjectGuideIterator<PhysQuantGuide,Element>::begin;
            using InjectGuideIterator<PhysQuantGuide,Element>::end;
            size_t size()const override{return state_.size();}
        private:
            std::vector<std::shared_ptr<PhysQuantity>> state_{};
        };


    }
}

#endif //FROMMLE_PHYSQUANTGUIDE_HPP
