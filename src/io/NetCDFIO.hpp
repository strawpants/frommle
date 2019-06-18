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
#include <string>
#include "io/CFConventions.hpp"
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
        class NetCDFGroupBase;

        ///@brief class with member functions which can be imported into NC objects
        class NetCDFBase{
        public:
            inline int id()const{return id_;}
        protected:
            void setParentid(core::TreeNodeBase * in);
            int id_=-1;
            NetCDFGroupBase *ncparent_=nullptr;
        };

        class NetCDFGroupBase:public NetCDFBase{
        public:
            int getdimid(const size_t sz)const;
            int getdimid(const std::string name)const;
            int setdimid(const size_t len,const std::string name);
            core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
            void setAttr(const std::string& name ,const std::string &value);
        };

        class NetCDFGroup:public Group,public NetCDFGroupBase{
        public:
            NetCDFGroup() : Group() {}
            //NetCDFGroup(const std::string name):Group(name){}
            NetCDFGroup(core::TreeNodeRef && in){}
            int ncid()const;
            core::TreeNodeRef convertChild(core::TreeNodeRef &&in){return NetCDFGroupBase::convertChild(std::move(in));};
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
            using Variable<T>::getAttributeCount;
            using core::TreeNodeBase::getAttribute;
//            virtual void getValue(singlePtr & in,const ptrdiff_t idx)const{}
//            virtual void setValue(const singlePtr & val,const ptrdiff_t idx){}
            virtual void setValue(const core::Hyperslab<T> & hslab);
            virtual void getValue(core::Hyperslab<T> & hslab);
            int ndim(){return ndim_;}

            void setAttr(const std::string& name ,const std::string &value);

        private:
            void parentHook();
            void setUpVariable(const std::vector<size_t> &extents);
            size_t ndim_=1;
            //supports variables with up to 10 dimensions..
            std::vector<int> dimids{};

        };


        ///@brief a class which helps in setting attributes according to the CFconventions
        class NetCDFArchive:public ArchiveBase, public NetCDFGroupBase{
        public:
        NetCDFArchive(const std::string source, core::Attribs && attr);
        explicit NetCDFArchive(const std::string source);
        ~NetCDFArchive();
        core::TreeNodeRef convertChild(core::TreeNodeRef &&in){return NetCDFGroupBase::convertChild(std::move(in));};
        private:

            void loadCollection(){};
            void init();
        };


        template<class T>
        void NetCDFVariable<T>::parentHook(){
            setParentid(getParent());

        }

        template<class T>
        void NetCDFVariable<T>::setValue(const core::Hyperslab<T> &hslab) {
            assert(id_);
            setUpVariable(hslab.extents());
            NetCDFCheckerror(nc_put_var	(ncparent_->id(),id_, hslab.data()));


        }

        template<class T>
        void NetCDFVariable<T>::getValue(core::Hyperslab<T> &hslab) {}

        template<class T>
        void NetCDFVariable<T>::setUpVariable(const std::vector<size_t> & extents) {

            if(writable()){
                //dynamically set number of dimensions
                ndim_=extents.size();
                dimids=std::vector<int>(ndim_,-1);

                //create/reuse dimensions from parent
                for(int i=0;i< ndim_;++i){
                    std::string fallback(getName());
                    if (getAttributeCount("Dimensions") == 0){
                        if (ndim_ != 1){
                            //append a dimension id to name as the variable for the 1D case
                            fallback+=std::to_string(i);
                        }

                        //tryto find a dimension which ahs the same size
                        dimids[i]=ncparent_->getdimid(extents[i]);

                    }else{
                        fallback=core::TreeNodeBase::template getAttribute< std::vector<std::string> >("Dimensions").at(i);
                        //try to find a dimension with the same name
                        dimids[i]= ncparent_->getdimid(fallback);
                    }

                    if(dimids[i] == -1){
                        //not found so we would like to create a new dimension
                        dimids[i]=ncparent_->setdimid(extents[i],fallback);

                    }


                }

                // After setting up /finding the dimensions we need to register the variable definition
                NetCDFCheckerror(nc_def_var(ncparent_->id(),getName().c_str(),NetCDFtype<T>::type(), ndim_,  dimids.data(), &id_));
                //also setup attributes
                CFConventions::SetDataAttr(*this);

            }


        }
        template<class T>
        void NetCDFVariable<T>::setAttr(const std::string &name, const std::string &value) {
            NetCDFCheckerror(nc_put_att_text(ncparent_->id(),id_,name.c_str(),value.size(),value.c_str()));
        }
    }

}

#endif //FROMMLE_NETCDFARCHIVE_HPP
