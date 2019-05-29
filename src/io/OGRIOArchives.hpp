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
#include "core/UserSettings.cpp"
#include <ogrsf_frmts.h>
#include <string>
#include "geometry/OGRiteratorBase.hpp"

#ifndef FROMMLE_OGRIOARCHIVES_HPP
#define FROMMLE_OGRIOARCHIVES_HPP
namespace frommle{
    namespace io{

        std::string GDALPOSTGISSource(const std::string PGname="",const std::string schemas="") {
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

        ///@brief small singleton class which calls GDAL initialization routines (needs to be called only once)
        class GDALinit : public core::Singleton<GDALinit> {
        private:
            friend class core::Singleton<GDALinit>;

            GDALinit() {
                GDALAllRegister();
//				OGRRegisterAll();
            }

        };

        class OGRArchive;


        class OGRGroup:public Group{
        public:
            OGRGroup(const std::string & gname, const OGRArchive *  const inAr):Group(gname,inAr){
                indexlayers();
                loadlayer(gname);
            }
            OGRGroup(const OGRArchive * const inAr):Group(inAr){
                indexlayers();
                loadlayer(0);
            }
            OGRGroup(const int gid, const OGRArchive * const inAr):Group(inAr){
                indexlayers();
                loadlayer(gid);
            }

            VarRef geoVar();
            //@brief load the next layer
            OGRGroup & operator++();
            OGRSpatialReference * getOGRspatialRef()const;
            OGRFeatureDefn * getOGRFeatDef()const{return layerdef_;}
            OGRLayer * getLayer()const{return layer_;}
        private:
            VarRef at(const std::string & VarName)const;
            VarRef at(const int nvar)const;

            void loadlayer(const std::string & gname);
            void loadlayer(const int gid);
            void indexlayers();
            OGRLayer *layer_ = nullptr;
            OGRFeatureDefn *layerdef_=nullptr;
            std::map<std::string,int> layerlookup_{};

        };

        class OGRVar:public VarItem{
        public:
            OGRVar(const Group * parent);
            OGRVar(const std::string &varname, const Group *parent);
            OGRVar(const int varid, const Group *parent, const bool geo=false);
            OGRVar & operator ++();
//			OGRVar & operator >> (std::vector<OGRGeometry*> & geovec);
        private:
            friend OGRArchive;
            void loadvar(const int varid);
            void loadvar(const std::string & varName);
            void loadgeom(const int ngeom=0);
            void loadAttributes();
            inline int nField(){return static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef()->GetFieldCount();}
            inline int nGeom(){return static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef()->GetGeomFieldCount();}
            inline OGRFieldDefn * getFieldDef(const int ifield){return static_cast<const OGRGroup *>(grpParentPtr_)->getOGRFeatDef()->GetFieldDefn(ifield);}
            ValueRef at(const size_t nVal)const;

            OGRFieldDefn * fielddef_=nullptr;
            OGRGeomFieldDefn * geomfielddef_=nullptr;
        };

        class OGRValue:public ValueItem{
        public:
            OGRValue():ValueItem(){}
            OGRValue(const size_t i, const OGRVar * const parent);
            OGRValue & operator++();
        private:
            OGRLayer * layer=nullptr;
            OGRFeature* feat=nullptr;
        };





    }
}
#endif //FROMMLE_OGRIOARCHIVES_HPP
