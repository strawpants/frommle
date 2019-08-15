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
            if(writable() and !readable()){
                return;
            }
            auto nfields=layerdef_->GetFieldCount();

            for(int i=0 ;i<nfields;++i){
                upsertChild(i,OGRVarfactory(layerdef_,i,false));
            }
            //also load the geometry fields
            auto ngeom=layerdef_->GetGeomFieldCount();
            for(int i=0 ;i<ngeom;++i){
                upsertChild(i+nfields,OGRVarfactory(layerdef_,i,true));
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
            layer_=static_cast<OGRArchive*>(getParent())->createLayer(getName(),wkbUnknown);
            layerdef_ = layer_->GetLayerDefn();
            if (!layerdef_){
                throw core::IOException(" Cannot read layer definition");
            }
            return true;
        }

        OGRGroup::OGRGroup(OGRLayer *const layer) :Group(std::string(layer->GetName())),layer_(layer){
            if (layer_) {
                layerdef_ = layer_->GetLayerDefn();
            }

        }

        int OGRGroup::getField(OGRFieldDefn *infield) {
            if(currentFeat and writable()){
                throw core::InputException("Cannot (re)create a GDAL field once a feature has been loaded");
            }
            auto id=layerdef_->GetFieldIndex(infield->GetNameRef());
            if (id ==-1 and not writable()){
                throw core::InputException("cannot find requested OGR field");

            }
            if(id ==-1) {
                layer_->CreateField(infield);
                return layerdef_->GetFieldIndex(infield->GetNameRef());
            }else{
                return id;
            }
        }

        int OGRGroup::getField(OGRGeomFieldDefn *ingeofield) {
            if (currentFeat and writable()) {
                throw core::InputException("Cannot (re)create a GDAL field once a feature has been loaded");
            }
            auto id = layerdef_->GetGeomFieldIndex(ingeofield->GetNameRef());
            if( id ==-1 and not writable()){
                throw core::InputException("cannot find requested OGR field");
            }

            if (id == -1 ) {
                if (layer_->TestCapability(OLCCreateGeomField)) {
                    layer_->CreateGeomField(ingeofield);
                }

                return layerdef_->GetGeomFieldIndex(ingeofield->GetNameRef());
            }else {
                //possibly modify field
                if(writable()) {
                    layerdef_->SetGeomType(ingeofield->GetType());
                }
                 return id;
            }
        }

        const OGRFeature *OGRGroup::getFeature(const ptrdiff_t idx)const {
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


        OGRFeature *OGRGroup::getFeature(const ptrdiff_t idx) {
           if(currentFeat){

               //quick return if the feature is still the same
               if (idx != -1 and currentFeat->GetFID() == idx){return currentFeat;}

                if (writable()){
                    if( layer_->CreateFeature(currentFeat) != OGRERR_NONE){
                        throw core::IOException("Cannot set feature in GDAl data source");
                    }
                }
//                if(writable()) {
//                    layer_->SetFeature(currentFeat);
//                }
                    OGRFeature::DestroyFeature(currentFeat);
           }

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
                else if (idx ==0){
                    //special case where we reset the features to start from the beginning
                    layer_->ResetReading();
                    currentFeat=layer_->GetNextFeature();
                    currentFeat->SetFID(0);
                }else{
                    currentFeat=layer_->GetFeature(idx);

                }
           }


            return currentFeat;
        }

        //template<class F, class ...Ts>
        //struct try_casts {
            //core::TreeNodeRef operator()(core::TreeNodeRef &&in) {
                //auto tmp=dynamic_cast<Variable<F> *>(in.get());
                //if(tmp){
                   ////yeah, success let's proceed by returning a converted type
                    //return core::TreeNodeRef(OGRVarBase<F>(std::move(in)));
                //} else{
                    ////no success try the next type
                    //return try_casts<Ts...>()(std::move(in));
                //}

            //}
        //};

        //template<class F>
        //struct try_casts<F> {
            //core::TreeNodeRef operator()(core::TreeNodeRef &&in) {
                //auto tmp=dynamic_cast<Variable<F> *>(in.get());
                //if (tmp){
                    ////yeah, success let's proceed by returning a converted type
                    //return core::TreeNodeRef(OGRVarBase<F>(std::move(in)));
                //}else{
                    ////no success  and nothing left to try
                    //throw core::InputException("No more casting possibilities for OGRVarBase");
                //}

            //}
        //};


        core::TreeNodeRef OGRGroup::convertChild(core::TreeNodeRef &&in) {
            //return try_casts<double,int,long long int, std::string, OGRPolygon, OGRPoint, OGRGeometry >()(std::move(in));
            return tryVarCasts<OGRVarBase,double,int,long long int, std::string, OGRPolygon, OGRPoint, OGRGeometry >()(std::move(in));
        }

        OGRGroup::~OGRGroup() {
            if(currentFeat and layer_ and writable()){
                //write the last feature if it is still opened
                auto geom=currentFeat->GetGeometryRef();
                char ** wkt=new char*;
                geom->exportToWkt(wkt);
                layer_->CreateFeature(currentFeat);
                OGRFeature::DestroyFeature(currentFeat);
                layer_->SyncToDisk();
            }
        }


        std::string GDALPOSTGISSource(const std::string PGname, const std::string schemas) {
            using us=core::UserSettings;
            std::string pgname_=PGname;
            if (PGname.empty()){
                //search for a default postgis database entry
                pgname_=us::at("Defaultdb").as<std::string>();
            }
            auto username=us::at(pgname_)["user"].as<std::string>();

            std::string source = std::string("PG:dbname='") + us::at(pgname_)["db"].as<std::string>() +
                                 "' host='" + us::at(pgname_)["host"].as<std::string>() +
                                 "' port='" + us::at(pgname_)["port"].as<std::string>() +
                                 "' user='" + username +
                                 "' password='" + us::getAuth("geoslurp",username) + "'";
            if(!schemas.empty()){
                source+= "schemas="+schemas;
            }
            return source;
        }

            core::TreeNodeRef OGRVarfactory(OGRFeatureDefn * featdef,const size_t id,const bool isgeo){
                if(isgeo){

                    OGRGeomFieldDefn * fielddef=featdef->GetGeomFieldDefn(id);
                    auto type= fielddef->GetType();
                    switch (type){
                        case (wkbUnknown):
                            return core::TreeNodeRef(OGRVarBase<OGRGeometry>(fielddef,id));
                            break;
                        case(wkbPolygon):
                            return core::TreeNodeRef(OGRVarBase<OGRPolygon>(fielddef,id));
                            break;
                        default:
                            throw core::InputException("Cannot handle this OGRwkbGeometryType");
                    }


                }else{
                    OGRFieldDefn * fielddef=featdef->GetFieldDefn(id);
                    auto type = fielddef->GetType();
                    switch (type){
                        case (OFTInteger):
                            return core::TreeNodeRef(OGRVarBase<int>(fielddef,id));
                            break;
                        case (OFTInteger64):
                            return core::TreeNodeRef(OGRVarBase<long long int>(fielddef,id));
                            break;
                        case (OFTReal):
                            return core::TreeNodeRef(OGRVarBase<double>(fielddef,id));
                            break;
                        case (OFTString):
                            return core::TreeNodeRef(OGRVarBase<std::string>(fielddef,id));
                            break;

                        default:
                            throw core::InputException("Cannot handle this OGRfieldType");
                    }

                }

            }

        

        template<class T>
        OGRVarBase<T>::OGRVarBase(fieldtype  *const fieldef, const int id):Variable<T>(std::string(fieldef->GetNameRef())),fieldef_(fieldPtr(fieldPtr(),fieldef)), fieldid_(id){
        }

        template<class T>
        OGRVarBase<T>::OGRVarBase(const std::string &fieldName):Variable<T>(fieldName){
           //note: this class now owns fielddef_
           fieldef_=std::make_shared<fieldtype>(getName().c_str(),OGRtype<T>::type());

        }

        template<class T>
        OGRVarBase<T>::OGRVarBase(core::TreeNodeRef &&in):Variable<T>(std::move(in)){
            //note: this class now owns fielddef_
            fieldef_=std::make_shared<fieldtype>(getName().c_str(),OGRtype<T>::type());

        }

        template<class T>
        void OGRVarBase<T>::setValue( const singlePtr & in, const ptrdiff_t idx){
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


        template<class T>
        void OGRVarBase<T>::getValue( singlePtr & val,const ptrdiff_t idx) const {
            if (!layer_) {
                throw core::InputException("OGR layer from parent has not been set");
            }
            OGRFeature* feat=ogrparent_->getFeature(idx);

            if (!feat){
                //nothing to iterate over anymore
                val=singlePtr();
                return;
            }

            //load data in value
            OGRtype<T>::valueFromFeat(feat,*val,fieldid_);

        }

        template<class T>
        void OGRVarBase<T>::parentHook() {
            ogrparent_=static_cast<OGRGroup*>(getParent());
            layer_=ogrparent_->getLayer();

            //also create a field entry in the OGRGroup above
           fieldid_=ogrparent_->getField(fieldef_.get());
        }



        //explicit instantiation of various templates
        template class OGRVarBase<double>;
        template class OGRVarBase<int>;
        template class OGRVarBase<long long int>;
        template class OGRVarBase<std::string>;
        template class OGRVarBase<OGRGeometry>;
        template class OGRVarBase<OGRPolygon>;
        template class OGRVarBase<OGRPoint>;





    }



}

