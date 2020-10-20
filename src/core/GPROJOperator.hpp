/*! \file
 \brief Hold an operator which does a projection
 \copyright Roelof Rietbroek 2020
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



#include "core/GOperatorBase.hpp"
#include <functional>
#ifndef SRC_CPP_PROJOPERATOR_HPP_
#define SRC_CPP_PROJOPERATOR_HPP_
namespace frommle{

    namespace core{

        ///@brief Projection matrix which maps the input on the output when a condition holds
        template<class T, class Element>
        class GSimpleProjection:public GOperatorDyn<T,1,1>{
        public:
            using GOp=GOperatorDyn<T,1,1>;
            using cfunc=std::function<bool (const & Element)>;
            GSimpleProjection( cfunc & func):GOp(),cond_(func){} 
            void fwdOp(const GArrayBase<T,2> & gin, GArrayBase<T,2>  &gout)override{
                //determine the output size
                std::vector<size_t> vidx{};
                size_t idx=0;
                for(auto const &el:gin.gp[0]){
                    if( cond_(el)){
                        vidx.push_back(idx);
                    }
                    ++idx;
                }



                
            }

            
        private:
           cfunc cond_{[](const & Element el){return true;};};

       };
    }
}
