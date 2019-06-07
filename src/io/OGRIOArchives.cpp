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
#include "io/OGRArchive.hpp"
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

        void OGRGroup::loadCollection() {
            //currently we just load everything which is available
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

        void OGRGroup::parentHook() {
            setAmode();

            if (readable() and writable()){
                if (!readlayer()){
                    //try creating a new one
                    if(!createlayer()){
                        throw core::IOException("Cannot create layer");
                    }
                }
            }else if (readable()){
                if(!readlayer()){
                    throw core::IOException("Cannot read existing layer");
                }
            }else{
                if(!createlayer()){
                    throw core::IOException("Cannot create layer");
                }
            }
        }

        bool OGRGroup::readlayer() {
            layer_=static_cast<OGRArchive*>(getParent())->loadLayer(getName());
            if(layer_){
                layerdef_ = layer_->GetLayerDefn();
                if (!layerdef_){
                    throw core::IOException(" Cannot read layer definition");
                }
                return true;
            }else{
                return false;
            }
        }

        bool OGRGroup::createlayer() {
            layer_=static_cast<OGRArchive*>(getParent())->loadLayer(getName());
            layerdef_ = layer_->GetLayerDefn();
            if (!layerdef_){
                throw core::IOException(" Cannot read layer definition");
            }
            return true;
        }

        OGRGroup::OGRGroup(OGRLayer *const layer) :Group(layer->GetName()),layer_(layer){
            if (layer_) {
                layerdef_ = layer_->GetLayerDefn();
            }

        }

        int OGRGroup::createField(OGRFieldDefn *infield) {
            if(currentFeat){
                throw core::InputException("Cannot (re)create a GDAL field once a feature has been loaded");
            }
            layer_->CreateField(infield);
            return layerdef_->GetFieldIndex(infield->GetNameRef());
        }

        int OGRGroup::createField(OGRGeomFieldDefn *ingeofield) {
            if(currentFeat){
                throw core::InputException("Cannot (re)create a GDAL field once a feature has been loaded");
            }
            layer_->CreateGeomField(ingeofield);
            return layerdef_->GetGeomFieldIndex(ingeofield->GetNameRef());
        }

        const OGRFeature *OGRGroup::getFeature(const size_t idx)const {
            assert(!writable());

            //only read existing feature
            if(currentFeat) {
                //quick return if the feature is still the same
                if (currentFeat->GetFID() == idx){return currentFeat;}

                OGRFeature::DestroyFeature(currentFeat);
            }

            if(idx ==-1){
                return layer_->GetNextFeature();
            }else{
                return layer_->GetFeature(idx);
            }

        }


        OGRFeature *OGRGroup::getFeature(const size_t idx) {
           if(currentFeat){

               //quick return if the feature is still the same
               if (currentFeat->GetFID() == idx){return currentFeat;}

                if (writable()){
                    if( layer_->CreateFeature(currentFeat) != OGRERR_NONE){
                        throw core::IOException("Cannot create feature in GDAl data source");
                    }
                }
                OGRFeature::DestroyFeature(currentFeat);

            }else{
                if(writable()){
                    if (idx != -1){
                        throw core::InputException("Newly created features can currently only be appended");
                    }
                    //create new feature
                    currentFeat=OGRFeature::CreateFeature(layerdef_);
                }else{
                    //read existing feature
                    if(idx ==-1){
                        currentFeat=layer_->GetNextFeature();}
                    else{
                        currentFeat=layer_->GetFeature(idx);
                    }
                }

            }
            return currentFeat;
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

        OGRVar::singlePtr OGRVar::getValue(const size_t idx)const {

            if (!layer_) {
                throw core::InputException("OGR layer from parent has not been set");
            }
            OGRFeature* feat=ogrparent_->getFeature(idx);

            if (!feat){
                //nothing to iterate over enymore
                return singlePtr();
            }

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

        return val;
        }

        void OGRVar::setValue(singlePtr & ini, const size_t idx){
            if (!writable()){
                throw core::IOException("Archive not opened for writing");
            }
            if (idx != -1){
                throw core::IOException("GDAl datasource currently only supports appending");

            }

            auto feat=ogrparent_->getFeature(idx);

            auto type =getType();

            if (type == OFTInteger) {
                feat->SetField(fieldid_,boost::get<int>(*ini));
            }else if (type == OFTInteger64) {
                feat->SetField(fieldid_,boost::get<long long int>(*ini));
            }else if (type == OFTReal) {
                feat->SetField(fieldid_,boost::get<double>(*ini));
            }else if (type == OFTString) {
                feat->SetField(fieldid_,boost::get<std::string>(*ini).c_str());
            }else{
                throw core::MethodException("cannot cast  variant types to OGR type");
            }


        }

        void OGRVar::parentHook() {
            ogrparent_=static_cast<OGRGroup*>(getParent());
            layer_=ogrparent_->getLayer();

            if (writable()) {
                //also create a field entry in the OGRGroup above
                if (geomfielddef_) {
                    fieldid_=ogrparent_->createField(geomfielddef_.get());
                } else if (fieldef_) {
                    fieldid_=ogrparent_->createField(fieldef_.get());
                }
            }


        }
        //Note that in this case the shared-ptr is constructed as an alias and is not owned by the class
        OGRVar::OGRVar(OGRGeomFieldDefn *const geomfieldef, const int id)
                :Variable(geomfieldef->GetNameRef()),geomfielddef_(geoFieldPtr(geoFieldPtr(),geomfieldef)),fieldid_(id){
            if(getName().empty()){
                //force default name for geometry when not set
                setName("geom");
            }
        }

        ///fielddef is NOT owned by this class so we need to use the alias constructor of the shared_ptr
        OGRVar::OGRVar(OGRFieldDefn *const fieldef, const int id)
                :Variable(fieldef->GetNameRef()),fieldef_(fieldPtr(fieldPtr(),fieldef)), fieldid_(id){
        }

        template<class T, class F>
        OGRVarBase<T, F>::OGRVarBase(F *const fieldef, const int id):Variable<T>(fieldef->GetNameRef()),fieldef_(fieldPtr(fieldPtr(),fieldef)), fieldid_(id){
        }

        template<class T, class F>
        OGRVarBase<T, F>::OGRVarBase(const std::string &fieldName):Variable<T>(fieldName){
           //note: this class now owns fielddef_
           fieldef_=std::make_shared<F>(getName().c_str(),OGRtype<T>::type());

        }

        template<class T, class F>
        void OGRVarBase<T, F>::setValue(OGRVarBase<T,F>::singlePtr &in, const size_t idx){
            if (!layer_) {
                throw core::InputException("OGR layer from parent has not been set");
            }

            if (!writable()){
                throw core::IOException("Archive not opened for writing");
            }
            if (idx != -1){
                throw core::IOException("GDAl datasource currently only supports appending");

            }

            OGRtype<T>::valueToFeat(ogrparent_->getFeature(idx),*in,fieldid_);

        }


        template<class T, class F>
        typename OGRVarBase<T,F>::singlePtr OGRVarBase<T, F>::getValue(const size_t idx) const {
            if (!layer_) {
                throw core::InputException("OGR layer from parent has not been set");
            }
            OGRFeature* feat=ogrparent_->getFeature(idx);

            if (!feat){
                //nothing to iterate over anymore
                return singlePtr();
            }

            singlePtr val(new single());

            //load data in value
            *val=OGRtype<T>::valueFromFeat(feat,fieldid_);
            return val;

        }

        template<class T, class F>
        void OGRVarBase<T, F>::parentHook() {
            ogrparent_=static_cast<OGRGroup*>(getParent());
            layer_=ogrparent_->getLayer();

            if (writable()) {
            //also create a field entry in the OGRGroup above
                fieldid_=ogrparent_->createField(fieldef_.get());
            }
        }


        //explicit instantiation of various templates
        template class OGRVarBase<double>;
        template class OGRVarBase<int>;
        template class OGRVarBase<long long int>;
        template class OGRVarBase<std::string>;
        template class OGRVarBase<std::unique_ptr<OGRGeometry>,OGRGeomFieldDefn>;





    }



}

