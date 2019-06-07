/*! \file
 \brief Holds header info relevant to both in and output OGR archives
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

#include "core/Singleton.hpp"
#include "core/UserSettings.hpp"
#include <ogrsf_frmts.h>
#include <string>
//#include "geometry/OGRiteratorBase.hpp"
#include "io/Group.hpp"
#ifndef FROMMLE_OGRIOARCHIVES_HPP
#define FROMMLE_OGRIOARCHIVES_HPP
namespace frommle{
    namespace io{

        std::string GDALPOSTGISSource(const std::string PGname="",const std::string schemas="");

        ///@brief small singleton class which calls GDAL initialization routines (needs to be called only once)
        class GDALinit : public core::Singleton<GDALinit> {
        private:
            friend class core::Singleton<GDALinit>;

            GDALinit() {
                GDALAllRegister();
//				OGRRegisterAll();
            }

        };

///@brief an OGRGroup points to a certain layer in an GDAL data source
    class OGRGroup:public Group{
        public:
            ~OGRGroup(){}
            OGRGroup(OGRLayer * const layer):Group(layer->GetName()),layer_(layer){
                if (layer_) {
                    layerdef_ = layer_->GetLayerDefn();
                }

            }
            OGRSpatialReference * getOGRspatialRef()const;
//            OGRFeatureDefn * getOGRFeatDef()const{return layerdef_;}


            OGRLayer* getLayer()const{return layer_;}
        private:
            virtual void loadChildren();
            void parentHook(){
                setAmode();
            }
            OGRLayer *layer_ = nullptr;
            OGRFeatureDefn *layerdef_=nullptr;
        };


        class OGRVar:public Variable{
        public:
            ~OGRVar(){}
            OGRVar(OGRFieldDefn * const fieldef,const int id):Variable(fieldef->GetNameRef()),fieldef_(fieldef), fieldid_(id){
            }
            OGRVar(OGRGeomFieldDefn * const geomfieldef, const int id):Variable(geomfieldef->GetNameRef()),geomfielddef_(geomfieldef),fieldid_(id){
                if(getName().empty()){
                    //force default name for geometry when not set
                    setName("geom");
                }
            }
            bool isGeom()const{return bool(geomfielddef_);}
            OGRFieldType getType()const{return fieldef_->GetType();}
            int getFieldId()const{return fieldid_;}
            singlePtr getValue()const;
            void setValue(singlePtr & in);
        private:
            void parentHook(){
                auto parent=static_cast<OGRGroup*>(getParent());
                layer_=parent->getLayer();
            }

            OGRFieldDefn * fieldef_=nullptr;
            OGRGeomFieldDefn * geomfielddef_=nullptr;
            OGRLayer* layer_=nullptr; //copy of the relevant layer pointer
//            OGRFeature* feat=nullptr;
            int fieldid_=-1;
        };
    }
}
#endif //FROMMLE_OGRIOARCHIVES_HPP
