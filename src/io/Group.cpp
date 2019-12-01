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

#include "io/Group.hpp"
namespace frommle{
    namespace io{
        Group &Group::getGroup(const std::string &name) {
            auto idx=findidx(name);
            if (idx == -1){
                //create a new group
                this->operator[](name)=Group();
            }
            return this->operator[](name).as<Group>();
        }

        std::shared_ptr<VariableDyn> Group::getVariable(const std::string &name) {
            auto idx=findidx(name);
            if (idx == -1){
                //create a new empty VariableDyn
                this->operator[](name)=VariableDyn();
            }
            return std::static_pointer_cast<VariableDyn>(this->operator[](name).ptr());
        }


        bool Group::readable() const {
            if (getParent()) {
                return static_cast<Group *>(getParent())->readable();
            }else{
                return openForReading;
            }
        }

        bool Group::writable() const {
            if (getParent()) {
                return static_cast<Group *>(getParent())->writable();
            }else{
                return openForWriting;
            }
        }


    }

}



