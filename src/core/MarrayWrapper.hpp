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

    /*!@brief
     * Wraps a boost multi_array as a Eigen::matrix
     * Note that the returned eigen::map points to the same data as the input multi_array
     * @tparam T type contained int he array
     * @param marr
     * @return
     */

    template<typename T>
    Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> > marray_to_eigenmat(boost::multi_array<T,2> & marr){
        //Currently this assumes no strides so we need to check whether the input is contiguous (all strides are 1)
        for(int i=0;i<2;++i) {
            if (marr.strides()[i] != 1) {
                throw IndexingException("Only strides of 1 are currently allowed");
            }
        }
      return Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>>(marr.data(),marr.shape()[0],marr.shape()[1]);
    };


    template<typename T>
    Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> > marray_to_eigenmat(boost::multi_array<T,1> & marr){
        //Currently this assumes no strides so we need to check whether the input is contiguous (all strides are 1)
        for(int i=0;i<1;++i) {
            if (marr.strides()[i] != 1) {
                throw IndexingException("Only strides of 1 are currently allowed");
            }
        }
        return Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>>(marr.data(),marr.shape()[0]);
    };

    template<typename T>
    Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> > marray_to_eigenmat(boost::multi_array<T,3> & marr){
        //Currently this assumes no strides so we need to check whether the input is contiguous (all strides are 1)
        for(int i=0;i<3+i) {
            if (marr.strides()[i] != 1) {
                throw IndexingException("Only strides of 1 are currently allowed");
            }
        }
//        return Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>>(marr.data(),marr.shape()[0]);
    };

}
}



#endif //FROMMLE_MARRAYWRAPPER_HPP
