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
#ifndef FROMMLE_GEOGRID_HPP
#define FROMMLE_GEOGRID_HPP

namespace frommle{
    namespace geometry{
        using frommle::core::index;
        /*!brief
         * A guide class which contains the description of an equidistant grid
         * a different stepsize in longitude and latitude direction is allowed
         */
        class GeoGrid:public frommle::core::GuideBase{
        public:
            enum gridreg{pix=0,grid=1};
            using lint=long unsigned int;

            GeoGrid(double west=-180.0, double east=180.0, double south=-90, double north=90,double dlon=1.0,double dlat=1.0, const gridreg reg = gridreg::pix);

            index idx(const lint ilon,const lint ilat)const;

            std::tuple<lint,lint> ilonilat(const index idx)const;

            std::tuple<double,double> lonlat(const lint ilon, const lint ilat)const;

//        class const_iterator:public frommle::core::Guideiterator<geopoint,const_iterator>{
//        public:
//            const_iterator():Guideiterator(geopoint(-1000,-1000)),idx_(-1){}
//            const_iterator(const GeoGrid * ggrd):gptr_(ggrd),idx_(0){}
//        private:
//            const GeoGrid* gptr_=nullptr;
//            index idx_=0;
//        };
            ///@brief nested iterator class using boost iterators to loop over the points of the grid
            class const_iterator:public  boost::iterator_facade<const_iterator,geopoint const,boost::forward_traversal_tag>{
            public:
                ~const_iterator(){}
                const_iterator();
                const_iterator(const const_iterator & in){
                    currentpoint_=in.currentpoint_;
                    ilat=in.ilat;
                    ilon=in.ilon;
                    gridPtr= in.gridPtr;
                }
                const_iterator(const GeoGrid * in);
            private:
                friend class boost::iterator_core_access;
                GeoGrid const * gridPtr = nullptr;
                long long int ilat=-1;
                long long int ilon=-1;
                OGRPoint currentpoint_={};//default starts with an empty point
                void increment();
                const OGRPoint & dereference()const{return currentpoint_;}
                bool equal(const const_iterator & comp)const{return comp.ilat == ilat && comp.ilon == ilon;}

            };

            const_iterator  begin()const{return const_iterator(this);}
            const_iterator  end()const{return const_iterator();}
        private:
            //bounding box(envelope)
            bbox bbox_;
            //degree steps in x and y
            double dx_=1.0;
            double dy_=1.0;
            gridreg reg_= gridreg::pix; // pixel or gridline registration( 0 versus 1)
            lint nlon_=0;
            lint nlat_=0;
        };

    }
}

#endif //FROMMLE_GEOGRID_HPP
