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
#include "core/IndexedGuide.hpp"
#include "core/NonIndexedGuide.hpp"
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

        
        using nmEl=std::tuple<int,int>;
        enum trigenum{C=0,S=1};
        using nmtEl=std::tuple<int,int,trigenum>;

        class SHGuideMeta{
            public:
                SHGuideMeta(){}
                SHGuideMeta(int nmax,int nmin):nmax_(nmax),nmin_(nmin){}
                int nmax()const{return nmax_;}
                int nmin()const{return nmin_;}
            protected:
                int nmax_=-1;
                int nmin_=0;
        };


        std::ostream &operator<<(std::ostream &os, nmtEl const &el);

        std::ostream &operator<<(std::ostream &os, nmEl const &el);
        
        ///@brief guide which provides a spherical harmonic guide depending on degree and m but ot on the trigonometric sign  
        class SHnmGuide:public SHGuideMeta, public NonIndexedGuide< nmEl > {
            public:
                using Element=nmEl;
                using NiGd=NonIndexedGuide<nmEl>;
                SHnmGuide():SHGuideMeta(),NiGd("shg"){}
                SHnmGuide(const int nmax):SHGuideMeta(nmax,0),NiGd("shg"){}
                core::typehash hash()const override{return core::typehash("SHnmGuide_t");}
                static size_t i_from_nm(const int n,const int m, const int nmax);
                size_t ifromel(const Element & el) const override;
                static Element nm_from_i(const size_t idx, const int nmax);
                Element elfromi(size_t i) const override;
                size_t fullsize()const override;
            private:
                using SHGuideMeta::nmax_;
        };


    
        /*!brief
         * SHtmnGuide holds a SH sorting scheme which varies quickest in degree then order and lastly the trigonemetric sign
         * The ordering also includes S_n0 terms (which should be zero)
         */
        class SHtmnGuide:public SHGuideMeta,public NonIndexedGuide< nmtEl >{
            public:
            using Element=nmtEl;
            using NiGd=NonIndexedGuide<nmtEl>;
            SHtmnGuide():SHGuideMeta(),NiGd("shg"){}
            SHtmnGuide(const int nmax):SHGuideMeta(nmax,0),NiGd("shg"),size_(2*SHnmGuide::i_from_nm(nmax,nmax,nmax)){}
            
            core::typehash hash()const override{return core::typehash("SHtmnGuide_t");}
            static size_t i_from_nmt(int n,int m, trigenum t,int nmax);

            size_t ifromel(const Element & el) const override;
            
            Element elfromi(size_t i) const override;

            size_t fullsize()const override;
            
            private:
            using SHGuideMeta::nmax_;
                size_t size_=0;
        };

        /*!brief
         * SHnmtGuide holds a SH sorting scheme which varies fastest in trigonometric type, then order and degree is slowest. It includes order 0 sine coefficients
         */
        class SHnmtGuide: public SHGuideMeta, public NonIndexedGuide< nmtEl >{
        public:
            using Element=nmtEl;
            using NiGd=NonIndexedGuide<nmtEl>;
            SHnmtGuide():SHGuideMeta(),NiGd("shg"){}
            SHnmtGuide(const int nmax):SHGuideMeta(nmax,0),NiGd("shg"),nmax_(nmax){}
            int nmax()const{return nmax_;}
            int nmin()const{return 0;}
            core::typehash hash()const override{return core::typehash("SHnmtGuide");}
            size_t ifromel(const Element & el) const override{
                return i_from_nmt(std::get<0>(el),std::get<1>(el),std::get<2>(el));
            }
            
            Element elfromi(size_t i) const override;

            size_t fullsize()const override;
            
            inline static size_t i_from_nmt(int n, int m, trigenum t){
                return 2*((n*(n+1))/2+m)+t;
            }
        private:
            int nmax_=0;
        };






    ///@brief one-size fits all SHguide which contains an internal index
    class SHGuide:public IndexedGuide<nmtEl>{
        public:
            using Element=nmtEl;
            using IGd=IndexedGuide<nmtEl>;
            SHGuide():IGd("shg"){}
            SHGuide(std::string name):IGd(name){}  
            SHGuide(int nmax,std::string name="SHGuide"){
                *this=create_nmt(nmax);
            }
            SHGuide(const std::vector<Element> & invec):IGd("shg",invec){}
            using trig=trigenum;
            core::typehash hash()const override{return core::typehash("SHGuide_t");}

            int nmax()const{return nmax_;}
            int nmin()const{return nmin_;}
            //static constructors which produce predefined sequences
            static SHGuide create_nmt(int nmax,int nmin=0,bool tight=false);
        private:
            int nmax_=-1;
            int nmin_=-1;
    };

    }
}


#endif //FROMMLE_SHDIMENSION_HPP
