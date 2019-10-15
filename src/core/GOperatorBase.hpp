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
#include "core/GuidePack.hpp"
//#include "GArrayBase.hpp"

#ifndef SRC_CPP_OPERATORBASE_HPP_
#define SRC_CPP_OPERATORBASE_HPP_


namespace frommle {
namespace core {

template<class T1,class T2>
    class GArray;

template<class ...Types>
class GuidePack;
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


    template<class T,class outGP, class inGP>
    class GOperator : public GOperatorBase  {
    public:
        using outgp=outGP;
        using ingp=inGP;
        GOperator() {}

        GOperator(outGP &&out, inGP &&in):gpo_(std::move(out)),gpi_(std::move(in)) {
        }
       
        template<class addG>
        using garout=core::GArray<T,typename guides::GPappend<addG,outGP>::right>;
        
        template<class addG>
        using garin=core::GArray<T,typename guides::GPappend<addG,inGP>::right>;
       
        ///@brief virtual function which return the adjoint (possibly linearized wrt g0)
        //virtual core::Garray<T,outGP,inGP> adjoint(const core::Garray<T,inGP> & g0)=0;


        template<class addG>
        garout<addG> apply(garin<addG> & inGar){
            //check for proper guide matching
            // possibly resort?
            // //...
            
            //extract the guidepack from the input
            //gpi_=inGar.g            
            //allocate garray for output
            garout<addG> gout(gpo_.append(inGar.template g<garin<addG>::ndim>())); 
            
            //call forward operator for matrices
            fwdOp(inGar.mat(),gout.mat());

            ////get the size of the auxiliary dimension
            //auto sz=inGar.template g<inGP::ndim>().size();
            //using range=boost::multi_array_types::index_range;
            //typename garin<addG>::index_gen indin;
            //typename garout<addG>::index_gen indout;
            //for(size_t i=0;i<sz;++i){
                //fwdOp(inGar[indin[range()][i]],gout[indout[range()][i]]);
            //}
            return gout;
            
        }
        


        using maroutv=typename boost::multi_array_ref<T,outGP::ndim+1>::template array_view<outGP::ndim>;
        using marinv=typename boost::multi_array_ref<T,inGP::ndim+1>::template array_view<inGP::ndim>;

        using marout=boost::multi_array_ref<T,outGP::ndim+1>;
        using marin=boost::multi_array_ref<T,inGP::ndim+1>;
        //the actual forward operator to be implemented int he derived classes
        virtual void fwdOp(const marin & in, marout & out)=0;     

    protected:
        outGP gpo_{};
        inGP gpi_{};


    };


}

}

#endif /* SRC_CPP_OPERATORBASE_HPP_*/
