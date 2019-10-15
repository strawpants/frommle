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
#include "geometry/OGR2boost.hpp"
#include "core/Logging.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg=boost::geometry;
namespace bgi = boost::geometry::index;

#ifndef FROMMLE_OGRGUIDE_HPP
#define FROMMLE_OGRGUIDE_HPP

namespace frommle{
    namespace guides{
        template<class T>
        class OGRGuide:public GuideBase{
        public:
            using Element=T;
            using ElementContainer=std::vector<std::shared_ptr<Element>>;
            //structors
            OGRGuide(){}
            OGRGuide(const std::string & name):GuideBase(typehash(name)){}
            OGRGuide(const std::string & name, const typehash & hash):GuideBase(name,hash){}
            OGRGuide(const OGRGuide & in)=default;

            ///@brief push_back family
            void push_back(Element &&geometry);
            void push_back(const Element &geometry);
            //@brief push back a new geometry from a well-known test representation
            void push_back(const std::string & WKT);
            using const_iterator=typename ElementContainer::const_iterator;
            using iterator=typename ElementContainer::iterator;
            const_iterator begin() const { return geoms_.cbegin(); }
            const_iterator end() const { return geoms_.cend(); }

            iterator begin() { return geoms_.begin(); }
            iterator end() { return geoms_.end(); }
            Element & operator[](const size_t idx){return *(geoms_.at(idx));}
            Element & operator[](const size_t idx)const{return *(geoms_.at(idx));}
            //spatial queries
            //Rtree stuff (store the encompassing geo box of the OGR geometry in the rtree)
            //using point=bg::model::point<double,2,bg::cs::geographic<bg::degree>>;
            //using point=bg::model::point<double,2,bg::cs::cartesian>;
            using point=OGRPoint;
            using box=bg::model::box<point>;
            
            using idxmap=std::pair<box, size_t>;
            using rtree=bgi::rtree<idxmap,bgi::rstar<16,4>>;
            const rtree & getRtree();
            //operator core::MaskedGuide<OGRGuide>(){
            //return core::MaskedGuide<OGRGuide>(static_cast<const OGRGuide&>(*this));
            //}
            using GuideBase::operator MaskedGuide<OGRGuide>;
        private:
            friend class io::serialize;
            template<class Archive>
            void load(Archive & Ar);
            template<class Archive>
            void save(Archive & Ar)const;
            std::vector <std::shared_ptr<Element>> geoms_={};
            std::shared_ptr<rtree> rtreeIndex{};
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
