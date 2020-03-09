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
#include "io/Variable.hpp"

#include <boost/range.hpp>
#ifndef FROMMLE_INDEXGUIDE_HPP
#define FROMMLE_INDEXGUIDE_HPP

namespace frommle{
    namespace guides{

        /*!brief
         * A guide which keeps track on indices (future extension will be to allow for permutations)
         */
        class IndexGuide:public GuideBase, public InjectGuideIterator<IndexGuide,unsigned long long int>{
        public:
            using Element=unsigned long long int;

            IndexGuide():GuideBase("IndexGuide",typehash("IndexGuide").add(0).add(0)){}
            IndexGuide(const size_t nsize):GuideBase("IndexGuide",typehash("IndexGuide").add(0).add(nsize)),size_(nsize){}
            IndexGuide(const std::string & name, const size_t nsize):GuideBase(name,typehash("IndexGuide").add(0).add(nsize)),size_(nsize){}
            Element operator[](const size_t idx)const{return idx;}
            using const_iterator=InjectGuideIterator<IndexGuide,Element>::const_iterator;
            using InjectGuideIterator<IndexGuide,Element>::begin;
            using InjectGuideIterator<IndexGuide,Element>::end;
            size_t idx(const Element & el)const {return el;}
            void append(){
                ++size_;
            }
            void load(io::Group & Ar){
                auto gvar=Ar.getVariable(name());
                auto spl=gvar->hash().split();
                size_=std::stoi(spl[0]);

            }

            void save(io::Group & Ar)const{
//                //create an empty variable holding the index range
//                using rangear=std::array<size_t,2>;
//                rangear irange{0,size()-1};
//                std::string gname=name();
//                if (gname.empty()){
//                    gname=std::string(hash());
//                }
//                Ar[gname]=io::Variable<size_t>(gname,{{"guidetype",std::string(hash())}});
//
//                Ar[gname].template as<io::Variable<size_t>>().setValue(core::Hyperslab<size_t>(irange));

            }
            size_t size()const override{return size_;}
        private:
            size_t size_=0;
        };


    }
}

#endif //FROMMLE_INDEXGUIDE_HPP
