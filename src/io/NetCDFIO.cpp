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
                omode=NC_CLOBBER;
               NetCDFCheckerror(nc_create(getName().c_str(), omode, &ncid));

            }else {
                NetCDFCheckerror(nc_open(getName().c_str(), omode, &ncid));
            }

            if(writable()){
                //write global attributes
                CFConventions::SetGlobalAttr(*this);
            }


        }

        NetCDFArchive::~NetCDFArchive() {
            nc_close(ncid);


        }

        NetCDFArchive::NetCDFArchive(const std::string source) :ArchiveBase(source){
            init();
        }

        void NetCDFArchive::setAttr(const std::string &name, const std::string &value) {
            nc_put_att_text(ncid,NC_GLOBAL,name.c_str(),value.size(),value.c_str());
        }

        core::TreeNodeRef NetCDFArchive::convertChild(core::TreeNodeRef &&in) {
//            return core::TreeNodeRef(NetCDFGroup(std::move(in)));
        }

        ///@brief converts an error code into a frommle IO exception
        void NetCDFCheckerror(const int status) {
            if(status != NC_NOERR){
                THROWIOEXCEPTION(nc_strerror(status));
            }

        }



    }
}