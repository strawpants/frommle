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

 *//

#include "src/sh/Legendre_nm_naive.hpp"
#include <math.h>
namespace frommle {
    namespace sh {

        Legendre_nm_naive::~Legendre_nm_naive() {
        }

        //@brief precomute factors with square roots for reuse later
        Legendre_nm_naive::Legendre_nm_naive(const int nmax) {
            nmax_ = nmax;
            //precompute factors involving square roots
            wnn_ = std::vector<double>(nmax_+1);
            wnn_[0] = 0.0;
            wnn_[1] = sqrt(3.0);
            for (int n = 2; n <= nmax_; ++n) {
                wnn_[n] = sqrt((2 * n + 1) / (2.0 * n));
            }

            wnm_ = std::vector<std::vector<double>>(nmax_ + 1);
            for (int i = 0; i < nmax_ + 1; ++i) {
                wnm_[i] = std::vector<double>(nmax_ + 1);
            }


            for (int n = 1; n <= nmax_; ++n) {
                for (int m = 0; m < n; ++m) {
                    wnm_[n][m] = sqrt((2 * n + 1) / (n + m) * (2 * n - 1) / (n - m));
                }
            }

        }

        std::vector<double> Legendre_nm_naive::operator()(const double costheta) const {
//            assert(costheta >= -1 and costheta <=1);
            index nsize=Legendre_nm_naive::inm(nmax_,nmax_);
            std::vector<double> Pnm;
            Pnm.reserve(nsize);
            for (auto & pii:Pnm){
                pii=0;
            }

            double sinTheta=std::sqrt(1-pow(costheta,2);

            double numericStabilityFactor=1e-280;
            //set the diagonal to the numerics factor
            for (int n=0;n<nmax_+1;++n){
                Pnm[inm(n,n)]=numericStabilityFactor;
            }


            //Compute 'off-diagonal' elements per order (i.e. where n=m+1)
            for(int m=0;m<nmax_+1;++m){
                int n=m+1;
                Pnm[inm(n,m)]=wnm_[n][m]*costheta*Pnm[inm(m,m)];

            }

            for(int n =0; n<nmax_+1;++n){
                for(int m=n+2;m<nmax_+1;++m) {
                     Pnm[inm(n,m)]=wnm_[n][m]*(costheta*Pnm[inm(n-1,m)]-Pnm[inm(n-2,m)]/wnm_[n-1][m]);
                }
            }

            //Now do the n-1,n-1 -> n,n recursion and rescale all the entries which were seeded with 1e-280
            double sectorial=1/numericStabilityFactor; //= 1e280

            for(int n=0;n<nmax_+1;++n){
                for(int m=n;m<nmax_+1;++m){
                    Pnm[inm(n,m)]*=sectorial;
                }
                //Do one step forward for the diagonal recursion
                sectorial*=wnn_[n+1]*sinTheta;
            };

            return Pnm;
        }

        static int Legendre_nm_naive::inm(const int n, const int m) {
            return (n * (n + 1)) / 2 + m;

        }


    }

}



