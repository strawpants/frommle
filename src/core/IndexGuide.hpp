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
#ifndef FROMMLE_INDEXGUIDE_HPP
#define FROMMLE_INDEXGUIDE_HPP

namespace frommle{
    namespace core{

        /*!brief
         * A guide which keeps track on (permuted) indices
         */
        class IndexGuide:public GuideBase{
        public:
            using Element=unsigned long long int;
            IndexGuide():GuideBase("IndexGuide"){}
            IndexGuide(const size_t nsize):GuideBase("IndexGuide",nsize){}
            IndexGuide(const std::string & name, const size_t nsize):GuideBase(name,"IndexGuide",nsize){}
            Element operator[](const frommle::core::index idx)const{return idx;}
//            Element & operator[](GuideBase::index idx){return idx;}
            private:

            friend class io::serialize;
            template<class Archive>
            void load(Archive & Ar){
                std::string gname=name();
                if (gname.empty()){
                    gname=type();
                }
                auto & gvar=Ar.template getVariable<size_t>(gname);
                //set the size of the current index
                core::Hyperslab<size_t> hslab{};
                gvar.getValue(hslab);
                size_=hslab.data()[1]+1;

            }

            template<class Archive>
            void save(Archive & Ar)const{
                //create an empty variable holding the index range
                using rangear=std::array<size_t,2>;
                rangear irange{0,size()-1};
                std::string gname=name();
                if (gname.empty()){
                    gname=type();
                }
                Ar[gname]=io::Variable<size_t>(gname,{{"guidetype",type()}});

                Ar[gname].template as<io::Variable<size_t>>().setValue(core::Hyperslab<size_t>(irange));

            }
        };


    }
}

#endif //FROMMLE_INDEXGUIDE_HPP
