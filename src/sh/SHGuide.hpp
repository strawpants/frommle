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
#include "io/Group.hpp"
#include <string>
#include <regex>
#ifndef FROMMLE_SHDIMENSION_HPP
#define FROMMLE_SHDIMENSION_HPP

namespace frommle{
    namespace guides{

        inline int nmax_from_sz(const size_t sz){
            return (std::sqrt(1.0+8*(sz-1))-1.0)/2.0;
        }

        ///@brief describes a Guide which has no separate entry for trigonometric C/S (e.g. to store associated legendre polynomials)
        class SHnmHalfGuide:public GuideBase, public InjectGuideIterator<SHnmHalfGuide,std::tuple<int,int>>{
            public:
            using Element=std::tuple<int,int>;
            SHnmHalfGuide();
            SHnmHalfGuide(const int nmax); //:GuideBase("SHnmHalfGuide",core::typehash("SHnmHalfGuide").add(nmax)sz),nmax_(nmax),nmin_(nmin){}
            int nmax()const{return nmax_;}
            int nmin()const{return 0;}
            using const_iterator=InjectGuideIterator<SHnmHalfGuide,Element>::const_iterator;
            using iterator=InjectGuideIterator<SHnmHalfGuide,Element>::iterator;
            using InjectGuideIterator<SHnmHalfGuide,Element>::begin;
            using InjectGuideIterator<SHnmHalfGuide,Element>::end;
//            using InjectGuideIterator<SHnmHalfGuide>::cbegin;
//            using InjectGuideIterator<SHnmHalfGuide>::cend;
            /*! brief returns a vectorized index for order sorted spherical harmonics (no trigonometric variable)
             * @param n input degree
             * @param m input order
             * @param nmax maximum degree to accomodate for
             * @return zero based index of the corresponding entry
             */
            inline static size_t i_from_nm(const int n,const int m, const int nmax){
                assert(m<=n);
                assert(n<=nmax);
                return m*(nmax+1)-(m*(m+1))/2+n;
            }

            /*!brief retrieve the degree and order from a index sorted by order and degree (no trig variable
            *
            * @param idx input index (zero based)
            * @param nmax maximum degree for which has been allocated
            * @return a tuple containing the degree and order in the first and second element respectively
            */
            inline static std::tuple<int,int> nm_from_i(const size_t idx, const int nmax){
                int m=(3.0+2*nmax)/2-std::sqrt(std::pow(3+2*nmax,2)/4.0-2*idx);
                int n=idx-(((m+1)*(m+2))/2+m*(nmax-m))+m+1;
                assert(m<=n);
                assert(n<=nmax);
                return std::make_tuple(n,m);

            }

            //inline index idx(const Element el)const{return idxfromEl(el);}

            inline size_t idx(const int n,const int m)const{
                return i_from_nm(n,m,nmax_);
            }
            inline size_t idx(const Element & el)const{
               return idx(std::get<0>(el),std::get<1>(el)); 
            }
            Element operator[](const size_t idx)const{return nm_from_i(idx,nmax_);}
           size_t size()const override{
                if (nmax_==-1){
                    return 0;
                }
                return i_from_nm(nmax_,nmax_,nmax_)+1;
            }
            private:
                int nmax_=-1;
        };

        enum trigenum{C=0,S=1};
        using nmtEl=std::tuple<int,int,trigenum>;

        /*!brief
         * SHGuideBase groups all SH harmonic dimensions together
         */
    class SHGuideBase:public GuideBase,public InjectGuideIterator<SHGuideBase,nmtEl>{
        public:
            using trig=trigenum;
            using Element=nmtEl;
            SHGuideBase():GuideBase("shg",typehash("SHGuideBase")){}
            int nmax()const{return nmax_;}
            int nmin()const{return nmin_;}
            SHGuideBase(const std::string & name, const int nmax,const int nmin):GuideBase(name),nmax_(nmax),nmin_(nmin){
                size_=size();
                rehash();
            }
            size_t idxfromEl(const Element el)const;
            //helper function to make C++ interface consistent
            inline size_t idx(const Element el)const{return idxfromEl(el);}

            virtual size_t idx(const int n,const int m,const trig t)const=0;
            virtual Element operator[](const size_t idx)const=0;
//            virtual Element & operator[](const size_t idx)=0;

            using const_iterator=InjectGuideIterator<SHGuideBase,Element>::const_iterator;
            using iterator=InjectGuideIterator<SHGuideBase,Element>::iterator;
            using InjectGuideIterator<SHGuideBase,Element>::begin;
            using InjectGuideIterator<SHGuideBase,Element>::end;
            void createFrom(const std::shared_ptr<core::Frommle> & frptr) override{
                auto shptr=dynamic_cast<SHGuideBase*>(frptr.get());
                if( not shptr){
                    THROWINPUTEXCEPTION("Frommle pointer cannot be cast to a SHGuide");
                }
                nmax_=shptr->nmax_;
                nmin_=shptr->nmin_;
                size_=size();
                rehash();
            }
            size_t size()const override{
                if (nmax_==-1){
                    return 0;
                }
                return 2*(SHnmHalfGuide::i_from_nm(nmax_,nmax_,nmax_)+1);
            }
        protected:
            int nmax_=-1;
            int nmin_=-1;
            size_t size_=0;
        private:
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
            SHtmnGuide(const int nmax,const std::string name="shg");
            size_t idx(const int n, const int m, const trig t)const;
            Element operator[](const size_t idx)const;
        private:
            void rehash();

        };

        /*!brief
         * SHnmtGuide holds a SH sorting scheme which varies fastest in trigonometric type, then order and degree is slowest. It includes order 0 sine coefficients
         */
        class SHnmtGuide: public SHGuideBase{
        public:
            using SHGuideBase::trig;
            using SHGuideBase::Element;
            using SHGuideBase::idx;
            using GuideBase::hash;
            SHnmtGuide()=default;
//            SHtmnGuide(const int nmax);
            SHnmtGuide(const int nmax,const std::string name="shg");
            SHnmtGuide & operator = (const SHnmtGuide & gin){
                nmax_=gin.nmax_;
                return *this;
            }
            void createFrom(typehash hashval) override {

                //initiate from a typehase string
                std::regex reg("SHnmtGuide.*");
                if ( !std::regex_match(std::string(hashval),reg)){
                   THROWINPUTEXCEPTION("SHGuide type not matching"); 
                }
                //extract nmax from the hash
                nmax_=std::stoi(hashval.split()[1]);
                size_=size();
                // note nmin_ remains 0
                //recreate the hash
                rehash();

            }
            size_t idx(const int n, const int m, const trig t)const;
            Element operator[](const size_t idx)const;
            inline static size_t i_from_nmt(const int n, const int m, const SHGuideBase::trig t){
                return 2*((n*(n+1))/2+m)+t;
            }
        private:
            void rehash();

        };


		std::ostream &operator<<(std::ostream &os, SHGuideBase::Element const &el);

		std::ostream &operator<<(std::ostream &os, SHnmHalfGuide::Element const &el);

    }
}

        //declare some stream operators


#endif //FROMMLE_SHDIMENSION_HPP
