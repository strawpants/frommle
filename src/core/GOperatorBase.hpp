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
#include <assert.h>
#include <tuple>
#ifndef SRC_CPP_OPERATORBASE_HPP_
#define SRC_CPP_OPERATORBASE_HPP_


namespace frommle {


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
private:
protected:

};


	template<class outGuide, class inGuide>
	class GOperator:public GOperatorBase {
    public:
        using guide_t=std::tuple<outGuide,inGuide>;
        GOperator(){}
        GOperator(outGuide && out,inGuide &&in){
            guides_=std::make_tuple(out,in);
        }

        template<int n>
        typename std::tuple_element<n,guide_t>::type & g(){
            assert(n<3);
            return std::get<n>(guides_);
        }

        template<int n>
        const typename std::tuple_element<n,guide_t>::type & g()const {
            assert(n<3);
            return std::get<n>(guides_);
        }


    protected:
    private:
        guide_t guides_={};

};




}

#endif /* SRC_CPP_OPERATORBASE_HPP_*/
