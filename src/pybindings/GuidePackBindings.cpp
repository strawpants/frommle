/*! \file
 \brief contains python boost bninding aids for registering the GuidePack 
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
#include "core/GuidePack.hpp"

namespace p = boost::python;


namespace frommle{
    namespace py{
        
        class gvar_visitor: public boost::static_visitor<core::GuideBasePtr>{
            public:
                template<class T>
                core::GuideBasePtr operator()(T & gvar)const{
                    return std::static_pointer_cast<core::GuideBase>(gvar);
                }

        };

        struct gvar_to_pGuide {
            static PyObject *convert(core::GuideRegistry::Gvar const  & tin) {
                auto guideptr=boost::apply_visitor(gvar_visitor(),tin).get();
                return p::incref(p::object(guideptr).ptr());

            }
        };
        
        template<class ...T>
            struct GuidePackAppendOverride;
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<core::GuideTlist<T,Types...>>: public virtual core::GauxMembers<core::GuideTlist<T,Types...>>, public GuidePackAppendOverride<Types...>{
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            core::GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
            };
        
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<T,Types...>:public virtual core::GauxMembers<T,Types...>,public GuidePackAppendOverride<Types...>{
                public:
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            core::GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
        
            };

        template<class T>
            struct GuidePackAppendOverride<T>: public virtual core::GauxMembers<T>{
                core::GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
        
            };
        
        using GPWrapOverride=GuidePackAppendOverride<core::GuideRegistry>;

        struct GuidePackWrapper : public core::GuidePackBase, public GPWrapOverride, p::wrapper<core::GuidePackBase> {
        public:
            
            int nDim()const{
                if (p::override nDimf = this->get_override("nDim")){
                    return nDimf(); 
                }
                return this->core::GuidePackBase::nDim();
            }
            int default_nDim()const{return this->GuidePackBase::nDim();}
        
            const core::GuideRegistry::Gvar & operator[](const int i)const override
            {
                if (p::override fdispatch = this->get_override("operator[](const int)const")){
                    return fdispatch(i); // *note*
                }
                return this->core::GuidePackBase::operator[](i);
            
            }

            const core::GuideRegistry::Gvar &default_getguide(const int i)const{
               return this->core::GuidePackBase::operator[](i);
            }


        };
        using boostpyGP=p::class_<GuidePackWrapper,boost::noncopyable>;
        
        
        template<class ... T>
        struct RegisterMembers;


        template<class T, class ... Types>
        struct RegisterMembers<core::GuideTlist<T,Types...>> {
            static boostpyGP & regapp(boostpyGP & gpin){
            using gapp=core::GauxMembers<core::GuideTlist<T,Types...>>;
            //function pointer to pure virtual function
            core::GuidePackPtr (gapp::*appf)(T &)=&gapp::append; 
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T, class ... Types>
        struct RegisterMembers<T,Types...> {
            static boostpyGP & regapp(boostpyGP & gpin){
            using gapp=core::GauxMembers<T,Types...>;
            //function pointer to pure virtual function
            core::GuidePackPtr  (gapp::*appf)(T &)=&gapp::append; 
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T>
        struct RegisterMembers<T> {
            static boostpyGP & regapp(boostpyGP & gpin){

            //function pointer to pure virtual function
            using gapp=core::GauxMembers<T>;
            core::GuidePackPtr  (gapp::*appf)(T &)=&gapp::append; 
            
            return gpin.def("append",p::pure_virtual(appf));


            }
        };


        void registerGuidePack(){
            p::to_python_converter<core::GuideRegistry::Gvar, gvar_to_pGuide> ();

            //set up some pointers to the const and non const versions of the virtual [] operator
            const core::GuideRegistry::Gvar & (core::GuidePackBase::*cget)(const int)const =&core::GuidePackBase::operator[];

            RegisterMembers<core::GuideRegistry>::regapp(
                    boostpyGP("GuidePackBase",p::no_init).def("ndim",&core::GuidePackBase::nDim,&GuidePackWrapper::default_nDim).def("__getitem__",p::pure_virtual(cget),p::return_value_policy<p::copy_const_reference>())
                        .def("__iter__",p::iterator<const core::GuidePackBase>())
            );
            p::register_ptr_to_python< std::shared_ptr<core::GuidePackBase> >();
            p::class_<core::GuidePackDyn<0>,p::bases<core::GuidePackBase>>("GuidePack0");

            p::class_<core::GuidePackDyn<1>,p::bases<core::GuidePackBase>>("GuidePack1");



        }
    }
}

