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




#include <boost/python.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>
#include "core/GOperatorBase.hpp"

namespace p = boost::python;


namespace frommle{
    namespace core{

        template<class T, int ndim_o, int ndim_i>
        class GOperatorWrapper: public GOperatorDyn<T,ndim_o,ndim_i>,public p::wrapper<GOperatorDyn<T,ndim_o,ndim_i>>{
        public:
        
        void fwdOp(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> & gout)    {
        if(auto fwdop=this->get_override("fwdOp")){
            fwdop(gin,gout);
        }else{
           //call the overload from python
           default_fwdop(gin,gout);
        }
    }
       
        void default_fwdop(const GArrayDyn<T,ndim_i+1> & gin, GArrayDyn<T,ndim_o+1> & gout)    {
        
            //when we land we should have a python defined forward operator
            
            //get a python object of the current object
            auto self=p::object(p::ptr(this)); 
            //call the python __call__ method of this class
            gout=p::extract<GArrayDyn<T,ndim_o+1>>(self.attr("__call__")(gin)); 
        }



        };

    template<class T, int no, int ni>
        struct register_Goperator{
            using Gop=GOperatorDyn<T,no,ni>;
            static void reg(const std::string & basename){
                

                GArrayDyn<T,2>  (Gop::*adjointf)(const guides::GuidePackDyn<1> & )=&Gop::adjoint;

                p::class_<GOperatorWrapper<T,no,ni>,p::bases<GOperatorBase>,boost::noncopyable>(std::string(basename).append(std::to_string(no)).append("d_to_").append(std::to_string(ni)).append("d").c_str())
                    .def("adjoint",adjointf);
            }
    
    };

    void registerGOperators(){
    ///Register the operator base class 
    p::class_<GOperatorBase>("GOperatorBase").def(p::init<p::optional<std::string>>())
            .add_property("name",&GOperatorBase::name,&GOperatorBase::setName);
    
        register_Goperator<double,1,1>::reg("GOperator"); 
    
    }





    }
}
