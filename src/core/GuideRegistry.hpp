/*! \file
 \brief 
 \copyright Roelof Rietbroek 2019
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

#include <boost/variant.hpp>
#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include "sh/SHGuide.hpp"

#ifndef FROMMLE_GUIDEREGISTRY_HPP
#define FROMMLE_GUIDEREGISTRY_HPP
namespace frommle{
    namespace guides{

        template<class ... T>
        struct GuideTlist{
            static const int nguides=sizeof...(T);
            using Gvar=boost::variant<std::shared_ptr<T>...>;
        };


        using GuideRegistry=GuideTlist<IndexGuide,DateGuide,SHnmHalfGuide>;


     //some useful visitors to be applied to the boost variant of the registered guides
    class gvar_size:public boost::static_visitor<size_t>{
    public:
        template<class G>
        size_t operator()(const G & guide)const{
            return guide->size();
        }
    };


    }


}



#endif //FROMMLE_GUIDEREGISTRY_HPP
