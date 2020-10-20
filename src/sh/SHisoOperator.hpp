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

#include "core/GOperatorDiag.hpp"
#include "sh/SHGuide.hpp"

#ifndef FROMMLE_SHISOOPERATOR_HPP
#define FROMMLE_SHISOOPERATOR_HPP
namespace frommle{
    namespace sh{

    ///@brief a class which holds an isotropic kernel (i.e. Green's function) in the spherical harmonic domain
    template<class T>
    class SHisoOperator:public core::GOperatorDiag<T>{
    public:
        using GOpDiag=core::GOperatorDiag<T>;
        using GOpDiag::diag_;
        using GOpDiag::gpo_;
        using typename GOpDiag::gpo_t;
        using typename GOpDiag::eigd;
        using typename GOpDiag::inverse;
        SHisoOperator(std::string name="shop"):GOpDiag(name){}
        //SHisoOperator(const int nmax):GOpDiag(guides::GuidePackDyn<1>(guides::SHnmtGuide(nmax))),kernel_(nmax+1){}
        SHisoOperator(std::vector<T> kernel, std::string name="shop"):GOpDiag(guides::GuidePackDyn<1>(guides::SHnmtGuide(kernel.size()-1)),name),kernel_(kernel){
            expandkernel<guides::SHnmtGuide>();
        }

        template<class SHG>
        SHisoOperator(std::vector<T> kernel, const SHG & guide, std::string name="shop"):GOpDiag(guides::GuidePackDyn<1>(guide),name),kernel_(kernel){
            expandkernel<SHG>();
        }

        //SHisoOperator(std::string name,std::vector<T> kernel):GOpDiag(guides::GuidePackDyn<1>(guides::SHnmtGuide(kernel.size()-1))),kernel_(kernel){
        //}
        core::typehash hash()const override{return core::typehash("SHisop_t");}
        inline int nmax()const{return kernel_.size()-1;}
    private:

        ///@brief expand the degree dependent kernel in a full diagonal
        template<class SHG>
        void expandkernel(){



            auto shg=gpo_->template as<SHG>(0);
            if (shg->nmax() > nmax()){
                THROWINPUTEXCEPTION("Requested nmax is larger than supported by the kernel");
            }

            int n,m;
            guides::trigenum t;
            size_t i=0;
            for(const auto & tnm:*shg){
                std::tie(n,m,t)=tnm;
                diag_.eig().diagonal()[i]=kernel_[n];

                ++i;
            }
        }
        std::vector<T> kernel_{};
    };


    }

}

#endif //FROMMLE_SHISOOPERATOR_HPP
