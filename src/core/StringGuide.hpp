/*! \file
 \brief 
 \copyright Roelof Rietbroek 2020
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

#include "core/IndexedGuide.hpp"
#include <regex>

#ifndef SRC_CORE_STRINGGUIDE_HPP_
#define SRC_CORE_STRINGGUIDE_HPP_
namespace frommle{

    namespace guides{

        class StringGuide:public IndexedGuide<std::string>{
            public:
            using Element=std::string;
            using IGd=IndexedGuide<Element>;

            StringGuide(std::string name="strg"):IGd(name){}  
            core::typehash hash()const override{return core::typehash("StringGuide_t");} 
            //@brief mask the string based on an extended regular expression
            void regexmask(std::string regex){
                std::regex rgx(regex, std::regex::extended);
                this->mask([rgx](const Element & strin)-> bool{
                            return std::regex_search(strin,rgx);
                        });

            }
        };


    }

}
#endif /* SRC_CORE_STRINGGUIDE_HPP_*/
