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
#include <ogrsf_frmts.h>
#ifndef FROMMLE_OGRITERATORBASE_HPP
#define FROMMLE_OGRITERATORBASE_HPP
namespace frommle{
    namespace geometry{

        class OGRiteratorBase{
        public:
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = OGRGeometry;
            using difference_type = std::ptrdiff_t;
            using pointer = OGRGeometry*;
            using reference = OGRGeometry&;
            virtual OGRiteratorBase& operator++(){ return *this;};
            bool operator==(OGRiteratorBase & other) const {return elVal == other.elVal;}
            bool operator!=(OGRiteratorBase & other) const {return !(*this == other);}
            OGRGeometry * operator*() {return elVal;}
            virtual ~OGRiteratorBase(){}
            OGRiteratorBase(){}
        protected:
            OGRGeometry *elVal=nullptr;
        private:
        };


    }
}
#endif //FROMMLE_OGRITERATORBASE_HPP
