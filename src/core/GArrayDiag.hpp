/*! \file GArrayDiagonal.hpp
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

#include "core/GArrayBase.hpp"
#include "core/Exceptions.hpp"

#ifndef FROMMLE_GARRAYDIAGONAL_HPP
#define FROMMLE_GARRAYDIAGONAL_HPP
namespace frommle{
    namespace core{
        ///@brief contains a diagonal array
        template<class T>
        class GArrayDiag:public GArrayBase<T,2>{
        public:
            using GAB=GArrayBase<T,2>;
            using eigd=typename Eigen::DiagonalMatrix<T,Eigen::Dynamic,Eigen::Dynamic>;
            using arr=boost::multi_array_ref<T, 1>;
            using const_arr=boost::const_multi_array_ref<T, 1>;
            using GAB::gpp;
            using GAB::gp;
            using typename GAB::gp_ptr_t;

            GArrayDiag():GAB("diag"){}

            ///@brief constructor which uses the same guide for both dimensions (it copies the internal guide)
            template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<1>, GP>::value, int> ::type = 0>
            GArrayDiag(GP guidepack):GAB(guidepack.append(guidepack.gv(0))),eig_(gpp()->at(0)->size()){}

            GArrayDiag(gp_ptr_t guidepackptr): GAB(guidepackptr){}
            GArrayDiag(const GArrayDiag & in):GAB(in.gp_),eig_(in.eig_){}

            core::typehash hash()const override{return core::typehash("GArDiag_t");}
            //returns the diagonal matrix as an eigen diagonal matrux
            eigd & eig(){return eig_;}
            const eigd & eig()const{return eig_;}

            arr diagonal(){
                auto ext=gp().strip()->extent();
                return arr(eig_.diagonal().data(),ext);
            }
            const_arr diagonal()const{
                auto ext=gp().strip()->extent();
                return const_arr(eig_.diagonal().data(),ext);
            }
            
            GArrayBase<T,2> &operator=(const T scalar)override{
               eig_.diagonal().array()=scalar;
               return *this;
            }

            
            ///@brief add two diagnonal matrices dn returns a new one
            GArrayDiag operator +(const GArrayDiag & gdiagin)const{
                matchGuides(gdiagin);
                auto gdiaout=GArrayDiag(*this);
                gdiaout+=gdiagin;
                return gdiaout;
            }

            ///@brief adds a diagonal matrix to the current one
            GArrayDiag & operator +=(const GArrayDiag & gdiagin){
                matchGuides(gdiagin);
                eig_.diagonal()+=gdiagin.eig_.diagonal();
                return *this;
            }

            ///@brief subtracts two diagnonal matrices dn returns a new one
            GArrayDiag operator -(const GArrayDiag & gdiagin)const{
                matchGuides(gdiagin);
                auto gdiaout=GArrayDiag(*this);
                gdiaout-=gdiagin;
                return gdiaout;
            }

            ///@brief subtracts a diagonal matrix from the current one
            GArrayDiag & operator -=(const GArrayDiag & gdiagin){
                matchGuides(gdiagin);
                eig_.diagonal()-=gdiagin.eig_.diagonal();
                return *this;
            }


        private:
            ///@breif check whether the guide of the input agrees with the current object
            void matchGuides(const GArrayDiag & gdiagin)const{
                if ( *(gp()[0]) != *(gdiagin.gp()[0])){
                    THROWINPUTEXCEPTION("Input guides don't agree ");
                }
            }
            //internal data is contained within an eigen matrix
            eigd eig_{};
        };


    }
}
#endif //FROMMLE_GARRAYDIAGONAL_HPP
