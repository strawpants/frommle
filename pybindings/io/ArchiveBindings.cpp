/*! \file ArchiveBindings.cpp
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

 Created by Roelof Rietbroek,  2019

 */


#include <boost/python.hpp>
#include <boost/python/slice.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/noncopyable.hpp>
#include "io/ArchiveBase.hpp"
#include "../core/numpyConverting.hpp"
namespace p = boost::python;


namespace frommle{
    namespace io {

        ///@brief Groupwrapper which is needed to allow dynamic python overloading
        class GroupWrapper:public Group,public p::wrapper<Group>{
        public:
            GroupWrapper():Group(){}
            GroupWrapper(std::string name):Group(name){}

            std::shared_ptr<VariableDyn> getVariable(const std::string &name) {

                //quyick return if a variable with the name already exists
                auto idx = findidx(name);
                if (idx != -1) {
                    return this->operator[](idx).ptr<VariableDyn>();
                }


                //else try to create a new one using the python overload
                if (p::override f = this->get_override("getVariable")) {
                    return f(name);
                } else {
                    //just tap into the virtual functions from the base class
                    return Group::getVariable(name);
                }
            }
       
            
            std::shared_ptr<VariableDyn> default_getVariable(const std::string & name) {
                return this->Group::getVariable(name);
            }


        private:
        };


        ///@brief Variablewrapper which is needed to allow dynamic python overloading
//        class VariableDynWrapper:public VariableDyn,public p::wrapper<VariableDyn> {
//        public:
//            VariableDynWrapper():VariableDyn(){}
//            VariableDynWrapper(const std::string name):VariableDyn(name){}
//
//            void setValue(std::shared_ptr<const core::Frommle> ptr){
//                if (p::override setf = this->get_override("__setitem__")) {
//                    setf(p::slice(),ptr);
//
//                }else{
//                    //tap into the virtual resolution of the base
//                    VariableDyn::setValue(ptr);
//                }
//            }
//
//        };

        ///@brief Variablewrapper which is needed to allow dynamic python overloading
        template<class T>
        class VariableWrapper:public Variable<T>,public p::wrapper<Variable<T>>{
        public:
            VariableWrapper():Variable<T>(){}
            VariableWrapper(std::string name):Variable<T>(name){}
            void setValue(const core::Hyperslab<T> & hslab){
                //we need to use python sliceig

                if (p::override setf = this->get_override("__setitem__")) {

                        if(hslab.ndim() ==1){
                            p::slice slc=py::slice_from_hslab<T>(hslab);
                            setf(slc,hslab);
                        }else{
                            //use a list of slices
                            p::list slc=py::slices_from_hslab<T>(hslab);
                            setf(slc,hslab);
                        }
                }else{
                    Variable<T>::setValue(hslab);
                }
            }

//            void default_setValue(const core::Hyperslab<T> & hslab){
//                Variable<T>::setValue(hslab);
//            }

//            static np::ndarray getitem(PyObject* varptr, const  p::slice & slice){
//                auto res=p::call_method<np::ndarray>(varptr,"__getitem__",slice);
//                return res;
//            }

//            //let's overload this function so a derived type in python can use it's __getitem__ function instead of getValue
//            void getValue(core::Hyperslab<T> & hslab) {
//                if (p::override getf = this->get_override("__getitem__")) {
//                    //sucess: use this function and convert requested hyperslab to a tuple of python slices
//                        if(hslab.ndim() ==1){
//                            p::slice slc=py::slice_from_hslab<T>(hslab);
//                            np::ndarray ndout=getf(slc);
//                            hslab.setdata(reinterpret_cast<T*>(ndout.get_data()));
//                        }else{
//                            //use a list of slices
//                            p::list slc=py::slices_from_hslab<T>(hslab);
//                            np::ndarray ndout=getf(slc);
//                            hslab.setdata(reinterpret_cast<T*>(ndout.get_data()));
//                        }
////                    //set data storage location
//                }else{
////                    //no python overload: just forward call to derived c++ type
//                    Variable<T>::getValue(hslab);
////
//                }
//            }
            ///@brief either use the python implmentation of the derived type or the getValue function of c++


            PyObject* getitem(const p::slice & slice){
                //call the getitem method
                
                if (p::override getf=this->get_override("__getitem__")){
                    auto res=getf(slice);
                    return res;
                }else{
                    //use an hslab
                    core::Hyperslab<T> hslab = py::hslab_from_slice<T>(slice);
                    Variable<T>::getValue(hslab);
                    return py::hslab_to_ndarray<T>::convert(hslab);

                }

            }

            //std::shared_ptr<VariableWrapper> ptr(){

            ///@brief extracts a ndarray from the variable with the specified type
            //PyObject * getitem(const p::tuple & slicetuple){
                    //if (p::override getf=this->get_override("__getitem__")){
                        //auto res=getf(slicetuple);
                        //return res;
                    //}else{
                        ////use an hslab
                        //core::Hyperslab<T> hslab = py::hslab_from_slices<T>(slicetuple);
                        //Variable<T>::getValue(hslab);
                        //return py::hslab_to_ndarray<T>::convert(hslab);

                    //}

            //}

            //PyObject * getitem_default(const p::slice &slice){
                    ////use an hslab
                    //core::Hyperslab<T> hslab = py::hslab_from_slice<T>(slice);
                    //Variable<T>::getValue(hslab);
                    //return py::hslab_to_ndarray<T>::convert(hslab);

            //}
            
            //PyObject * getitem_default(const p::tuple &slicetuple){
                    ////use an hslab
                    //core::Hyperslab<T> hslab = py::hslab_from_slices<T>(slicetuple);
                    //Variable<T>::getValue(hslab);
                    //return py::hslab_to_ndarray<T>::convert(hslab);
            //}
                //return std::make_shared<VariableWrapper>();
            //}
        private:
            //PyObject * self{nullptr};
        };

        template<class T>
        struct register_var{
                static void reg(const std::string & basename){
                    PyObject* (VariableWrapper<T>::*geti1)(const p::slice &)=&VariableWrapper<T>::getitem;
                    //PyObject* (VariableWrapper<T>::*geti2)(const p::tuple &)=&VariableWrapper<T>::getitem;

                    //PyObject* (VariableWrapper<T>::*defgeti1)(const p::slice &)=&VariableWrapper<T>::getitem_default;
                    //PyObject* (VariableWrapper<T>::*defgeti2)(const p::tuple &)=&VariableWrapper<T>::getitem_default;
                    
                    p::class_<VariableWrapper<T>,p::bases<VariableDyn>,boost::noncopyable>(basename.c_str())
                            .def(p::init<std::string>())
//                            .def("setValue",&Variable<T>::setValue,&VariableWrapper<T>::default_setValue)
//                            .def("__getitem__",&VariableWrapper<T>::getitem);
                            .def("__getitem__",geti1);
                            //.def("__getitem__",geti2);

                    p::register_ptr_to_python< std::shared_ptr<Variable<T>> >();
                    p::register_ptr_to_python< std::shared_ptr<VariableWrapper<T>> >();
                }
        };



        void (Group::*setamode1)(std::string )=&Group::setAmode;

        void (Group::*setamode2)()=&Group::setAmode;
        using groupPyC=p::class_<GroupWrapper,p::bases<core::TreeNodeCollection>,boost::noncopyable>;

        template<class ... T>
        struct register_group;

        template<class T, class ... TOthers>
        struct register_group<T,TOthers...>{
            static void reg(){
                register_group<T,TOthers...>::regothers(
                        p::class_<GroupWrapper,p::bases<core::TreeNodeCollection>,boost::noncopyable>("Group")
                        .def(p::init<std::string>())
                        .def("readable",&Group::readable)
                        .def("writable",&Group::writable)
                        .def("setAmode",setamode1)
                        .def("setAmode",setamode2)
                        .def("getGroup",&Group::getGroup,p::return_value_policy<p::reference_existing_object>())
                );
                //also register pointer to group
                p::register_ptr_to_python< std::shared_ptr<Group> >();
            }
            static groupPyC & regothers(groupPyC & grppc){
                return register_group<TOthers...>::regothers(grppc
//                .def("__setitem__",&assigntoGroup<size_t,Variable<T>>)
//                .def("__setitem__",&assigntoGroup<std::string,Variable<T>>)
//                .def("__setitem__",&assigntoGroup<size_t,VariableWrapper<T>>)
//                .def("__setitem__",&assigntoGroup<std::string,VariableWrapper<T>>)
                );

            }
        };

        template<>
        struct register_group<>{
            static groupPyC & regothers(groupPyC & grppc){
                //nothing to add (stop recursion)
                return grppc;
            }
        };




        //void testslice(const p::slice & slice){
            //LOGINFO<< p::extract<int>(slice.start()) <<std::endl;
        //}

        void registerArchives(){

            register_group<double,int>::reg();

//            p::class_<VariableDynWrapper,p::bases<core::TreeNodeItem>,boost::noncopyable>("Variable")
//            .def(p::init<std::string>());

            p::class_<VariableDyn,p::bases<core::TreeNodeItem>>("Variable")
                    .def(p::init<std::string>())
                    .def<void(VariableDyn::*)(std::shared_ptr<const core::Frommle>)>("setValue",&VariableDyn::setValue)
                    .def<void(VariableDyn::*)(std::shared_ptr<const core::Frommle>)const>("getValue",&VariableDyn::getValue);

            p::register_ptr_to_python< std::shared_ptr<VariableDyn> >();
            
            register_var<double>::reg("Variable_float64");
            register_var<int>::reg("Variable_int");

            //def("testslice",&testslice);
        }
    }
}
