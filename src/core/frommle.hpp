/*! \file frommle.hpp
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

 Created by Roelof Rietbroek,  2019

 */

#include <string>
#include "core/Exceptions.hpp"
#ifndef FROMMLE_FROMMLE_HPP
#define FROMMLE_FROMMLE_HPP


namespace frommle{
    namespace io {
        //we only need to forward declare Group here
        class Group;
        //and for the python bindings the wrapper version
//        class GroupWrapper;
    }

    namespace core{
        ///@brief Polymorphic base class which wraps all important frommle classes
        class Frommle{
        public:
            Frommle(){};
            Frommle(std::string name):name_(name){}
            std::string name()const{return name_;}
            void setName(std::string name){name_=name;}


            virtual void save(io::Group & grp)const{
                THROWNOTIMPLEMENTED("This object cannot be saved to an archive");
            }
            virtual void load(io::Group & group){
                THROWNOTIMPLEMENTED("This object cannot be loaded from an archive");
            }
            std::string name_="Frommle";
        };

    }
}

#endif //FROMMLE_FROMMLE_HPP
