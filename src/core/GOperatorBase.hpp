/*! \file
 \brief Holds the base for the  Operator class
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

#include <string>
#include <cassert>
#include <tuple>
#include "GArrayBase.hpp"
#ifndef SRC_CPP_OPERATORBASE_HPP_
#define SRC_CPP_OPERATORBASE_HPP_


namespace frommle {
namespace core {

/*!
 * \brief Abstract base class for describing an operator mapping from one dimension in the other dimensions
 * This class allows the storing of complex functional operators
 * Derived classes of this operator can then be passed to generic functions
 * Derived classes are expected to provide the following functionality:
 * -# Implement the functional operator
 * -# Allow serialization: i.e. allows an operator to be stored in a file
 */
    class GOperatorBase {
    public:
        virtual ~GOperatorBase() {
        }
//        GOperatorBase( ){}
        GOperatorBase(const std::string & name= "Goperator" ):name_(name){}
        inline std::string name()const{return name_;}
        void setName(const std::string & name){
            name_=name;
        }
    private:
        std::string name_="GOperator";
    protected:

    };


    template<class T, int no,int ni>
    class GOperatorDyn{
    public:
        static const int ndim_o=no;
        static const int ndim_i=ni;
        using gin_t=GArrayDyn<T,ni>;
        using ginplus_t=GArrayDyn<T,ni+1>;

        using gout_t=GArrayDyn<T,no>;
        using goutplus_t=GArrayDyn<T,no+1>;
        virtual bool isLinear()const{return false;};
        GOperatorDyn(){}

        ///@brief constructs an operator and registers the output guides directly
        GOperatorDyn(guides::GuidePackDyn<ndim_o> && outGP):gpo_(std::make_shared<guides::GuidePackDyn<ndim_o>>(std::move(outGP))){

        }


        virtual void fwdOp(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> & gout)=0;

        
        virtual std::shared_ptr<GOperatorDyn> inv()const{
            
            THROWNOTIMPLEMENTED("Inverse operation is not implemented for this Operator");
        
        }
        
        ///@brief takes an array with extended dimension as input and applies the forward operator
        inline void operator()(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> & gout){fwdOp(gin,gout);}
//        {
//
//            THROWNOTIMPLEMENTED("Default forward operator with extended dimension is currently not implemented");
//        }

        ///@brief takes an array with extended dimension as input and applies the forward operator
        GArrayDyn<T,ndim_o+1> operator()(const GArrayDyn<T,ndim_i+1> & gin){
            if (!gpo_){
                THROWMETHODEXCEPTION("Operator's output guide is unintialized and must be set upon construction");
            }
//            auto gptmp=*(gpo_->append(gin.gp()[ndim_i]));
//            LOGINFO << gptmp[0]->size() << " "<< gptmp[1]->size() <<std::endl;
            GArrayDyn<T,ndim_o+1> gout(gpo_->append(gin.gpp()->gv(ndim_i)));
            this->operator()(gin,gout);
            return gout;
        }

        ///@brief takes an array as input and applies the forward operator
        void operator()(const GArrayDyn<T,ndim_i> & gin, GArrayDyn<T,ndim_o> & gout){
            //expand the dimension so the common virtual operator can be called (but refer to the same data)
            guides::IndexGuide iguide(1);
            auto gpi=std::dynamic_pointer_cast<guides::GuidePackDyn<ndim_i+1>>(gin.gp().append(iguide));
            auto gpo=std::dynamic_pointer_cast<guides::GuidePackDyn<ndim_o+1>>(gout.gp().append(iguide));
            auto gin_plus(gin.reshape(*gpi));
            auto gout_plus(gout.reshape(*gpo));
            this->operator()(gin_plus,gout_plus);
        }

        ///@brief version which allocates a new matrix
        GArrayDyn<T,ndim_o> operator()(const GArrayDyn<T,ndim_i> & gin) {
            if (!gpo_){
                THROWMETHODEXCEPTION("Operator's output guide is unintialized and must be set upon construction");
            }

            GArrayDyn<T,ndim_o> gout(*gpo_);
            //forward call to virtual function so this function does not need to have this implemented
            this->operator()(gin,gout);
            return gout;
        }

        ///@brief explicitly provides the Jacobian matrix of the linear operator
        void jacobian(const guides::GuidePackDyn<ndim_i> &gpin, GArrayDyn<T, ndim_o + ndim_i> &gout){
            //just call the forward operator with an input of ones
            if (!isLinear()){
                THROWMETHODEXCEPTION("Cannot compute linear jacobian from a non-linear operator without reference values");
            }

            //create a identity matrix
            GArrayDyn<T,ndim_i+1> gunit=GAconstruct<T>::eye(gpin);
            //call the forward operator with the identity matrix
            fwdOp(gunit,gout);
        }
        
        ///@brief explicitly provides the jacobian matrix of a linear operator
        GArrayDyn<T,ndim_o+ndim_i> jacobian(const guides::GuidePackDyn<ndim_i> & gpin){
            //create new GArray and forward call
            
            if (!gpo_){
                THROWMETHODEXCEPTION("Operator's output guide is unintialized and must be set upon construction");
            }
            auto tmpgpo=gpo_->append(gpin);
            GArrayDyn<T,ndim_o+ndim_i> gout(*tmpgpo);
            jacobian(gpin, gout);
            return gout;
        }



    protected:
        std::shared_ptr<guides::GuidePackDyn<ndim_o>> gpo_{};

    };


//    template<class T,class outGP, class inGP>
//class GOperator : public GOperatorDyn<T, outGP::ndim, inGP::ndim > {
//    public:
//        using gpout_t=outGP;
//        using gpin_t=inGP;
//        using gopbase=GOperatorDyn<T,outGP::ndim,inGP::ndim>;
//        using gopbase::ndim_o;
//        using gopbase::ndim_i;
//        using gopbase::operator();
//
//        GOperator() {}
//
//        GOperator(outGP &&gpout):gopbase(std::move(gpout)) {
//
//        }
//    protected:
//        using gopbase::gpo_;
//
//    };


}

}

#endif /* SRC_CPP_OPERATORBASE_HPP_*/
