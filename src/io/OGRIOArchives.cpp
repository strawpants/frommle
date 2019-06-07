/*! \file
 \brief Holds the implementation for OGR groups and variable items
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
 */
#include "io/OGRIOArchives.hpp"
#include "core/Exceptions.hpp"

namespace frommle{
    namespace io{

///@brief returns the spatial reference of the current layer (group)
OGRSpatialReference *OGRGroup::getOGRspatialRef()const{

    if (!layer_) {
        throw core::IOException("No layer is opened");
    }
    return layer_->GetSpatialRef();
}

        void OGRGroup::loadChildren() {

            if (size() != 0){
                //quick return when already loaded
                return;
            }
            auto nfields=layerdef_->GetFieldCount();

            for(int i=0 ;i<nfields;++i){
                upsertChild(i,OGRVar(layerdef_->GetFieldDefn(i),i));
            }
            //also load the geometry fields
            auto ngeom=layerdef_->GetGeomFieldCount();
            for(int i=0 ;i<ngeom;++i){
                upsertChild(i+nfields,OGRVar(layerdef_->GetGeomFieldDefn(i),i));
            }

        }


        std::string GDALPOSTGISSource(const std::string PGname, const std::string schemas) {
            using us=core::UserSettings;
            std::string pgname_=PGname;
            if (PGname.empty()){
                //search for a default postgis database entry
                pgname_=us::at("Defaultdb").as<std::string>();
            }
            std::string source = std::string("PG:dbname='") + us::at(pgname_)["db"].as<std::string>() +
                                 "' host='" + us::at(pgname_)["host"].as<std::string>() +
                                 "' port='" + us::at(pgname_)["port"].as<std::string>() +
                                 "' user='" + us::at(pgname_)["user"].as<std::string>() +
                                 "' password='" + us::getAuth(pgname_) + "'";
            if(!schemas.empty()){
                source+= "schemas="+schemas;
            }
            return source;
        }

        OGRVar::singlePtr OGRVar::getValue()const {

            if (!layer_){
                throw core::InputException("OGR layer from parent has not been set");
            }
            OGRFeature * feat=layer_->GetNextFeature();

            singlePtr val(new single());

            //load data in variant
            if (isGeom()){
                *val=std::unique_ptr<OGRGeometry>(feat->StealGeometry());
            }else{
                auto type=getType();
                auto iField=getFieldId();
				if (type == OFTInteger) {
                    *val = feat->GetFieldAsInteger(iField);
                }else if (type == OFTInteger64) {
                    *val = feat->GetFieldAsInteger64(iField);
                }else if (type == OFTReal) {
                    *val = feat->GetFieldAsDouble(iField);
                }else if (type == OFTString) {
				    *val=std::string(feat->GetFieldAsString(iField));
				}else{
				    throw core::MethodException("cannot cast OGR type to registered values");
				}
            }

            OGRFeature::DestroyFeature(feat);
        return val;
        }

        void OGRVar::setValue(singlePtr & in){
            if (!writable()){
                throw core::IOException("Archive not opened for writing");
            }

        }



    }



}

