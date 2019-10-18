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
#include "core/GPCookieCut.hpp"

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
        virtual bool isLinear()const{return false;};
        GOperatorDyn(){}

        ///@brief constructs an operator and registers the output guides directly
        GOperatorDyn(guides::GuidePackDyn<ndim_o> && outGP):gpo_(std::make_shared<guides::GuidePackDyn<ndim_o>>(std::move(outGP))){

        }


        ///@brief takes an array with extended dimension as input and applies the forward operator
        virtual void operator()(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> & gout){

            THROWNOTIMPLEMENTED("Default forward operator with extended dimension is currently not implemented");
        }

        ///@brief takes an array as input and applies the forward operator
        virtual void operator()(const GArrayDyn<T,ndim_i> & gin, GArrayDyn<T,ndim_o> & gout){

            THROWNOTIMPLEMENTED("Default forward operator is currently not implemented");
        }

        ///@brief explicitly provides the adjoint matrix of a linear operator
        void adjoint(const guides::GuidePackDyn<ndim_i> & gpin, GArrayDyn<T,ndim_o+ndim_i> & gout){
            //just call the forward operator with an input of ones
            if (!isLinear()){
                THROWMETHODEXCEPTION("Cannot compute linear adjoint from a non-linear operator use adjoint_linearized");
            }

            //create a unit diagonalized array by
//            auto gunit;///=GA_ones<T,ndim_i>(gpin);

            ///apply the forward operator to the unit diagonal array
            THROWNOTIMPLEMENTED("adjoint operator is curretnly not implemented");
        }

        ///@brief provides the adjoint matrix linearized wrt input g0
        void adjoint_linearized(const GArrayDyn<T,ndim_i> & g0, GArrayDyn<T,ndim_o+ndim_i> & gout ){

            THROWNOTIMPLEMENTED("linearized adjoint operator is currently not implemented");
        }


    protected:
        std::shared_ptr<guides::GuidePackDyn<ndim_o>> gpo_{};

    };


    template<class T,class outGP, class inGP>
class GOperator : public GOperatorDyn<T, outGP::ndim, inGP::ndim > {
    public:
        using outgp=outGP;
        using ingp=inGP;
        using gopbase=GOperatorDyn<T,outGP::ndim,inGP::ndim>;
        using gopbase::ndim_o;
        using gopbase::ndim_i;

        GOperator() {}

        GOperator(outGP &&gpout):gopbase(std::move(gpout)) {

        }
    protected:
        using gopbase::gpo_;

//        using rmerge_t=typename guides::GPCookieCut<outGP>::template with<inGP>::rmerge_t;
//        using adjoint_t=GArray<T,rmerge_t>;
//
//        template<class addG>
//        using goapp_t=typename guides::GPCookieCut<outGP>::template gapp_t<addG>;
////
//        template<class addG>
//        using giapp_t=typename guides::GPCookieCut<inGP>::template gapp_t<addG>;
//
//        template<class addG>
//                using garout=GArray<T,goapp_t<addG>>;
//
//
//        template<class addG>
//            using garin=GArray<T,giapp_t<addG>>;
//
//        template<class addG>
//                garout<addG> operator()(const garin<addG> & gin){
//                    //copy relevant input guides to gpin_
////                    auto frst=gin.gp().begin();
////                    std::copy(frst,frst+indim-1,gpi_.begin());
////
////                    //allocate a new array
////
////                    auto gout=garout<addG>(guides::GPstrip<outGP>::gapp(gpo_,gin.gp()[indim-2]));
//
//                }
//           ///@brief compute the adjoint of the operator
//           virtual adjoint_t adjoint(){
//
//           }
//
////        out=core::GArray<T,typename guides::GPappend<addG,inGP>::right>;
//
//        ///@brief virtual function which return the adjoint (possibly linearized wrt g0)
//        //virtual core::Garray<T,outGP,inGP> adjoint(const core::Garray<T,inGP> & g0)=0;
//
//
//
////        template<class addG>
////        garout<addG> apply(garin<addG> & inGar){
////            //check for proper guide matching
////            // possibly resort?
////            // //...
////
////            //extract the guidepack from the input
////            //gpi_=inGar.g
////            //allocate garray for output
////            garout<addG> gout(gpo_.append(inGar.template g<garin<addG>::ndim>()));
////
////            //call forward operator for matrices
////            fwdOp(inGar.mat(),gout.mat());
////
////            ////get the size of the auxiliary dimension
////            //auto sz=inGar.template g<inGP::ndim>().size();
////            //using range=boost::multi_array_types::index_range;
////            //typename garin<addG>::index_gen indin;
////            //typename garout<addG>::index_gen indout;
////            //for(size_t i=0;i<sz;++i){
////                //fwdOp(inGar[indin[range()][i]],gout[indout[range()][i]]);
////            //}
////            return gout;
////
////        }
//
//
//
//        using maroutv=typename boost::multi_array_ref<T,ndim_o+1>::template array_view<ndim_o>;
//        using marinv=typename boost::multi_array_ref<T,ndim_i+1>::template array_view<ndim_i>;
//
//        using marout=boost::multi_array_ref<T,ndim_o+1>;
//        using marin=boost::multi_array_ref<T,ndim_i+1>;
//        //the actual forward operator to be implemented int he derived classes
//        virtual void fwdOp(const marin & in, marout & out)=0;
//
//    protected:
//        outGP gpo_{};
//        inGP gpi_{};


    };


}

}

#endif /* SRC_CPP_OPERATORBASE_HPP_*/
