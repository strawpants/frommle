//
// Created by roelof on 16-5-19.
//

namespace frommle{
    namespace io{

///@brief returns the spatial reference of the current layer (group)
OGRSpatialReference *OGRGroup::getOGRspatialRef()const{

    if (!layer_) {
        throw core::IOException("No layer is opened");
    }
    return layer_->GetSpatialRef();
}

VarRef OGRGroup::at(const std::string &VarName) const {
    return VarRef(new OGRVar(VarName, this));
}

VarRef OGRGroup::at(const int nvar) const {
    return VarRef(new OGRVar(nvar, this));
}

OGRGroup &OGRGroup::operator++() {
    //get the next layer
    if(++id_ < layerlookup_.size()){
        loadlayer(id_);
    }else{
        id_=-1;
    }
    return *this;
}

void OGRGroup::loadlayer(const std::string &gname) {
    auto gid=layerlookup_[gname];
    loadlayer(gid);
}

void OGRGroup::loadlayer(const int gid) {
    layer_=static_cast<const OGRArchive *>(Arptr_)->poDS->GetLayer(gid);

    if (layer_) {
        layerdef_ = layer_->GetLayerDefn();
        layer_->ResetReading();
        //also update name and id
        name_ = layer_->GetName();
        id_ = gid;


    }else{
        throw core::IOException("invalid OGRLayer requested");
    }
}

void OGRGroup::indexlayers() {
    auto OGRptr=static_cast<const OGRArchive*>(Arptr_);
    auto nlyrs=OGRptr->poDS->GetLayerCount();
    OGRLayer * layer=nullptr;
    for(int i=0;i< nlyrs;++i ){
        layer=OGRptr->poDS->GetLayer(i);
        layerlookup_[layer->GetName()]=i;
    }



}

VarRef OGRGroup::geoVar() {return VarRef(new OGRVar(0,this,true));}


OGRVar::OGRVar(const Group *parent) :VarItem(parent){
    this->loadvar(0);


}

OGRVar::OGRVar(const std::string &varname, const Group *parent) : VarItem(varname, parent){
    loadvar(varname);
}

OGRVar::OGRVar(const int varid, const Group *parent, const bool geo) : VarItem(parent) {
    if (geo){
        loadgeom(varid);
    }else {
        loadvar(varid);
    }
}

OGRVar &OGRVar::operator++() {
    //get the next field
    if ( ++id_ < nField()+nGeom()) {
        loadvar(id_);
    }else{
        id_=-1;
    }
    return *this;
}

void OGRVar::loadvar(const int varid){
    if (varid < nField()){

        fielddef_ = static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef()->GetFieldDefn(varid);
        name_ = fielddef_->GetNameRef();
        id_ = varid;

        //also load attributes
        loadAttributes();
    }else if (varid < (nGeom()+nField())){
        loadgeom(varid-nField());
    }else{
        throw core::IOException("variable number is not supported");
    }



}

void OGRVar::loadvar(const std::string &varName) {
    auto featdef=static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef();
    int varid =featdef->GetFieldIndex(varName.c_str());
    if (varid!=-1) {
        loadvar(varid);
    }else{
        //try searching for a geometry field
        auto geomid=featdef->GetGeomFieldIndex(varName.c_str());
        if (geomid !=-1){
            loadgeom(geomid);
        }

    }
}

void OGRVar::loadgeom(const int ngeom){
    auto featdef=static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef();
    if (ngeom < nGeom()){
        geomfielddef_=featdef->GetGeomFieldDefn(ngeom);
        name_=geomfielddef_->GetNameRef();
        id_=ngeom+nField();
    }else{
        throw core::IOException(" No (geometry) layer found with this number");
    }

}

ValueRef OGRVar::at(const size_t nVal) const {
    return ValueRef(new OGRValue(nVal,this));
}

void OGRVar::loadAttributes() {
//			for (int iField = 0; iField <  nField(); iField++) {
//				OGRFieldDefn *poField = getFieldDef(iField);
//				boost::any tmp;
//				if (poField->GetType() == OFTInteger)
//					tmp = boost::any(poFeat->GetFieldAsInteger(iField));
//				else if (poField->GetType() == OFTInteger64)
//					tmp = boost::any(poFeat->GetFieldAsInteger64(iField));
//				else if (poField->GetType() == OFTReal)
//					tmp = boost::any(poFeat->GetFieldAsDouble(iField));
//				else if (poField->GetType() == OFTString)
//					tmp = boost::any(std::string(poFeat->GetFieldAsString(iField)));
//				else
//					tmp = boost::any(std::string(poFeat->GetFieldAsString(iField)));
//
//				attribs_[poField->GetNameRef()] = tmp;
//			}

}

OGRValue::OGRValue(const size_t i,const OGRVar *const parent):ValueItem(parent){
    layer=static_cast<const OGRGroup*>(parent_->parent())->getLayer();
    //Tables in postgis ,may not necessarily start with an internal 0 index so in that case we interpret i=0 here as "the first available feature"
    if (i ==0 ){
        feat=layer->GetNextFeature();
    }else{;
        feat=layer->GetFeature(i);
    }
    if(feat) {
        anyval_ = feat->GetGeometryRef();
        id_=i;
    }

}

OGRValue &OGRValue::operator++() {
    if(feat) {
        OGRFeature::DestroyFeature(feat);
    }
    feat=layer->GetNextFeature();

    if(feat) {
        anyval_ = feat->GetGeometryRef();
        ++id_;
    }else{
        id_=-1;
    }
    return *this;
}
    }

}
