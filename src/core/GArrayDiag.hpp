/*! \file GArrayDiagonal.hpp
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

 Created by Roelof Rietbroek,  2019

 */

#include "core/GArrayBase.hpp"
#include "core/Exceptions.hpp"

#ifndef FROMMLE_GARRAYDIAGONAL_HPP
#define FROMMLE_GARRAYDIAGONAL_HPP
namespace frommle{
    namespace core{
        ///@brief contains a diagonal array
        template<class T>
        class GArrayDiag:public GArrayBase<T,2>{
        public:
            using eigd=typename Eigen::DiagonalMatrix<T,Eigen::Dynamic,Eigen::Dynamic>;
            using arr=boost::multi_array_ref<T, 1>;
            using GArrayBase<T,2>::gpp;
            using GArrayBase<T,2>::gp;
            using typename GArrayBase<T,2>::gp_ptr_t;

            GArrayDiag():GArrayBase<T,2>(){}

            ///@brief constructor which uses the same guide for both dimensions (it copies the internal guide)
            template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<1>, GP>::value, int> ::type = 0>
            GArrayDiag(GP guidepack):GArrayBase<T,2>(guidepack.append(guidepack.gv(0))),eig_(gpp()->at(0)->size()){}

            GArrayDiag(gp_ptr_t guidepackptr): GArrayBase<T,2>(guidepackptr){}

            //returns the diagonal
            eigd & eig(){return eig_;}
            const eigd & eig()const{return eig_;}

            arr mat(){return arr(eig_.data(),gpp()->extent());}
            GArrayBase<T,2> &operator=(const T scalar)override{
               eig_.diagonal().array()=scalar;
               return *this;
            }
        private:
            //internal data is contained within an eigen matrix
            eigd eig_{};
        };


    }
}
#endif //FROMMLE_GARRAYDIAGONAL_HPP
