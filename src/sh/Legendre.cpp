/*! \file
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
 */

#include "sh/Legendre.hpp"

namespace frommle{
namespace sh{
    ///@brief a class which computes and caches a unnormalized Legendre polynomial
template<class ftype>
            Legendre<ftype> & Legendre<ftype>::set(const ftype costheta){
                assert(nmax_ >0);
                ftype pnmin1=costheta;
                ftype pnmin2=1;
                ftype pn;
                pn_[0]=pnmin2;
                pn_[1]=pnmin1;

                for(int n=2;n<=nmax_;++n){
                   pn=((2*n-1)*costheta*pnmin1-(n-1)*pnmin2)/static_cast<ftype>(n);
                   pnmin2=pnmin1;
                   pnmin1=pn;
                   pn_[n]=pn;
                }

                return *this;
            }
            
            //Legendre<ftype> & Legendre<ftype>::set(const ftype costheta){
                //assert(nmax_ > 1);
                //PCache<ftype> P(costheta);
                //pn_[0]=P.l2;
                //pn_[1]=P.l1;
                //pn_[2]=P.l;

                //for(int n=3;n<=nmax_;++n){
                   //P.l=((2*n-1)*costheta*P.l1-(n-1)*P.l2)/static_cast<ftype>(n);
                   //P.l2=P.l1;
                   //P.l1=P.l;
                   //pn_[n]=P.l;
                //}

                //return *this;
            //}

        template
        class Legendre<double>;

        template
        class Legendre<long double>;
}

}


