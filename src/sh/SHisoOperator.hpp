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
        SHisoOperator(const int nmax):GOpDiag(),kernel_(nmax+1){}
        SHisoOperator(std::vector<T> kernel):GOpDiag(),kernel_(kernel){}
//        SHisoOperator(std::vector<T> kernel ):GOpDiag(),kernel_(kernel){}

        void fwdOp(const GArrayDyn<T,2> & gin, GArrayDyn<T,2> & gout) override {
            //expand the  kernel into the requested diagonal
            if (!gpo_){

                gpo_=gin.gp().strip();

            }else if (gpo_->at(0) != gin.gp()[0]){
                //check for consistency
                THROWINPUTEXCEPTION("Guides of the diagonal matrix are not consistent");

            }
            expandkernel();

           //call forward function of the base class
            GOpDiag::fwdOp(gin,gout);
        }


    private:
        ///@brief expand the degree dependent kernel in a full diagonal
        void expandkernel(){

            if (! gpo_){
                THROWINPUTEXCEPTION("Calling expand kernel without specifying a guide, don't know how to expand");
            }

            //quick return if a diagonal with the same size already exists
            if(diag_.diagonal().size() == gpo_->at(0)->size()){
                return;
            }

            auto shg=gpo_->template as< guides::SHGuideBase >(0);
            if (shg->nmax() > nmax()){
                THROWINPUTEXCEPTION("Requested nmax is larger than supported by the kernel");
            }
            //set diagonal matrix
            diag_=eigd(gpo_->at(0)->size());

            int n,m;
            guides::SHGuideBase::trig t;
            size_t i=0;
            for(const auto & tnm:*shg){
                std::tie(n,m,t)=tnm;
                diag_.diagonal()[i]=kernel_[n];
                ++i;
            }
        }

        inline int nmax()const{return kernel_.size()-1;}
        std::vector<T> kernel_{};
    };


    }

}

#endif //FROMMLE_SHISOOPERATOR_HPP
