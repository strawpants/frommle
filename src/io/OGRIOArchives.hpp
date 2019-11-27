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
    namespace io {

        std::string GDALPOSTGISSource(const std::string PGname = "", const std::string schemas = "");

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
        struct OGRtype<std::string> {
            using fieldtype=OGRFieldDefn;
            static constexpr OGRFieldType type() { return OFTString; }

            static constexpr bool isGeo() { return false; }

            static void valueFromFeat(OGRFeature *feat, std::string & val,const int iField) {
                val=std::string(feat->GetFieldAsString(iField));
            }

            static void valueToFeat(OGRFeature *feat, const std::string &val, const int iField) {
                feat->SetField(iField, val.c_str());
            }
        };

        template<>
        struct OGRtype<int> {
            using fieldtype=OGRFieldDefn;
            static constexpr OGRFieldType type() { return OFTInteger; }

            static constexpr bool isGeo() { return false; }

            static void valueFromFeat(OGRFeature *feat, int & val, const int iField) {
                val=feat->GetFieldAsInteger(iField);
            }

            static void valueToFeat(OGRFeature *feat, const int &val, const int iField) {
                feat->SetField(iField, val);
            }
        };

        template<>
        struct OGRtype<long long int> {
            using fieldtype=OGRFieldDefn;
            static constexpr OGRFieldType type() { return OFTInteger64; }

            static constexpr bool isGeo() { return false; }

            static void valueFromFeat(OGRFeature *feat, long long int & val,const int iField) {
                val=feat->GetFieldAsInteger64(iField);
            }

            static void valueToFeat(OGRFeature *feat, const long long int &val, const int iField) {
                feat->SetField(iField, val);
            }
        };


        template<>
        struct OGRtype<double> {
            using fieldtype=OGRFieldDefn;
            static constexpr OGRFieldType type() { return OFTReal; }

            static constexpr bool isGeo() { return false; }

            static void valueFromFeat(OGRFeature *feat, double & val,const int iField) {
                val=feat->GetFieldAsDouble(iField);
            }

            static void valueToFeat(OGRFeature *feat, const double &val, const int iField) {
                feat->SetField(iField, val);
            }
        };

        template<class T>
        struct OGRtype{
            using fieldtype=OGRGeomFieldDefn;
            static constexpr OGRwkbGeometryType type() { return wkbUnknown; }

            static constexpr bool isGeo() { return true; }

            static void valueFromFeat(OGRFeature *feat, T & val,const int iField) {
                auto tmp=static_cast<T*>(feat->StealGeometry(iField));
                val=*tmp;
            }

            static void valueToFeat(OGRFeature *feat, const T & val, const int iField) {
                if(iField !=0){
                    throw core::IndexingException("can only support one geometry field currently");
                }
//                feat->SetGeometry(&val);
                feat->SetGeomField(iField, &val);

            }

        };




        ///@brief an OGRGroup points to a certain layer in an GDAL data source
        class OGRGroup : public Group {
        public:
            ~OGRGroup();

            OGRGroup() : Group() {}

            OGRGroup(OGRLayer *const layer);
            OGRGroup(const std::string name):Group(name){}

            OGRGroup(core::TreeNodeRef && in):Group(std::move(in)){}
            OGRSpatialReference *getOGRspatialRef() const;
//            OGRFeatureDefn * getOGRFeatDef()const{return layerdef_;}


            OGRLayer *getLayer() const { return layer_; }

            int getField(OGRFieldDefn *infield);

            int getField(OGRGeomFieldDefn *ingeofield);

            OGRFeature *getFeature(const ptrdiff_t idx = -1);

            const OGRFeature *getFeature(const ptrdiff_t idx = -1) const;

            core::TreeNodeRef convertChild(core::TreeNodeRef &&in);
        private:
            void loadCollection();

            void parentHook();


            OGRLayer *layer_ = nullptr;
            OGRFeatureDefn *layerdef_ = nullptr;
            OGRFeature *currentFeat = nullptr;

            bool readlayer();

            bool createlayer();

        };



        template<class T>
        class OGRVarBase : public Variable<T> {
        public:
            using fieldtype=typename OGRtype<T>::fieldtype;
            using fieldPtr=std::shared_ptr<fieldtype >;
            using typename Variable<T>::single;
            using typename Variable<T>::singlePtr;
            using Variable<T>::writable;
            using Variable<T>::readable;
            using Variable<T>::getParent;
            using Variable<T>::name;

            ~OGRVarBase() {}

            OGRVarBase(fieldtype *const fieldef, const int id);

            OGRVarBase(const std::string &fieldName);
            OGRVarBase(core::TreeNodeRef && in);

            using geotype=typename std::is_same<fieldtype , OGRGeomFieldDefn>::type;

            void getValue(singlePtr & in, const ptrdiff_t idx) const;

            void setValue(const singlePtr & in, const ptrdiff_t idx);

        private:
            void parentHook();

            OGRGroup *ogrparent_ = nullptr;
            fieldPtr fieldef_ = nullptr;
            OGRLayer *layer_ = nullptr; //copy of the relevant layer pointer
            int fieldid_ = -1;
        };

        ///@brief a factory method to create various OGRVarBase objects
        core::TreeNodeRef OGRVarfactory(OGRFeatureDefn *featdef, const size_t id, const bool isgeo);
    }
}
#endif //FROMMLE_OGRIOARCHIVES_HPP
