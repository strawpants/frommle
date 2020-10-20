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
#include <boost/python/return_internal_reference.hpp>
#include "core/GOperatorBase.hpp"
#include "core/GOperatorDiag.hpp"

namespace p = boost::python;


namespace frommle{
    namespace core{

        template<class T, int ndim_o, int ndim_i>
        class GOperatorWrapper: public GOperatorDyn<T,ndim_o,ndim_i>,public p::wrapper<GOperatorDyn<T,ndim_o,ndim_i>>{
        public:
        
        void fwdOp(const GArrayBase<T,ndim_i+1> & gin, GArrayBase<T,ndim_o+1> & gout)  override  {
        if( auto fwdop=this->get_override("__call__")) {
            //call the python __call__ method of this class
            auto gtmp=fwdop(gin);
//            auto shp=p::extrac
            auto goutptr= gout.template as<GArrayDense<T,ndim_o+1>*>();

        }else{

//            GOperatorDyn<T,ndim_o,ndim_i>::fwdOp(gin,gout);

        }
    }
       



        };

    template<class T, int no, int ni>
        struct register_Goperator{
            using Gop=GOperatorDyn<T,no,ni>;
            static void reg(const std::string & basename){
                

                std::shared_ptr<GArrayBase<T,2>>  (Gop::*adjointf)(const guides::GuidePackDyn<1> & )=&Gop::jacobian;

                p::class_<GOperatorWrapper<T,no,ni>,p::bases<Frommle>,boost::noncopyable>(std::string(basename).append(std::to_string(no)).append("d_to_").append(std::to_string(ni)).append("d").c_str())
                    .def("jacobian",adjointf);
            }
    
    };
 
    template<class T>
struct register_GoperatorDiag{
    static void reg(const std::string & basename){
                const GArrayDiag<T> & (GOperatorDiag<T>::*cgdiag)( )const=&GOperatorDiag<T>::gdiag;
        
                p::class_<GOperatorDiag<T>,p::bases<GOperatorDyn<T,1,1>>>(basename.c_str())
            .def(p::self+p::self)
            .def(p::self+=p::self)
            .def(p::self-p::self)
            .def(p::self-=p::self)
            .def("gdiag",cgdiag,p::return_value_policy<p::copy_const_reference>());
            //.def("mat",&GOperatorDiag<T>::mat,p::return_value_policy<p::copy_const_reference>());
    }
};

    void registerGOperators(){
    ///Register the operator base class 
        //p::class_<GOperatorBase,p::bases<Frommle>>("GOperatorBase").def(p::init<p::optional<std::string>>());

        register_Goperator<double,1,1>::reg("GOperator_float64");

        register_GoperatorDiag<double>::reg("GOperatorDiag_float64");
    }


    }
}
