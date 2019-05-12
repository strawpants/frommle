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
#include <boost/serialization/split_member.hpp>
#include "io/OGRIArchive.hpp"
#include "memory"
#ifndef FROMMLE_OGRGUIDE_HPP
#define FROMMLE_OGRGUIDE_HPP

namespace frommle{
    namespace geometry{
        template<class T,class Rtree=void>
        class OGRGuide:public core::GuideBase{
        public:
            using Element=T;
            OGRGuide(){}

            void push_back(Element &&geometry){
                geoms_.push_back(std::move(geometry));
                ++size_;
            }

            void push_back(const Element &geometry){
                geoms_.push_back(geometry);
                ++size_;
            }

           typename std::vector<Element>::const_iterator begin() const { return geoms_.begin(); }
           typename std::vector<Element>::const_iterator end() const { return geoms_.end(); }

            typename std::vector<Element>::iterator begin() { return geoms_.begin(); }
            typename std::vector<Element>::iterator end() { return geoms_.end(); }
            //spatial queries
            //...
        private:
            friend class boost::serialization::access;
            BOOST_SERIALIZATION_SPLIT_MEMBER()
            template<class Archive>
            void load(Archive & Ar,const unsigned int file_version);
            template<class Archive>
            void save(Archive & Ar,const unsigned int file_version)const{}
            std::vector <Element> geoms_={};
            //Rtree stuff
            void buildRtree();


        };

        template<class T>
        template<class Archive>
        void OGRGuide<T>::load(Archive &Ar,const unsigned int file_version){

            //retrieve the variable which holds geometry info
            const auto geovar=Ar.geoVar();
            for (auto const & geom:*geovar ){
                auto geomref=dynamic_cast<T*>(geom->template as<OGRGeometry*>());
                geoms_.push_back(T(*geomref));
            }
////            do while (tref=geoVar.next<T>()){
////
////            }
//
//            for(const auto & val:geoVar){
//                    geoms_.push_back(val.as<T>);
//            }
            //loop over OGRgeometries from the Archive


//            OgrAr->setVar("dummy");
//            tmp=OgrAr->getOGRGeometry();
//            //get all features
//            while(tmp){
//
//                //try a dynamic cast
//                OGRGeotype * Gtype= dynamic_cast<OGRGeotype*>(tmp);
//
//                if (!Gtype){
//                    throw frommle::IOException("ERROR: OGR geometry can not be cast to derived type");
//                }
//                //make a copy and append to the data vector
//                data_.push_back(OGRGeotype(*Gtype));
//
//                OgrAr->setVar("dummy");
//                tmp=OgrAr->getOGRGeometry();
//            }
//            transformSpatialReference(OgrAr->getOGRspatialRef());

        }



    }
}


#endif //FROMMLE_OGRGUIDE_HPP
