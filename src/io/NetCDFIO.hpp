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
#include <netcdf.h>
#include <boost/multi_array.hpp>

#ifndef FROMMLE_NETCDFARCHIVE_HPP
#define FROMMLE_NETCDFARCHIVE_HPP

namespace frommle{
    namespace io{

        void NetCDFCheckerror(const int status);

        template<class T>
        struct NetCDFtype;

        template<>
        struct NetCDFtype<double>{
            static constexpr nc_type type(){return NC_DOUBLE;}
        };
        template<>
        struct NetCDFtype<boost::multi_array<double,2>>{
            static constexpr nc_type type(){return NC_DOUBLE;}
        };



        class NetCDFArchive;

        ///@brief class with member functions which can be imported into NC objects
        class NetCDFBase{
        public:
            void setAttr(const std::string& name ,const std::string &value);
        protected:
            void setParentid(const core::TreeNodeBase * in);
            int parentid_=-1;
            int id_=-1;
        };

        class NetCDFGroup:public Group, public NetCDFBase{
        public:
            NetCDFGroup() : Group() {}
            //NetCDFGroup(const std::string name):Group(name){}
            NetCDFGroup(core::TreeNodeRef && in){}
            core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
            int ncid()const;
        private:
        void parentHook();

        }; 

        template<class T>
        class NetCDFVariable:public Variable<T>,public NetCDFBase{
        public:
            NetCDFVariable()=default;
            NetCDFVariable(core::TreeNodeRef && in){}
            using Variable<T>::writable;
            using Variable<T>::getParent;
            using Variable<T>::getName;
            using Variable<T>::singlePtr;
            using Variable<T>::single;
//            virtual void getValue(singlePtr & in,const ptrdiff_t idx)const{}
//            virtual void setValue(const singlePtr & val,const ptrdiff_t idx){}
            virtual void setValue(const core::Hyperslab<T> & hslab);
            virtual void getValue(core::Hyperslab<T> & hslab);
            int ndim(){return 1;}
        private:
            void parentHook();
            //supports variables with up to 10 dimensions..
            std::array<int,10> dimids{};

        };


        ///@brief a class which helps in setting attributes according to the CFconventions
        class NetCDFArchive:public ArchiveBase, public NetCDFBase{
        public:
        NetCDFArchive(const std::string source, core::Attribs && attr);
        explicit NetCDFArchive(const std::string source);
        ~NetCDFArchive();
        core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
        private:

            void loadCollection(){};
            void init();
        };


        template<class T>
        void NetCDFVariable<T>::parentHook(){
            setParentid(getParent());

            if(writable()){
                //create variable definition (not the actual values)
                NetCDFCheckerror(nc_def_var(parentid_,getName().c_str(),NetCDFtype<T>::type, ndim(),   &dimids, &id_));
            }
        }

        template<class T>
        void NetCDFVariable<T>::setValue(const core::Hyperslab<T> &hslab) {



        }

        template<class T>
        void NetCDFVariable<T>::getValue(core::Hyperslab<T> &hslab) {}
    }

}

#endif //FROMMLE_NETCDFARCHIVE_HPP
