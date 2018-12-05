/*! \file
 \brief Definition file for the SHDimension class
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

 Created by Roelof Rietbroek, 2018

 */

#include "src/core/DimensionBase.hpp"

#ifndef FROMMLE_SHDIMENSION_HPP
#define FROMMLE_SHDIMENSION_HPP

namespace frommle{
    namespace sh{

        enum trig {C=0,S=1};

        struct nmt{int n; int m; trig t; };

        /*! brief
         *
         *
         */
        class SHDimension: public frommle::DimensionBase {
        public:
            using frommle::DimensionBase::index;
            SHDimension(const int nmaxin){
                nmax_=nmaxin;
            }

            /*
             *
             *
             */
            index operator[](const nmt & in)const{
                index idx=(in.n*(in.n+1))/2+in.m+1+in.t*size();
                return idx;
            }

            nmt invAt(const index in )const{
                nmt idx={0,0,trig::C};
                return idx;
            }

            index size()const{
                index sz=nmax_*(nmax_+1);
                return sz;
            }

            //Index function to get a specific function from n,m,trig
//            static index nmt(const int n, const int m, const trig cs ){
//                return
//            }
            //Iterator over most cache localized sequence of degrees,order, trig

        protected:
        private:
                int nmax_=0;
        };

    }
}


#endif //FROMMLE_SHDIMENSION_HPP
