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

#include "sh/SHGuide.hpp"
#include <cmath>

namespace frommle {
    namespace sh {

        using frommle::core::GuideBase::index;

        index SHtmnGuide::idx(const SHtmnGuide::Element & in)const{
            int n,m;
            trig t;
            std::tie(n,m,t)=in;
            index shft=(t==trig::C)?0:size_/2;
            return SHtmnDim::i_from_mn(n,m,nmax_)+shft;
        }

        index SHtmnGuide::idx(const int n, const int m, const trig t)const{
            index shft=(t==trig::C)?0:size_/2;
            return SHtmnDim::i_from_mn(n,m,nmax_)+shft;
        }


        SHtmnGuide::Element SHtmnGuide::operator[](const index idx)const{
            int n,m;
            trig t=(idx<size_/2)?trig::C:trig::S;
            std::tie(n,m)=SHtmnGuide::mn_from_i(idx,nmax_);
            return std::make_tuple(n,m,t);
        }

        SHtmnGuide::Element & SHtmnGuide::operator[](const index idx){
            int n,m;
            trig t=(idx<size_/2)?trig::C:trig::S;
            std::tie(n,m)=SHtmnGuide::mn_from_i(idx,nmax_);
            return std::make_tuple(n,m,t);

        }

        void SHtmnGuide::const_iterator::increment() {

        }

        /*! brief returns a vectorized index for order sorted spherical harmonics (no trigonometric variable)
         * @param n input degree
         * @param m input order
         * @param nmax maximum degree to accomodate for
         * @return zero based index of the corresponding entry
         */
        index SHtmnGuide::i_from_mn(const int n,const int m, const int nmax);{
            assert(m<=n);
            return m*(nmax+1)-(m*(m+1))/2+n;
        }

        /*!brief retrieve the degree and order from a index sorted by order and degree (no trig variable
        *
        * @param idx input index (zero based)
        * @param nmax maximum degree for which has been allocated
        * @return a tuple containing the degree and order in the first and second element respectively
        */
        std::tuple<int,int> SHtmnGuide::mn_from_i(const index idx, const int nmax){
            int m=(3.0+2*nmax)/2-std::sqrt(std::pow(3+2*nmax,2)/4.0-2*idx);
            int n=idx-(((m+1)*(m+2))/2+m*(nmax-m))+m+1;
            assert(m<=n);
            return std::make_tuple(n,m);

        }



    }
}