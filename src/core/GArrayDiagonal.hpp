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
        template<class T>
        class GArrayDiagonalDyn{
        public:
            using gp_t=guides::GuidePackDyn<2>;
            using gp_tptr=std::shared_ptr<guides::GuidePackDyn <2>>;

            using eigm=typename Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic>;

            GArrayDiagonalDyn():diag_(),gp_(){
            }
            GArrayDiagonalDyn(gp_t &&guidepack):gp_(std::make_shared<GP>(std::move(guidepack))),
                diagdata_(std::shared_ptr<T[]>(new T[gp_->at(0)->size()])){
                if (gp_->at(0).size() ~= gp_->at(1).size()){
                    THROWINPUTEXCEPTION("Dimensions should be equal when creating a diagonal matrix");
                }
            }
        private:
            gp_tptr gp_{};
            std::shared_ptr<T[]> diagdata_{};

        };


    }
}
#endif //FROMMLE_GARRAYDIAGONAL_HPP
