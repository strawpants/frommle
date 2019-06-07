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

        template<class T>
        struct OGRtype;

        template<>
        struct OGRtype<std::string>{
            static constexpr OGRFieldType type(){return OFTString; }
            static constexpr bool isGeo(){return false;}
            static std::string valueFromFeat(OGRFeature * feat, const int iField){
                return std::string(feat->GetFieldAsString(iField));
            }
            static void valueToFeat(OGRFeature * feat, std::string & val, const int iField){
                feat->SetField(iField,val.c_str());
            }
        };

        template<>
        struct OGRtype<int>{
            static constexpr OGRFieldType type(){return OFTInteger; }
            static constexpr bool isGeo(){return false;}
            static int valueFromFeat(OGRFeature * feat, const int iField){
                return feat->GetFieldAsInteger(iField);
            }
            static void valueToFeat(OGRFeature * feat, int & val, const int iField){
                feat->SetField(iField,val);
            }
        };

        template<>
        struct OGRtype<long long int>{
            static constexpr OGRFieldType type(){return OFTInteger64; }
            static constexpr bool isGeo(){return false;}
            static long long int valueFromFeat(OGRFeature * feat, const int iField){
                return feat->GetFieldAsInteger64(iField);
            }
            static void valueToFeat(OGRFeature * feat, long long int & val, const int iField){
                feat->SetField(iField,val);
            }
        };


        template<>
        struct OGRtype<double>{
            static constexpr OGRFieldType type(){return OFTReal; }
            static constexpr bool isGeo(){return false;}
            static double valueFromFeat(OGRFeature * feat, const int iField){
                return feat->GetFieldAsDouble(iField);
            }
            static void valueToFeat(OGRFeature * feat, double & val, const int iField){
                feat->SetField(iField,val);
            }
        };

        template<>
        struct OGRtype<std::unique_ptr<OGRGeometry>>{
            static constexpr OGRwkbGeometryType type(){return wkbUnknown; }
            static constexpr bool isGeo(){return true;}
            static std::unique_ptr<OGRGeometry> valueFromFeat(OGRFeature * feat, const int iField){
                return std::unique_ptr<OGRGeometry> (feat->StealGeometry(iField));
            }
            static void valueToFeat(OGRFeature * feat, std::unique_ptr<OGRGeometry> & val, const int iField){
                feat->SetGeomField(iField,val.get());

            }

        };



///@brief an OGRGroup points to a certain layer in an GDAL data source
    class OGRGroup:public Group{
        public:
            ~OGRGroup(){}
            OGRGroup():Group(){}
            OGRGroup(OGRLayer * const layer);
            OGRSpatialReference * getOGRspatialRef()const;
//            OGRFeatureDefn * getOGRFeatDef()const{return layerdef_;}


            OGRLayer* getLayer()const{return layer_;}
            int createField(OGRFieldDefn * infield );
            int createField(OGRGeomFieldDefn * ingeofield);
            OGRFeature * getFeature(const size_t idx=-1);
            const OGRFeature * getFeature(const size_t idx=-1)const;
        private:
            void loadCollection();
            void parentHook();


            OGRLayer *layer_ = nullptr;
            OGRFeatureDefn *layerdef_=nullptr;
            OGRFeature* currentFeat=nullptr;
            bool readlayer();
            bool createlayer();

        };


        class OGRVar:public Variable<>{
        public:
            using fieldPtr=std::shared_ptr<OGRFieldDefn>;
            using geoFieldPtr=std::shared_ptr<OGRGeomFieldDefn>;
            ~OGRVar(){}
            OGRVar(OGRFieldDefn * const fieldef,const int id);
            OGRVar(OGRGeomFieldDefn * const geomfieldef, const int id);

            template<class T>
            OGRVar(const std::string &fieldName,T*);

            bool isGeom()const{return bool(geomfielddef_);}
            OGRFieldType getType()const{return fieldef_->GetType();}
            int getFieldId()const{return fieldid_;}
            singlePtr getValue(const size_t idx)const;
            void setValue(singlePtr & in, const size_t idx);
        private:
            void parentHook();
            OGRGroup * ogrparent_=nullptr;
            fieldPtr fieldef_=nullptr;
            geoFieldPtr geomfielddef_=nullptr;
            OGRLayer* layer_=nullptr; //copy of the relevant layer pointer
//            OGRFeature* feat=nullptr;
            int fieldid_=-1;
        };



        template<class T>
        OGRVar::OGRVar(const std::string &fieldName, T *):Variable(fieldName){

            if (OGRtype<T>::isGeo()){
                //note: this class now owns fielddef_
                geomfielddef_=std::make_shared<OGRGeomFieldDefn>(getName().c_str(),OGRtype<T>::type());

            }else{
                //note: this class now owns fielddef_
                fieldef_=std::make_shared<OGRFieldDefn>(getName().c_str(),OGRtype<T>::type());
            }

        }

        template<class T,class F=OGRFieldDefn>
        class OGRVarBase:public Variable<T>{
        public:
            using fieldPtr=std::shared_ptr<F>;
            using typename Variable<T>::single;
            using typename Variable<T>::singlePtr;
            using Variable<T>::writable;
            using Variable<T>::readable;
            using Variable<T>::getParent;
            using Variable<T>::getName;

            ~OGRVarBase(){}
            OGRVarBase(F * const fieldef,const int id);
            OGRVarBase(const std::string &fieldName);;
            using geotype=typename std::is_same<F,OGRGeomFieldDefn>::type;
            singlePtr getValue(const size_t idx)const;
            void setValue(singlePtr & in, const size_t idx);
        private:
            void parentHook();
            OGRGroup * ogrparent_=nullptr;
            fieldPtr fieldef_=nullptr;
            OGRLayer* layer_=nullptr; //copy of the relevant layer pointer
            int fieldid_=-1;
        };



    }
}
#endif //FROMMLE_OGRIOARCHIVES_HPP
