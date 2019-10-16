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
    namespace guides{
        

        //struct gvar_to_pGuide {
            //static PyObject *convert(GuideRegistry::Gvar const  & tin) {
                //auto guideptr=boost::apply_visitor(gvar_baseptr(),tin).get();
                //return p::incref(p::object(guideptr).ptr());

            //}
        //};
        
        template<class ...T>
            struct GuidePackAppendOverride;
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<GuideTlist<T,Types...>>: public virtual GauxMembers<GuideTlist<T,Types...>>, public GuidePackAppendOverride<Types...>{
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
            };
        
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<T,Types...>:public virtual GauxMembers<T,Types...>,public GuidePackAppendOverride<Types...>{
                public:
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
        
            };

        template<class T>
            struct GuidePackAppendOverride<T>: public virtual GauxMembers<T>{
                GuidePackPtr append(T & guide)override{
               return this->get_override("append")(guide);
            }
        
            };
        
        using GPWrapOverride=GuidePackAppendOverride<GuideRegistry>;

        struct GuidePackWrapper : public GuidePackBase, public GPWrapOverride, p::wrapper<GuidePackBase> {
        public:


            size_t num_elements()const override{
                if (p::override numel = this->get_override("num_elements")){
                    return numel(); 
                }
                return this->GuidePackBase::num_elements();
            }
            size_t default_numelements()const{return this->GuidePackBase::num_elements();}
            
            int nDim()const{
                if (p::override nDimf = this->get_override("nDim")){
                    return nDimf(); 
                }
                return this->GuidePackBase::nDim();
            }
            int default_nDim()const{return this->GuidePackBase::nDim();}
            
            const GuideBasePtr operator[](const int i)const override
            {
                if (p::override fdispatch = this->get_override("operator[](const int)const")){
                    return fdispatch(i); // *note*
                }
                return this->GuidePackBase::operator[](i);
            
            }

            const GuideBasePtr default_getguide(const int i)const{
               return this->GuidePackBase::operator[](i);
            }
        
            //const GuideRegistry::Gvar & operator[](const int i)const override
            //{
                //if (p::override fdispatch = this->get_override("operator[](const int)const")){
                    //return fdispatch(i); // *note*
                //}
                //return this->GuidePackBase::operator[](i);
            
            //}

            //const GuideRegistry::Gvar &default_getguide(const int i)const{
               //return this->GuidePackBase::operator[](i);
            //}


        };
        using boostpyGP=p::class_<GuidePackWrapper,boost::noncopyable>;
        
        
        template<class ... T>
        struct RegisterMembers;


        template<class T, class ... Types>
        struct RegisterMembers<GuideTlist<T,Types...>> {
            static boostpyGP & regapp(boostpyGP & gpin){
            using gapp=GauxMembers<GuideTlist<T,Types...>>;
            //function pointer to pure virtual function
            GuidePackPtr (gapp::*appf)(T &)=&gapp::append;
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T, class ... Types>
        struct RegisterMembers<T,Types...> {
            static boostpyGP & regapp(boostpyGP & gpin){
            using gapp=GauxMembers<T,Types...>;
            //function pointer to pure virtual function
            GuidePackPtr  (gapp::*appf)(T &)=&gapp::append;
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T>
        struct RegisterMembers<T> {
            static boostpyGP & regapp(boostpyGP & gpin){

            //function pointer to pure virtual function
            using gapp=GauxMembers<T>;
            GuidePackPtr  (gapp::*appf)(T &)=&gapp::append;
            
            return gpin.def("append",p::pure_virtual(appf));


            }
        };


        void registerGuidePack(){
            //p::to_python_converter<GuideRegistry::Gvar, gvar_to_pGuide> ();

            //set up some pointers to the const and non const versions of the virtual [] operator
            //const GuideRegistry::Gvar & (GuidePackBase::*cget)(const int)const =&GuidePackBase::operator[];
            const GuideBasePtr (GuidePackBase::*cget)(const int)const =&GuidePackBase::operator[];

            
            RegisterMembers<GuideRegistry>::regapp(
                    boostpyGP("GuidePackBase",p::no_init)
                    .def("ndim",&GuidePackBase::nDim,&GuidePackWrapper::default_nDim)
                    .def("__getitem__",p::pure_virtual(cget))
                    .def("__iter__",p::iterator<const GuidePackBase>())
                    .def("num_elements",p::pure_virtual(&GuidePackBase::num_elements))
                    //.def("shape",&GuidePackWrapper::getshape)
                    );
            p::register_ptr_to_python< std::shared_ptr<GuidePackBase> >();

            p::class_<GuidePackDyn<0>,p::bases<GuidePackBase>>("GuidePack0");
            p::class_<GuidePackDyn<1>,p::bases<GuidePackBase>>("GuidePack1");
            p::class_<GuidePackDyn<2>,p::bases<GuidePackBase>>("GuidePack2");
            p::class_<GuidePackDyn<3>,p::bases<GuidePackBase>>("GuidePack3");
            p::class_<GuidePackDyn<4>,p::bases<GuidePackBase>>("GuidePack4");
            p::class_<GuidePackDyn<5>,p::bases<GuidePackBase>>("GuidePack5");
            p::class_<GuidePackDyn<6>,p::bases<GuidePackBase>>("GuidePack6");
        }
    }
}

