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


            /*! brief returns a vectorized index for order sorted spherical harmonics (no trigonometric variable)
             * @param n input degree
             * @param m input order
             * @param nmax maximum degree to accomodate for
             * @return zero based index of the corresponding entry
             */
            size_t SHnmGuide::i_from_nm(const int n,const int m, const int nmax){
                assert(m<=n);
                assert(n<=nmax);
                return m*(nmax+1)-(m*(m+1))/2+n;
            }

            size_t SHnmGuide::ifromel(const Element & el) const{
                return i_from_nm(std::get<0>(el),std::get<1>(el),nmax_);
            }
            
            /*!brief retrieve the degree and order from a index sorted by order and degree (no trig variable
            *
            * @param idx input index (zero based)
            * @param nmax maximum degree for which has been allocated
            * @return a tuple containing the degree and order in the first and second element respectively
            */
            nmEl SHnmGuide::nm_from_i(const size_t idx, const int nmax){
                int m=(3.0+2*nmax)/2-std::sqrt(std::pow(3+2*nmax,2)/4.0-2*idx);
                int n=idx-(((m+1)*(m+2))/2+m*(nmax-m))+m+1;
                assert(m<=n);
                assert(n<=nmax);
                return std::make_tuple(n,m);

            }

            nmEl SHnmGuide::elfromi(size_t i) const {
                return nm_from_i(i,nmax_);
            }

            size_t SHnmGuide::fullsize()const {

                if (nmax_==-1){
                    return 0;
                }
                return SHnmGuide::i_from_nm(nmax_,nmax_,nmax_)+1;
            }



            /*! brief returns a vectorized index for order sorted spherical harmonics (no trigonometric variable)
             * @param n input degree
             * @param m input order
             * @param nmax maximum degree to accomodate for
             * @return zero based index of the corresponding entry
             */
        size_t SHtmnGuide::i_from_nmt(int n, int m, trigenum t,int nmax) {
            size_t shft=(t==trigenum::C)?0:SHnmGuide::i_from_nm(nmax,nmax,nmax);
            return SHnmGuide::i_from_nm(n,m,nmax)+shft;
        }
        
        nmtEl SHtmnGuide::elfromi(const size_t idx)const { 
            int n,m;
            trigenum t=(idx<size_/2)?trigenum::C:trigenum::S;
            size_t shft=(t==trigenum::C)?0:size_/2;
            std::tie(n,m)=SHnmGuide::nm_from_i(idx-shft,nmax_);
            return std::make_tuple(n,m,t);
        }

        size_t SHtmnGuide::ifromel(const Element & el) const{
            return i_from_nmt(std::get<0>(el),std::get<1>(el),std::get<2>(el),nmax_);
        }

        size_t SHtmnGuide::fullsize()const{
                if (nmax_==-1){
                    return 0;
                }
                return 2*SHnmGuide::i_from_nm(nmax_,nmax_,nmax_)+1;
         }


        //implementation for SHnmtGuide


        SHnmtGuide::Element SHnmtGuide::elfromi(size_t idx) const {
            int n,m;
            
            trigenum t=(idx%2==0)?trigenum::C : trigenum::S;

            double ndbl=(std::sqrt(1.0+8*(idx/2))-1.0)/2.0;
            n=(int)(ndbl);
            m=(int)((ndbl-n)*(n+1));
            return std::make_tuple(n,m,t);
        }


        size_t SHnmtGuide::fullsize()const{
                if (nmax_==-1){
                    return 0;
                }
                return 2*SHnmGuide::i_from_nm(nmax_,nmax_,nmax_)+1;
         }

        std::ostream &operator<<(std::ostream &os, nmtEl const &el){
            int n,m;
            trigenum t;
            std::tie(n,m,t)=el;
            return os << n <<" " << m<< " " << t;
        }
                
        std::ostream &operator<<(std::ostream &os, nmEl const &el){
            int n,m;
            std::tie(n,m)=el;
            return os << n <<" " << m;
        }
        
        ///@brief create an shguide where n is varying slowest, than m than the trig sign
        SHGuide SHGuide::create_nmt(int nmax, int nmin,bool tight){
            if(nmax < 0 || nmin < 0){
                THROWINPUTEXCEPTION("nmax and nmin must be larger or equal than 0");
            }

            SHGuide shg{"SHGuide"};
            shg.nmax_=nmax;
            shg.nmin_=nmin;
            for (int n=nmin;n<=nmax;++n){
                for(int m=0;m<=n;++m){
                        shg.emplace_back(n,m,trig::C);
                    if (m >0){
                        shg.emplace_back(n,m,trig::S);
                    }else if(!tight){
                        assert(m==0);
                        shg.emplace_back(n,m,trig::S);
                    }

                }
            }

        return shg;

        }


    }
}


