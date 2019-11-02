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
#include "pybindings/numpyConverting.hpp"

namespace frommle{
    namespace guides{
        

        struct gvar_to_pGuide {
            static PyObject *convert(GuideRegistry::Gvar const  & tin) {
                auto guideptr=boost::apply_visitor(gvar_baseptr(),tin).get();
                return p::incref(p::object(guideptr).ptr());

            }
        };
        

        class gvar_to_ndarray: public boost::static_visitor<np::ndarray>{
        public:
            template<class T>
            np::ndarray operator()(T & gvar)const{
                using Element=typename T::element_type::Element;
                np::dtype dtype=np::dtype(p::object(Element()));            
                //create an numpy array
                p::tuple shape(gvar->size()); 
                np::ndarray py_array = np::empty(shape, dtype);
                //std::transform(gvar->begin(),gvar->end(),reinterpret_cast<Element*>(py_array.get_data()),[](const Element & elin){
                       //return p::object(elin);});
                std::copy(gvar->begin(),gvar->end(),reinterpret_cast<Element*>(py_array.get_data())); 
                return py_array;
            }


        };

        //struct gvar_to_ndarray {
            //static PyObject *convert(GuideRegistry::Gvar const  & tin) {
                //auto ndarr=boost::apply_visitor(gvar_baseptr(),tin).get();
                //return p::incref(p::object(guideptr).ptr());

            //}
        //};


        template<class ...T>
            struct GuidePackAppendOverride;
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<GuideTlist<T,Types...>>: public virtual GauxMembers<GuideTlist<T,Types...>>, public GuidePackAppendOverride<Types...>{
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            GuidePackPtr append(const T & guide)const override{
               return this->get_override("append")(guide);
            }
            };
        
        
        template<class T, class ... Types>
            struct GuidePackAppendOverride<T,Types...>:public virtual GauxMembers<T,Types...>,public GuidePackAppendOverride<Types...>{
                public:
            using next=GuidePackAppendOverride<Types...>;
            using next::append;
            GuidePackPtr append(const T & guide)const override{
               return this->get_override("append")(guide);
            }
        
            };

        template<class T>
            struct GuidePackAppendOverride<T>: public virtual GauxMembers<T>{
                GuidePackPtr append(const T & guide)const override{
               return this->get_override("append")(guide);
            }
        
            };
        
        using GPWrapOverride=GuidePackAppendOverride<GuideRegistry>;

        struct GuidePackWrapper : public GuidePackBase, public GPWrapOverride, p::wrapper<GuidePackBase> {
        public:
            GuidePackBase::const_iterator begin()const{
                 
                this->get_override("begin");
            }
            
            GuidePackBase::const_iterator end()const{
                 
                this->get_override("end");
            }
            
            size_t num_elements()const override{
                return this->get_override("num_elements")();
            }
            
            int nDim()const{
                return this->get_override("nDim")();
            }
            
            const GuideBasePtr operator[](const int i)const override
            {
                return  this->get_override("operator[](const int)const")(i);
            
            }
            
            Gvar & gv(const int i)override{
                return  this->get_override("gv")(i);
            }
            const Gvar & gv(const int i)const override{
                return  this->get_override("gv")(i);
            }

            //return a numpynarray from iterating ove the elements 
            template<int n>
            static np::ndarray coords(const GuidePackDyn<n> &gpin, int i){
                //return np::array(p::make_tuple(0,1,2));
                return boost::apply_visitor(gvar_to_ndarray(),gpin.gv(i));

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
            GuidePackPtr (gapp::*appf)(const T &)const=&gapp::append;
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T, class ... Types>
        struct RegisterMembers<T,Types...> {
            static boostpyGP & regapp(boostpyGP & gpin){
            using gapp=GauxMembers<T,Types...>;
            //function pointer to pure virtual function
            GuidePackPtr  (gapp::*appf)(const T &)const=&gapp::append;
            
            return RegisterMembers<Types...>::regapp(gpin.def("append",p::pure_virtual(appf)));


            }
        };

        template<class T>
        struct RegisterMembers<T> {
            static boostpyGP & regapp(boostpyGP & gpin){

            //function pointer to pure virtual function
            using gapp=GauxMembers<T>;
            GuidePackPtr  (gapp::*appf)(const T &)const=&gapp::append;
            
            return gpin.def("append",p::pure_virtual(appf));


            }
        };

        template<int n>
        struct register_dyngpack{
            register_dyngpack(){
                p::class_<GuidePackDyn<n>,p::bases<GuidePackBase>>(std::string("GuidePack").append(std::to_string(n)).c_str())
                        .def("shape",&GuidePackDyn<n>::extent)
                        .def("coords",&GuidePackWrapper::coords<n>);
                //alos register a shared_ptr convrsion

                p::register_ptr_to_python< std::shared_ptr<GuidePackDyn<n>> >();
                //recursively call the next version to register
                register_dyngpack<n-1>();
            }

        };

        template<>
        struct register_dyngpack<-1>{
            register_dyngpack() {
                //the recursion stops here
            }
        };





        void registerGuidePack(){
            p::to_python_converter<GuideRegistry::Gvar, gvar_to_pGuide> ();

            //set up some pointers to the const and non const versions of the virtual [] operator
            //const GuideRegistry::Gvar & (GuidePackBase::*cget)(const int)const =&GuidePackBase::operator[];
            const GuideBasePtr (GuidePackBase::*cget)(const int)const =&GuidePackBase::operator[];

            
            RegisterMembers<GuideRegistry>::regapp(
                    boostpyGP("GuidePackBase",p::no_init)
                    .def("ndim",p::pure_virtual(&GuidePackBase::nDim))
                    .def("__getitem__",p::pure_virtual(cget))
                    .def("__iter__",p::range(&GuidePackBase::begin,&GuidePackBase::end))
                    .def("num_elements",p::pure_virtual(&GuidePackBase::num_elements))
                    );
            p::register_ptr_to_python< std::shared_ptr<GuidePackBase> >();

            //register all dynamic guidepacks up to dimension 10
            register_dyngpack<10>();



//            p::class_<GuidePackDyn<0>,p::bases<GuidePackBase>>("GuidePack0");
//            p::class_<GuidePackDyn<1>,p::bases<GuidePackBase>>("GuidePack1");
//            p::class_<GuidePackDyn<2>,p::bases<GuidePackBase>>("GuidePack2");
//            p::class_<GuidePackDyn<3>,p::bases<GuidePackBase>>("GuidePack3");
//            p::class_<GuidePackDyn<4>,p::bases<GuidePackBase>>("GuidePack4");
//            p::class_<GuidePackDyn<5>,p::bases<GuidePackBase>>("GuidePack5");
//            p::class_<GuidePackDyn<6>,p::bases<GuidePackBase>>("GuidePack6");
        }
    }
}

