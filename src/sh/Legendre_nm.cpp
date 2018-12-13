/*! \file
 \brief Implementation code of (associated) Legendre functions
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

#include "sh/Legendre_nm.hpp"
#include <math.h>
#include <assert.h>
namespace frommle {
    namespace sh {


        template<class ftype>
        struct cacheEntry {
            ftype pnmin2 = 0.0;
            ftype pnmin1 = 0.0;
            ftype pn = 0.0;
            //note the sectorial is susceptable to underflow so a long double is used
//            long double sectorial = 0.0;
            ftype sectorial = 0.0;
        };


        //@brief precompute factors with square roots for reuse later
        template<class ftype>
        Legendre_nm<ftype>::Legendre_nm(const int nmax) {
            nmax_ = nmax;
            //precompute factors involving square roots
            wnn_ = std::vector<ftype>(nmax_ + 1);
            wnn_[0] = 0.0;
            wnn_[1] = sqrt(3.0);
            for (int n = 2; n <= nmax_; ++n) {
                wnn_[n] = sqrt((2 * n + 1) / (2.0 * n));
            }

            wnm_ = std::vector<ftype>(indxnm(nmax_, nmax_) + 1);
            for (int m = 0; m <= nmax_; ++m) {
                for (int n = m + 1; n <= nmax_; ++n) {
                    wnm_[indxnm(n, m)] = sqrt((2 * n + 1.0) / (n + m) * (2 * n - 1.0) / (n - m));
                }

            }

        }

        template<class ftype>
        std::vector<ftype> Legendre_nm<ftype>::operator()(const ftype costheta) const {
            assert(costheta >= -1.0 and costheta <= 1.0);

            std::vector<ftype> Pnm(indxnm(nmax_, nmax_) + 1);
            std::fill(Pnm.begin(), Pnm.end(), 0.0);


            ftype sinTheta = std::sqrt(1 - pow(costheta, 2));

            ftype numericStabilityFactor = 1e-280;

            //Loop over orders (slowly varying)
            size_t idx;
            cacheEntry<ftype> L1CacheEntry;


            //initial rescaling is 1e280
            L1CacheEntry.sectorial = 1.0 / numericStabilityFactor;


            Pnm[0] = 1.0; // or Pnm[0] = L1CacheEntry.sectorial*numericStabilityFactor

            for (int m = 0; m < nmax_; ++m) {
                idx = indxnm(m, m);
                L1CacheEntry.pnmin2 = numericStabilityFactor;

                //compute offdiagonal element
                L1CacheEntry.pnmin1 = wnm_[idx + 1] * costheta * L1CacheEntry.pnmin2;
                Pnm[idx + 1] = L1CacheEntry.pnmin1 * L1CacheEntry.sectorial;
                //loop over remaining degrees
                for (int n = m + 2; n <= nmax_ + 1; ++n) {
                    idx = indxnm(n, m);

                    L1CacheEntry.pn =
                            wnm_[idx] * (costheta * L1CacheEntry.pnmin1 - L1CacheEntry.pnmin2 / wnm_[idx - 1]);
                    //write value to output vector and shift entries in the cache
                    Pnm[idx] = L1CacheEntry.pn * L1CacheEntry.sectorial;
                    //shift entry to prepare for the next degree
                    L1CacheEntry.pnmin2 = L1CacheEntry.pnmin1;
                    L1CacheEntry.pnmin1 = L1CacheEntry.pn;
                }

                //Update the m+1 sectorial (applies n+1,n+1 <- n,n recursion)
                L1CacheEntry.sectorial *= wnn_[m + 1] * sinTheta;
                //also write the next sectorial to the output vector (scaled correctly)
                Pnm[indxnm(m + 1, m + 1)] = L1CacheEntry.sectorial * numericStabilityFactor;


            }
            return Pnm;
        }

        template<class ftype>
        std::vector<ftype> Legendre_nm<ftype>::d1at(const ftype costheta) const {
            return std::vector<ftype>();
        }


//explicit template initialization
        template
        class Legendre_nm<double>;

        template
        class Legendre_nm<long double>;
//        template class Legendre_nm<boost::multiprecision::float128>;

    }
}



