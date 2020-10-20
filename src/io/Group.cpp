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
#include "io/Variable.hpp"
namespace frommle{
    namespace io{
        std::shared_ptr<Group> Group::getGroup(const std::string &name)const {
            auto idx=findidx(name);
            if (idx == -1){
                //create a new group
                THROWINPUTEXCEPTION("Group not found in archive group"+name);
            }
            return std::static_pointer_cast<Group>(this->operator[](name).ptr());
        }
       
        std::shared_ptr<Group> Group::createGroup(const std::string &name){
           auto idx=findidx(name);
           if (idx == -1){
               //create a new empty Group
                this->operator[](name)=Group(name);
           }//else return existing variable
           return std::static_pointer_cast<Group>(this->operator[](name).ptr());

       }

        std::shared_ptr<VariableBase> Group::getVariable(const std::string &name)const {
            auto idx=findidx(name);
            if (idx == -1){
                THROWINPUTEXCEPTION("Variable not found in archive group"+name);
                //create a new empty VariableDyn
//                this->operator[](name)=VariableBase();
            }
            return std::static_pointer_cast<VariableBase>(this->operator[](name).ptr());
        }


       std::shared_ptr<VariableBase> Group::createVariable(const std::string &name){
           auto idx=findidx(name);
           if (idx == -1){
               //create a new empty VariableBase
                this->operator[](name)=VariableBase(name);
           }//else return existing variable
           return std::static_pointer_cast<VariableBase>(this->operator[](name).ptr());

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



