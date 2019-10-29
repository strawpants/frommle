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
    namespace guides {

        SHnmHalfGuide::SHnmHalfGuide():GuideBase("SHnmHalfGuide",typehash("SHnmHalfGuide").add(-1),0),nmax_(-1){}
        SHnmHalfGuide::SHnmHalfGuide(const int nmax):GuideBase("SHnmHalfGuide",typehash("SHnmHalfGuide").add(nmax),SHnmHalfGuide::i_from_nm(nmax,nmax,nmax)+1),nmax_(nmax){}

        SHtmnGuide::SHtmnGuide(const int nmax,const std::string  name) :SHGuideBase(name,typehash("SHtmnGuide").add(nmax),2*(SHnmHalfGuide::i_from_nm(nmax,nmax,nmax)+1),nmax,0){

        }

        size_t SHtmnGuide::idx(const int n, const int m, const SHGuideBase::trig t) const {
            size_t shft=(t==trig::C)?0:size_/2;
            return SHnmHalfGuide::i_from_nm(n,m,nmax_)+shft;
        }

        SHGuideBase::Element SHtmnGuide::operator[](const size_t idx) const {
            int n,m;
            trig t=(idx<size_/2)?trig::C:trig::S;
            size_t shft=(t==trig::C)?0:size_/2;
            std::tie(n,m)=SHnmHalfGuide::nm_from_i(idx-shft,nmax_);
            return std::make_tuple(n,m,t);
        }

        size_t SHGuideBase::idxfromEl(const std::tuple<int, int, SHGuideBase::trig> el) const {
            int n,m;
            trig t;
            std::tie(n,m,t)=el;
            return idx(n,m,t);
        } 

//implementation for SHnmtGuide

        SHnmtGuide::SHnmtGuide(const int nmax,const std::string  name) :SHGuideBase(name,typehash("SHnmtGuide").add(nmax),SHnmtGuide::i_from_nmt(nmax,nmax,trig::S)+1,nmax,0){

        }

        size_t SHnmtGuide::idx(const int n, const int m, const SHGuideBase::trig t)const{
            return SHnmtGuide::i_from_nmt(n,m,t);
        }

        SHGuideBase::Element SHnmtGuide::operator[](const size_t idx) const {
            int n,m;
            
            trig t=(idx%2==0)?trig::C : trig::S;

            double ndbl=(std::sqrt(1.0+8*(idx/2))-1.0)/2.0;
            n=(int)(ndbl);
            m=(int)((ndbl-n)*(n+1));
            return std::make_tuple(n,m,t);
        }

        
		std::ostream &operator<<(std::ostream &os, SHGuideBase::Element const &el){
            int n,m;
            SHGuideBase::trig t;
            std::tie(n,m,t)=el;
            return os << n <<" " << m<< " " << t;
        }
    }
}


