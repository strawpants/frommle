/*! \file
 \brief Tools making use of the visitor pattern 
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
#include "core/Exceptions.hpp"
#ifndef SRC_CORE_VISIT_HPP_
#define SRC_CORE_VISIT_HPP_
namespace frommle{
	namespace core{
		template<class ... T>
		struct typelist{};

	//class VisitableBase;

	//class VisitorBase{
	//public:
		//virtual void visit(VisitableBase * in)const=0; 

	//};

	//class VisitableBase{
		//public:
		//virtual void accept(const VisitorBase & vis)=0;
	//};


	//template<template<class> class BaseT,template<class> class DerivedT, class Try1,class ... TryOthers>
	//class specializer{};

	/////@brief specialize a templated class from BaseT to DerivedT if the T's match
	//template<template<class> class BaseT,template<class> class DerivedT, class Try1,class ... TryOthers>
	//class specializer<BaseT,DerivedT,typelist<Try1,TryOthers...>>:public VisitorBase{
		
		//public:
		//private:
			//void visit(VisitableBase & in){
				//specializer::convert(in);
			//}
			
			//static void convert(VisitableBase &in){
				
				//auto tmp=dynamic_cast<BaseT<Try1>*>(in);
				//if(tmp){
				   ////yeah, success let's proceed by specializing the input type
					//in=DerivedT<Try1>(in);
				//} else{
					////no success try the next type
					//specializer<BaseT,DerivedT,TryOthers...>::convert(in); 
				//}
			//}
	//};


	/////@brief specialize a templated class from BaseT to DerivedT if the T's match
	//template<template<class> class BaseT,template<class> class DerivedT, class TryLast>
	//class specializer<BaseT,DerivedT,typelist<TryLast>>:public VisitorBase{
		
		//public:
		//private:
			//void visit(VisitableBase & in){
				//specializer::convert(in);
			//}
			
			//static void convert(VisitableBase &in){
				
				//auto tmp=dynamic_cast<BaseT<TryLast>*>(in);
				//if(tmp){
				   ////yeah, success let's proceed by specializing the input type
					//in=DerivedT<TryLast>(in);
				//} else{
					////no success -> fail
                    //THROWINPUTEXCEPTION("No more casting possibilities");
				//}
			//}
	//};


	}
}		
#endif
