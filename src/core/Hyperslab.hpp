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
#ifndef FROMMLE_HYPERSLAB_HPP
#define FROMMLE_HYPERSLAB_HPP
namespace frommle{
    namespace core {

        template<class T>
        class Hyperslab {
        public:
            static const int maxdim = 9;
            T *data = nullptr;
            std::array <size_t, maxdim> offset{0};
            std::array <size_t, maxdim> count{0};
            std::array <size_t, maxdim> stride{1};
            std::array <size_t, maxdim> map{0};
        };
    }
}
#endif //FROMMLE_HYPERSLAB_HPP
