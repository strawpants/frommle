/*! \file NetCDFIO.cpp
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
#include "io/NetCDFIO.hpp"
#include <netcdf.h>
#include <core/UserSettings.hpp>
#include "core/Exceptions.hpp"
#include "NetCDFIO.hpp"
#include "core/UserSettings.hpp"
#include "io/CFConventions.hpp"
#include "boost/multi_array.hpp"

namespace frommle{
    namespace io{


        NetCDFArchive::NetCDFArchive(const std::string source, core::Attribs &&attr) :ArchiveBase(source,std::move(attr)){
            init();
        }

        void NetCDFArchive::init() {
            int omode=NC_NOWRITE;

            if (writable()){
                omode=NC_WRITE;
            }



            if(writable() and ! readable()){
                //create a new file
                omode=NC_CLOBBER|NC_NETCDF4;
               NetCDFCheckerror(nc_create(getName().c_str(), omode, &id_));

            }else {
                NetCDFCheckerror(nc_open(getName().c_str(), omode, &id_));
            }

            if(writable()){
                //write global attributes
                CFConventions::SetGlobalAttr(*this);
            }


        }

        NetCDFArchive::~NetCDFArchive() {
            nc_close(id_);


        }

        NetCDFArchive::NetCDFArchive(const std::string source) :ArchiveBase(source){
            init();
        }


        void NetCDFBase::setAttr(const std::string &name, const std::string &value) {
            NetCDFCheckerror(nc_put_att_text(id_,NC_GLOBAL,name.c_str(),value.size(),value.c_str()));
        }

        void NetCDFBase::setParentid(const core::TreeNodeBase *inParent) {
            auto test=static_cast<const NetCDFArchive*>(inParent);
            if(test){
                //get the id from the
                parentid_=test->id_;
            }else{
                auto test=dynamic_cast<const NetCDFGroup*>(inParent);
                if (!test){
                    THROWINPUTEXCEPTION("Parent type not related to NetCDFBAse object");
                }
                parentid_=test->id_;
            }


        }

        core::TreeNodeRef convertChildFree(core::TreeNodeRef &&in) {
            //check if the input node is a group
            try{
                in.as<Group>();
                return core::TreeNodeRef(NetCDFGroup(std::move(in)));
            }catch(std::bad_cast &excep){
                //ok try different variable casts with different dimensions
                return tryVarCasts<NetCDFVariable,double,int,long long int>()(std::move(in));
                
                    
            }

        }

        core::TreeNodeRef NetCDFArchive::convertChild(core::TreeNodeRef &&in) {
            return convertChildFree(std::move(in));
        }
        
        core::TreeNodeRef NetCDFGroup::convertChild(core::TreeNodeRef &&in) {
            return convertChildFree(std::move(in));
        }
        
        void NetCDFGroup::parentHook(){
            setParentid(getParent());
            if(writable()){
                //createGroup
                NetCDFCheckerror(nc_def_grp	(parentid_, getName().c_str(),&id_));
                CFConventions::SetGroupAttr(*this);
            }


        }


        ///@brief converts an error code into a frommle IO exception
        void NetCDFCheckerror(const int status) {
            if(status != NC_NOERR){
                THROWIOEXCEPTION(nc_strerror(status));
            }

        }



    }
}
