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

#include<vector>
#include "cassert"
#ifndef SRC_SH_LEGENDRE_HPP_
#define SRC_SH_LEGENDRE_HPP_

namespace frommle{
namespace sh{


    ///@brief a class which computes and caches a unnormalized Legendre polynomial
template<class ftype>
    class Legendre{
        public:
            
            Legendre(int nmax):nmax_(nmax),pn_(nmax+1){}
            Legendre & set(const ftype costheta);
            inline const ftype & operator[](size_t n)const{assert(n <= nmax_);return pn_[n];} 
            const std::vector<ftype> & get(){return pn_;}
        private:
            int nmax_=-1;
            std::vector<ftype> pn_{}; 
    };


}

}


#endif 
