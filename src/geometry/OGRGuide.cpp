/*! \file
 \brief 
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

#include "geometry/OGRGuide.hpp"

namespace frommle{
    namespace guides{

        template<class T>
        void OGRGuide<T>::push_back(T &&geometry) {
            geoms_.push_back(std::make_shared<T>(std::move(geometry)));
        }

        ///@brief create a boost rtree index of the geometry using the packing algorithm
        template<class T>
        void OGRGuide<T>::buildRtree() {
            //construct a vector with idxmap (box + index)
            std::vector<idxmap> idxboxes{};
            size_t indx=0;
            OGREnvelope env{};
            for(auto & geom:geoms_){
                geom->getEnvelope(&env);

                idxboxes.push_back(idxmap(box(point(env.MinX,env.MinY),point(env.MaxX,env.MaxY)),indx++));
            }
            rtreeIndex=std::unique_ptr<rtree>(new rtree(idxboxes.cbegin(),idxboxes.cend()));


        }

        template<class T>
        const typename OGRGuide<T>::rtree &OGRGuide<T>::getRtree() {
            if(!rtreeIndex){
                buildRtree();
            }
            return *rtreeIndex;
        }

        template<class T>
        void OGRGuide<T>::push_back(const std::string &WKT) {
//                geoms_.push_back(std::make_shared<T>());
            OGRGeometry ** geomptr= new OGRGeometry*;
//                *geomptr=geoms_.back().get();
            if (OGRERR_NONE != OGRGeometryFactory::createFromWkt(WKT.c_str(),&SpatialRef_,geomptr)){
                THROWINPUTEXCEPTION("Failed to create OGR geometry");
            }
            geoms_.push_back(std::make_shared<T>(static_cast<T&>(**geomptr)));
            OGRGeometryFactory::destroyGeometry(*geomptr);
            delete geomptr;
        }

        template<class T>
        void OGRGuide<T>::push_back(const T &geometry) {
            geoms_.push_back(std::make_shared<T>(geometry));
        }


        template class OGRGuide<OGRPolygon>;
        template class OGRGuide<OGRPoint>;

    }


}
