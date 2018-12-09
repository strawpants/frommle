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

#include "sh/Legendre_nm_naive.hpp"
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
                    wnm_[n][m] = sqrt((2 * n + 1.0) / (n + m) * (2 * n - 1.0) / (n - m));
                }
            }

        }

        std::vector<std::vector<double>> Legendre_nm_naive::operator()(const double costheta) const {
//            assert(costheta >= -1 and costheta <=1);
            auto nsize=Legendre_nm_naive::inm(nmax_,nmax_);
            std::vector<std::vector<double>> Pnm(nmax_+1);
            for(auto & el:Pnm){
                el=std::vector<double>(nmax_+1);
                std::fill(el.begin(),el.end(),0);
            }


            double sinTheta=std::sqrt(1-pow(costheta,2));

            double numericStabilityFactor=1e-280;
            //set the diagonal to the numerics factor
            for (int n=0;n<nmax_+1;++n){
                Pnm[n][n]=numericStabilityFactor;
            }


            //Compute 'off-diagonal' elements per order (i.e. where n=m+1)
            for(int m=0;m<nmax_;++m){
                int n=m+1;
                Pnm[n][m]=wnm_[n][m]*costheta*Pnm[m][m];

            }

            for(int m =0; m<nmax_+1;++m){
                for(int n=m+2;n<nmax_+1;++n) {
                     Pnm[n][m]=wnm_[n][m]*(costheta*Pnm[n-1][m]-Pnm[n-2][m]/wnm_[n-1][m]);
                }
            }

            //Now do the n-1,n-1 -> n,n recursion and rescale all the entries which were seeded with 1e-280
            double sectorial=1/numericStabilityFactor; //= 1e280

            for(int m=0;m<nmax_+1;++m){
                for(int n=m;n<nmax_+1;++n){
                    Pnm[n][m]*=sectorial;
                }
                //Do one step forward for the diagonal recursion
                sectorial*=wnn_[m+1]*sinTheta;
            };

            return Pnm;
        }

        int Legendre_nm_naive::inm(const int n, const int m) {
            return (n * (n + 1)) / 2 + m;

        }

        std::vector<double> Legendre_nm_naive::d1at(const double costheta) const {
            return std::vector<double>();
        }


    }

}



