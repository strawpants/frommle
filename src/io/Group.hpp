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
#include <string>
#include <memory>
#include <cassert>
#include "core/TreeNode.hpp"
#include "core/Hyperslab.hpp"
#include "core/GuideBase.hpp"
//#include "core/VisitorTools.hpp"
#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP
//fowrward declare classes contained within the variant class below
class OGRGeometry;

namespace frommle{
    namespace io{
        class Group;


        //forward declare a variable here
        class VariableBase;

        template<class T>
        class Variable;

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class Group:public core::TreeNodeCollection {
        public:
            Group():TreeNodeCollection(){}
            //construct when only the groupname is known
            Group(const std::string & name ):TreeNodeCollection(name){}
//            Group(const std::string && name ):TreeNodeCollection(std::move(name)){}
            Group(const std::string name, core::Attributes && attr):TreeNodeCollection(name,std::move(attr)){}
            //construct when both the name and parent are known
            template<class P>
            Group(const std::string & name, const P * const parent ):TreeNodeCollection(name,parent){}
            Group(core::TreeNodeRef && in):TreeNodeCollection(std::move(in)){};

            //serialization goodies (objects can be read/written to groups by implementing a serialization function
            template<class Y>
            Group & operator >> (Y & out){
                out.load(*this);
                return *this;
            }

            template<class Y>
            Group & operator << (Y & out){
                out.save(*this);
                return *this;
            }

            template<class Y>
            Group & operator << (Y && out){
                out.save(*this);
                return *this;
            }

            bool readable()const;
            bool writable()const;
            virtual std::shared_ptr<Group>  getGroup(const std::string &name)const;
            
            virtual std::shared_ptr<Group>  createGroup(const std::string &name);

            virtual std::shared_ptr<VariableBase> getVariable(const std::string &name)const;

            virtual std::shared_ptr<VariableBase> createVariable(const std::string &name);

            template<class T>
            Variable<T> & getVariable(const std::string & name);


            void setAmode(std::string mode){
                attr().set("mode",mode);
                if (mode == "r"){
                    openForReading=true;
                    openForWriting=false;
                }else if( mode == "w"){
                    openForReading=false;
                    openForWriting=true;
                }else if(mode == "rw"){
                    openForReading=true;
                    openForWriting=true;
                }else{
                    THROWINPUTEXCEPTION("cannot understand the access mode of the group");
                }

            }
            void setAmode(){
                auto ogrgrp=static_cast<Group*>(getParent());
                if (ogrgrp){
                    //take the accessmode from above
                    ogrgrp->setAmode();
                    openForReading=ogrgrp->readable();
                    openForWriting=ogrgrp->writable();
                }else if (attr().getCount("mode") != 0) {
                    auto mode = attr().get<std::string>("mode");
                    if( mode == "r"){
                        openForReading=true;
                        openForWriting=false;
                    }else if (mode == "w"){
                        openForReading=false;
                        openForWriting=true;

                    }else if (mode == "rw"){
                        openForReading=true;
                        openForWriting=true;

                    }else{
                        throw core::InputException("cannot understand the access mode of this Group");
                    }
                }

            }
        protected:
            bool openForReading=true;
            bool openForWriting=false;
    private:
        };


    }

}



#endif //FROMMLE_GROUPBASE_HPP
