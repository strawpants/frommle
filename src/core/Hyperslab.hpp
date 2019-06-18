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


#include <boost/multi_array.hpp>
#ifndef FROMMLE_HYPERSLAB_HPP
#define FROMMLE_HYPERSLAB_HPP
namespace frommle{
    namespace core {

        template<class T>
        class Hyperslab {
        public:
            template<size_t ndim>
            Hyperslab(const boost::multi_array<T,ndim> &arr){
                ndim_=ndim;
                cdata_= arr.origin();
                offset_=std::vector<size_t >(ndim_,0);
                stride_=std::vector<size_t >(ndim_,1);
                auto shape=arr.shape();
                count_=std::vector<size_t>(shape,shape+ndim_);

            }
        const T* data()const{return cdata_;}
        T* data(){return data_;}
        const std::vector<size_t > & extents()const{return count_;}
        private:
            size_t ndim_=0;
            T *data_ = nullptr;
            const T* cdata_=nullptr;
            std::vector<size_t> offset_{};
            std::vector <size_t> count_{};
            std::vector <size_t> stride_{};
//            std::vector <size_t> map_{};
        };
    }
}
#endif //FROMMLE_HYPERSLAB_HPP
