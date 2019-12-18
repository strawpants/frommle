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
#include "core/typehash.hpp"
#include <memory>
#ifndef FROMMLE_FROMMLE_HPP
#define FROMMLE_FROMMLE_HPP


namespace frommle{
    namespace io {
        //we only need to forward declare Group here
        class Group;
    }

    namespace core{
        ///@brief Polymorphic base class which wraps all important frommle classes
    class Frommle:public std::enable_shared_from_this<Frommle> {
        public:
            Frommle(){};
            Frommle(std::string name,typehash th=typehash("Frommle_t")):name_(name),type_(th){}
            std::string name()const{return name_;}
            void setName(std::string name){name_=name;}


            virtual void save(io::Group & grp)const{
                THROWNOTIMPLEMENTED("This object cannot (yet) be saved to an archive");
            }
            virtual void load(io::Group & group){
                THROWNOTIMPLEMENTED("This object cannot (yet) be loaded from an archive");
            }
            //add 1D index_range and index_gen types here?
            virtual typehash hash() const { return type_; }
            std::string hashstr()const{return std::string(hash());}

            template<class T=const Frommle>
            std::shared_ptr<T> getPtr()const{
                return std::static_pointer_cast<T>(shared_from_this());

            }

            template<class T=Frommle>
            std::shared_ptr<T> getPtr(){
                return std::static_pointer_cast<T>(shared_from_this());

            }
        protected:
            std::string name_="Frommle";
            typehash type_{"Frommle_t"};
        };

    }
}

#endif //FROMMLE_FROMMLE_HPP
