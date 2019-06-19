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

#include <eigen3/Eigen/Core>
#include <boost/multi_array.hpp>
#include <core/Exceptions.hpp>
#ifndef FROMMLE_MARRAYWRAPPER_HPP
#define FROMMLE_MARRAYWRAPPER_HPP


namespace frommle{
    namespace core{
    using namespace Eigen;


    /*!@brief
     * Wraps a boost multi_array as a Eigen::matrix
     * Note that the returned eigen::map points to the same data as the input multi_array
     * @tparam T type contained int he array
     * @param marr
     * @return
     */
    template<typename T>
    Eigen::Map<Matrix<T,Dynamic,Dynamic> ,Unaligned, Stride<Dynamic,Dynamic>> marray_to_eigen(boost::multi_array_ref<T,2> & marr){
      using Strd=Stride<Dynamic,Dynamic>;

        //Make sure the storage order agrees ..
//        if (marr.storage_order() != boost::fortran_storage_order()){
//            throw IndexingException(HERE+"Storage orders do not match");
//        }

        auto marrStrd=Strd(marr.strides()[0],marr.strides()[1]);
        return Map<Matrix<T,Dynamic,Dynamic>,0,Strd>(marr.data(),marr.shape()[0],marr.shape()[1],marrStrd);
    }



}
}



#endif //FROMMLE_MARRAYWRAPPER_HPP
