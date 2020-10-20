/*! \file GOperatorDiag.hpp
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

 Created by Roelof Rietbroek,  2019

 */


#include "core/GOperatorBase.hpp"
#include "core/GArrayDiag.hpp"

#ifndef FROMMLE_DIAGGOPERATOR_HPP
#define FROMMLE_DIAGGOPERATOR_HPP

namespace frommle{
    namespace core{

        ///@brief Holds a diagonal matrix to be used as linear operator
        template<class T>
        class GOperatorDiag:public GOperatorDyn<T,1,1>{
        public:
            using GOp=GOperatorDyn<T,1,1>;
            using GOp::gpo_;
            using typename GOp::gpo_t;
            using GOp::operator();
            using garr=GArrayDiag<T>;
            using eigd=typename garr::eigd;
            GOperatorDiag(std::string name="diagop"):GOp(name){}
            GOperatorDiag(guides::GuidePackDyn<1> gpo, std::string name="diagop"):GOp(std::move(gpo),name),diag_(*gpo_){}

            GOperatorDiag( std::shared_ptr<guides::GuidePackDyn<1>> gpo, eigd diag,std::string name="diagop"):GOp(gpo,name),diag_(*gpo_){}
            GOperatorDiag(garr diag ,std::string name="diagop"):GOp(diag.gp().strip(),name),diag_(diag){}

            core::typehash hash()const override {return core::typehash("GOpDiag_t");} 
            GOperatorDiag inverse()const{
                return GOperatorDiag(gpo_,diag_.eig().inverse());
                
            }

            ///@brief returns a new diagonal operator which chains the input throughanother diagonal operator before applying the operator itself
            GOperatorDiag operator()(const GOperatorDiag & pipethrough)const{
                   eigd outdiag(gpo_->at(0)->size());
                   outdiag.diagonal().array()=diag_.eig().diagonal().array()*pipethrough.diag_.eig().diagonal().array();
                   return GOperatorDiag(pipethrough.gpp(),outdiag); 
            }
//            GOperatorDiag(guides::GuidePackDyn<1> gpo, eigd diag ):GOp(gpo),diag_(diag){}

//            virtual

//            ///@brief takes an array with extended dimension as input and applies the forward operator
//            GArrayDyn<T,2> operator()(const GArrayDyn<T,2> & gin){
//
//                //output will have the same gudies as the input
//                GArrayDyn<T,2> gout(gin.gp());
//                fwdOp(gin,gout);
//                return gout;
//            }

            void fwdOp(const GArrayBase<T,2> & gin, GArrayBase<T,2> & gout) override {

                    //some sanity checks
                    if (diag_.eig().diagonal().size() != gin.gp()[0]->size()){
                        THROWINPUTEXCEPTION("Stored diagonal size does not agree with input");
                    }

                    if (*(gout.gp()[0]) != *(gin.gp()[0])){
                        THROWINPUTEXCEPTION("Input and output dimension size does not agree");
                    }

                    auto ginptr=gin.template as<const GArrayDense<T,2>*>();

                    auto goutptr=gout.template as<GArrayDense<T,2>*>();
                    //let the eigen library do the math (multiple desne matrix with diagonal matrix)
                    goutptr->eig()=diag_.eig()*ginptr->eig();
                    
                    //Eigen::DiagonalMatrix<double,-1> dia(3);
                    //dia.diagonal()[0]=1;
                    //dia.diagonal()[1]=2;
                    //dia.diagonal()[2]=3;
                    
                    
                    //Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> mat(3,2);
                    //mat.fill(1.0);

                    //LOGINFO << dia.diagonal();
                    //LOGINFO << mat ;
                    //LOGINFO << dia*mat;
            }




            garr & gdiag(){return diag_;}
            const garr & gdiag()const{return diag_;}

            eigd & eig(){return diag_.eig();}
            const eigd & eig()const{return diag_.eig();}
//            GOperatorDiag inverse()const{return GOperatorDiag(*gpo_,eig_.inverse());}

            ///@brief allow the addition of two diagonal operators into a new one
            GOperatorDiag operator+(const GOperatorDiag & inadd)const{
                checkBothGuides(inadd);
                return GOperatorDiag(diag_+inadd.diag_);

            }
            
            ///@brief allow the addition of a diagonal operator to the current one
            GOperatorDiag & operator+=(const GOperatorDiag & inadd){
                checkBothGuides(inadd);
                diag_+=inadd.diag_;
                return *this;

            }

            ///@brief allow the subtraction of two diagonal operators into a new one
            GOperatorDiag operator-(const GOperatorDiag & insub)const{
                checkBothGuides(insub);
                return GOperatorDiag(diag_-insub.diag_);

            }
            
            ///@brief allow the addition of a diagonal operator to the current one
            GOperatorDiag & operator-=(const GOperatorDiag & insub){
                checkBothGuides(insub);
                diag_-=insub.diag_;
                return *this;

            }
        protected:
            garr diag_{};
        private:
            void checkBothGuides(const GOperatorDiag & in)const{
                if (!(gpo_ and in.gpo_)){
                    THROWINPUTEXCEPTION("One of the input Diagonal operators has an uninitialized diagonal");
                }
            }

        };


    }

}

#endif //FROMMLE_DIAGGOPERATOR_HPP
