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
#include "boost/multi_array.hpp"
#include "core/VisitorTools.hpp"

namespace frommle{
    namespace io{


        NetCDFArchive::NetCDFArchive(const std::string source, core::Attributes &&attr) :ArchiveBase(source,std::move(attr)){
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
               NetCDFCheckerror(nc_create(name().c_str(), omode, &id_));

            }else {
                NetCDFCheckerror(nc_open(name().c_str(), omode, &id_));
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


        void NetCDFGroupBase::setAttr(const std::string &name, const std::string &value) {
            NetCDFCheckerror(nc_put_att_text(id_,NC_GLOBAL,name.c_str(),value.size(),value.c_str()));
        }


        void NetCDFBase::setParentid(core::TreeNodeBase *inParent) {
            auto test=static_cast<NetCDFArchive*>(inParent);
            if(test){
                //get the id from the
                ncparent_= static_cast<NetCDFGroupBase*>(test);
            }else{
                auto test=dynamic_cast<NetCDFGroup*>(inParent);
                if (!test){
                    THROWINPUTEXCEPTION("Parent type not related to NetCDFBase object");
                }
                ncparent_= static_cast<NetCDFGroupBase*>(test);
            }


        }
        using AcceptedTypes=core::typelist<double,size_t>;
        core::TreeNodeRef NetCDFGroupBase::convertChild(core::TreeNodeRef &&in) {
            //check if the input node is a group
            try{
                in.as<Group>();
                return core::TreeNodeRef(NetCDFGroup(std::move(in)));
            }catch(std::bad_cast &excep){
                //ok try different variable casts with different dimensions
                return tryVarCasts<NetCDFVariable,AcceptedTypes>()(std::move(in));
                //return tryVarCasts<NetCDFVariable,double,size_t>()(std::move(in));
//                return tryVarCasts<NetCDFVariable,double,int,long long int>()(std::move(in));

                    
            }

        }

        ///@brief get dimension by searching for the first with the same size
        int NetCDFGroupBase::getdimid(const size_t sz)const{
            //search the existing dimensions by size

            int ndims;


            auto status=nc_inq_ndims(id_,&ndims);

            if(status != NC_NOERR){
                //no dimensions found at all
                return -1;
            }

            size_t dimlen;
            for(int did=0;did<ndims;++did){
                int status=nc_inq_dimlen(id_,did,&dimlen);
                if(status == NC_NOERR) {
                    if (dimlen == sz) {
                        return did;
                    }
                }
            }
            //not found if we arrive here
            return -1;

        }

        ///@brief get a dimension by searching for the name
        int NetCDFGroupBase::getdimid(const std::string name)const{
            //search the existing dimensions by size
            int dimid=-1;
            int status=nc_inq_dimid(id_,name.c_str(),&dimid);
            if(status == NC_NOERR){
                return dimid;
            }

            return -1;

        }

        ///@brief create a new dimension
        int NetCDFGroupBase::setdimid(const size_t len,const std::string name){
            int dimid;
            NetCDFCheckerror(nc_def_dim	(id_,name.c_str(), len,&dimid));
            return dimid;
        }



        void NetCDFGroup::parentHook(){
            setParentid(getParent());

            if (readable() and ncparent_->id() != -1){
            //load group id from archive
                NetCDFCheckerror(nc_inq_grp_ncid (ncparent_->id(), name().c_str(),&id_));

            }

            if(writable()){
                //createGroup
                NetCDFCheckerror(nc_def_grp	(ncparent_->id(), name().c_str(),&id_));
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
