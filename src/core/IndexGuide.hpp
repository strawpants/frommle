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

#ifndef FROMMLE_INDEXGUIDE_HPP
#define FROMMLE_INDEXGUIDE_HPP

namespace frommle{
    namespace core{

        /*!brief
         * A guide which keeps track on (permuted) indices
         */
        class IndexGuide:public GuideBase{
        public:
            using pack=unsigned long long int;
            IndexGuide():GuideBase("IndexGuide"){}
            IndexGuide(const size_t nsize):GuideBase("IndexGuide",nsize){}
            pack operator[](const GuideBase::index idx)const{return idx;}
//            Element & operator[](GuideBase::index idx){return idx;}
        };


    }
}

#endif //FROMMLE_INDEXGUIDE_HPP
