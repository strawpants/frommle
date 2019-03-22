/*! \file
 \brief 
 \copyright Roelof Rietbroek 2018
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

#include <ogr_core.h>
#include "core/GuideBase.hpp"
#include "geometry/GeoTypes.hpp"
#include <boost/iterator/iterator_facade.hpp>
#include <ogr_geometry.h>

#ifndef FROMMLE_GEOPOINTS_HPP
#define FROMMLE_GEOPOINTS_HPP

namespace frommle{
    namespace geometry {


/*!brief
 * A guide class which contains an unselected list of geographical points
 *
 */
        class GeoPoints : public frommle::core::GuideBase {
        public:
            GeoPoints();

            void push_back(geopoint &&pnt);

            void push_back(const geopoint &pnt);

            void push_back(const double lon, const double lat);

            ///@brief nested iterator class using boost iterators to loop over the points of the grid
            class const_iterator
                    : public boost::iterator_facade<const_iterator, geopoint const, boost::forward_traversal_tag> {
            public:
                ~const_iterator() {}

                const_iterator(){};

                const_iterator(const const_iterator &in);


                const_iterator(const GeoPoints *in);

            private:
                friend class boost::iterator_core_access;

                GeoPoints const *GeoPointPtr = nullptr;
                long long int idx = -1;
                OGRPoint currentpoint_ = {};//default starts with an empty point
                void increment();

                const OGRPoint &dereference() const { return GeoPointPtr->points_[idx]; }

                bool equal(const const_iterator &comp) const { return comp.idx == idx; }

            };

            std::vector<geopoint>::const_iterator begin() const { return points_.begin(); }
            std::vector<geopoint>::const_iterator end() const { return points_.end(); }

            std::vector<geopoint>::iterator begin() { return points_.begin(); }
            std::vector<geopoint>::iterator end() { return points_.end(); }

        private:
            std::vector <geopoint> points_={};
        };
    }
}
#endif //FROMMLE_GEOPOINTS_HPP
