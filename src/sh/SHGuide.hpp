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

#include "core/GuideBase.hpp"
#include <cassert>
#include <cmath>
#include <boost/iterator/iterator_adaptor.hpp>
#include <iostream>
#ifndef FROMMLE_SHDIMENSION_HPP
#define FROMMLE_SHDIMENSION_HPP

namespace frommle{
    namespace sh{
        using frommle::core::index;

        inline int nmax_from_sz(const size_t sz){
            return (std::sqrt(1.0+8*(sz-1))-1.0)/2.0;
        }

        /*!brief
         * SHGuideBase groups all SH harmonic dimensions together
         */
        class SHGuideBase:public frommle::core::GuideBase{
        public:
            using trig=enum {C=0,S=1};
            using Element=std::tuple<int,int,trig>;
            SHGuideBase()=default;
            int nmax()const{return nmax_;}
            int nmin()const{return nmin_;}
            SHGuideBase(const std::string & name, const core::typehash & type,const index sz,const int nmax,const int nmin):GuideBase(name,type,sz),nmax_(nmax),nmin_(nmin){}
            index idxfromEl(const Element el)const;
            //helper function to make C++ interface consistent
            inline index idx(const Element el)const{return idxfromEl(el);}

            virtual index idx(const int n,const int m,const trig t=trig::C)const=0;
            virtual Element operator[](const index idx)const=0;
//            virtual Element & operator[](const index idx)=0;

        //nested class which acts as an iterator
        class const_iterator:public frommle::core::Guideiterator<Element ,const_iterator>{
        public:
            const_iterator():Guideiterator(Element(-1,-1,trig::C)){}
            const_iterator(const SHGuideBase * shg):Guideiterator(shg->operator[](0)),gptr_(shg),sz_(shg->size()),idx_(0){}
            const_iterator operator++(int){
                const_iterator retval(*this);
                ++(*this);
                return retval;
            }
            const_iterator & operator++(){
                ++idx_;
                if (idx_==sz_){
                    //stops iteration
                    elVal=Element(-1,-1,trig::C);
                }else {
                    elVal = gptr_->operator[](idx_);
                }
                return *this;
            }

        private:
            const SHGuideBase* gptr_=nullptr;
            index sz_=0;
            index idx_=0;
        };
            const_iterator begin()const{return const_iterator(this);}
            const_iterator end()const{return const_iterator();}
        protected:
            int nmax_=0;
            int nmin_=0;
        };

        /*!brief
         * SHmnGuide holds a SH sorting scheme which varies quickly in degree and than order
         * The scheme uses the sorting order for Cosines and Sine coefficients and thus
         * includes S_n0 terms (which should be zero)
         */
        class SHtmnGuide: public SHGuideBase{
        public:
            using SHGuideBase::trig;
            using SHGuideBase::Element;
            using SHGuideBase::idx;
            SHtmnGuide()=default;
//            SHtmnGuide(const int nmax);
            SHtmnGuide(const int nmax,const std::string name="SHtmnGuide");
            index idx(const int n, const int m, const trig t=trig::C)const;
            Element operator[](const index idx)const;

            /*! brief returns a vectorized index for order sorted spherical harmonics (no trigonometric variable)
             * @param n input degree
             * @param m input order
             * @param nmax maximum degree to accomodate for
             * @return zero based index of the corresponding entry
             */
            inline static index i_from_mn(const int n,const int m, const int nmax){
                assert(m<=n);
                return m*(nmax+1)-(m*(m+1))/2+n;
            }

            /*!brief retrieve the degree and order from a index sorted by order and degree (no trig variable
            *
            * @param idx input index (zero based)
            * @param nmax maximum degree for which has been allocated
            * @return a tuple containing the degree and order in the first and second element respectively
            */
            inline static std::tuple<int,int> mn_from_i(const index idx, const int nmax){
                int m=(3.0+2*nmax)/2-std::sqrt(std::pow(3+2*nmax,2)/4.0-2*idx);
                int n=idx-(((m+1)*(m+2))/2+m*(nmax-m))+m+1;
                assert(m<=n);
                return std::make_tuple(n,m);

            }

        };

    }
}


#endif //FROMMLE_SHDIMENSION_HPP
