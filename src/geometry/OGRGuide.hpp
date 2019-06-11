/*! \file
 \brief contains a guide which can hod, a templated OGR geometry entity
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

#include "core/GuideBase.hpp"
#include <memory>
#include <vector>
#include <ogr_geometry.h>
#include <io/Group.hpp>
#include "core/Logging.hpp"

#ifndef FROMMLE_OGRGUIDE_HPP
#define FROMMLE_OGRGUIDE_HPP

namespace frommle{
    namespace geometry{
        template<class T>
        class OGRGuide:public core::GuideBase{
        public:
            using Element=T;
            using ElementContainer=std::vector<std::shared_ptr<Element>>;
            OGRGuide(){}

            void push_back(Element &&geometry){
                geoms_.push_back(std::make_shared<T>(std::move(geometry)));
                ++size_;
            }

            void push_back(const Element &geometry){
                geoms_.push_back(std::make_shared<T>(geometry));
                ++size_;
            }

            //@brief push back a new geometry from a well-known test representation
            void push_back(const std::string & WKT){
//                geoms_.push_back(std::make_shared<T>());
                OGRGeometry ** geomptr= new OGRGeometry*;
//                *geomptr=geoms_.back().get();
                if (OGRERR_NONE != OGRGeometryFactory::createFromWkt(WKT.c_str(),&SpatialRef_,geomptr)){
                    throw core::InputException("failed to create OGR geometry");
                }
                ++size_;
                geoms_.push_back(std::make_shared<T>(static_cast<T&>(**geomptr)));
                OGRGeometryFactory::destroyGeometry(*geomptr);
                delete geomptr;
            }

           typename ElementContainer::const_iterator begin() const { return geoms_.begin(); }
           typename ElementContainer::const_iterator end() const { return geoms_.end(); }

            typename ElementContainer::iterator begin() { return geoms_.begin(); }
            typename ElementContainer::iterator end() { return geoms_.end(); }
            Element & operator[](const index idx){return *(geoms_.at(idx));}
            Element & operator[](const index idx)const{return *(geoms_.at(idx));}
            //spatial queries
            //...
        private:
            friend class io::serialize;
            template<class Archive>
            void load(Archive & Ar);
            template<class Archive>
            void save(Archive & Ar)const;
            std::vector <std::shared_ptr<Element>> geoms_={};
            //Rtree stuff
            void buildRtree();
            OGRSpatialReference SpatialRef_=*OGRSpatialReference::GetWGS84SRS();


        };

        template<class T>
        template<class Archive>
        void OGRGuide<T>::save(Archive &Ar)const {
//            //create a new variable holding the geometry
            Ar["geom"]=io::Variable<T>();
            auto geovar= dynamic_cast<io::Variable<T>*>(Ar["geom"].get());
            for (auto const & geom:geoms_){
               geovar->setValue(geom,-1);
            }


        }

        template<class T>
        template<class Archive>
        void OGRGuide<T>::load(Archive &Ar){

//            //retrieve the variable which holds geometry info
            auto &geovar=Ar.template getVariable<T>("geom");
            for (auto geom:geovar ){
                this->push_back(*geom);
            }

        }



    }
}


#endif //FROMMLE_OGRGUIDE_HPP
