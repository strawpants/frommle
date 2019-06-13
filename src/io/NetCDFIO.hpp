/*! \file NetCDFIO.hpp
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

#include "io/ArchiveBase.hpp"

#ifndef FROMMLE_NETCDFARCHIVE_HPP
#define FROMMLE_NETCDFARCHIVE_HPP

namespace frommle{
    namespace io{

        void NetCDFCheckerror(const int status);

        class NetCDFGroup:public Group{
        public:
            NetCDFGroup() : Group() {}
            //NetCDFGroup(const std::string name):Group(name){}
            NetCDFGroup(core::TreeNodeRef && in){}
            core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
        private:
        void parentHook();

        }; 

        template<class T>
        class NetCDFVariable:public Variable<T>{
        public:
            NetCDFVariable()=default;
            NetCDFVariable(core::TreeNodeRef && in){}
            using Variable<T>::writable;
        private:
            void parentHook();

        };


        ///@brief a class which helps in setting attributes according to the CFconventions
        class NetCDFArchive:public ArchiveBase{
        public:
        NetCDFArchive(const std::string source, core::Attribs && attr);
        explicit NetCDFArchive(const std::string source);
        ~NetCDFArchive();
        void setAttr(const std::string& name ,const std::string &value);
        core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
        private:
            void loadCollection(){};
            void init();
            int ncid=0;
        };


        template<class T>
        void NetCDFVariable<T>::parentHook(){
            if(writable()){
                //create variable definition (not the actual values)
            }
        }
    }

}

#endif //FROMMLE_NETCDFARCHIVE_HPP
