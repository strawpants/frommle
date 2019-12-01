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
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/noncopyable.hpp>
#include "io/ArchiveBase.hpp"
namespace p = boost::python;


namespace frommle{
    namespace io {

        ///@brief Groupwrapper which is needed to allow dynamic python overloading
        class GroupWrapper:public Group,public p::wrapper<Group>{
        public:
            GroupWrapper(){}
            GroupWrapper(std::string name):Group(name){}
            void pysave(const Frommle & frobj){
                frobj.save(static_cast<Group&>(*this));
            }

            Frommle* pyload(Frommle & frobj){
                frobj.load(static_cast<Group&>(*this));
                return &frobj;
            }


            std::shared_ptr<VariableDyn> getVariable(const std::string &name){
                if (p::override f = this->get_override("getVariable")){
                              return f(name); 
                }
                
                return Group::getVariable(name);
                }
       
            
            std::shared_ptr<VariableDyn> default_getVariable(const std::string & name) {
                return this->Group::getVariable(name);
            }   
            
            //std::shared_ptr<GroupWrapper> ptr(){
                //return std::make_shared<GroupWrapper>();
            //}
            
        private:
        };

        class ArchiveWrapper:public ArchiveBase,public p::wrapper<ArchiveBase>{
        public:
            ArchiveWrapper(){}
            ArchiveWrapper(std::string name):ArchiveBase(name){}
            ArchiveWrapper(std::string name,std::string mode):ArchiveBase(name,mode){}
        };
        ///@brief Variablewrapper which is needed to allow dynamic python overloading
        class VariableWrapper:public VariableDyn,public p::wrapper<VariableDyn>{
        public:
            VariableWrapper(){}
            VariableWrapper(std::string name):VariableDyn(name){}

            //std::shared_ptr<VariableWrapper> ptr(){
                //return std::make_shared<VariableWrapper>();
            //}
        private:
        };

        template<class T>
        struct register_var{
                static void reg(const std::string & basename){

                    p::class_<Variable<T>,p::bases<VariableDyn>>(basename.c_str())
                            .def(p::init<std::string>());

                p::register_ptr_to_python< std::shared_ptr<Variable<T>> >();                   
                }
        };


        template<class I,class NODE>
        void assigntoGroup(Group &coll,const I & idx, NODE & Value ){
            //auto test=dynamic_cast<VariableWrapper*>(Value);
            //if (test){ 
            //LOGINFO << "VariableWrapper "<< test<<" " << Value->name() <<std::endl;
            //}
            coll.upsertChild(idx,Value);
        }

        //template<class I>
        //void assignVar(Group &coll,const I & idx, VariableDyn *){
            
        //}

        void registerArchives(){



            p::class_<GroupWrapper,p::bases<core::TreeNodeCollection>,boost::noncopyable>("Group")
            .def(p::init<std::string>())
            .def("readable",&Group::readable)
            .def("writable",&Group::writable)
            .def("load",&GroupWrapper::pyload,p::return_value_policy<p::manage_new_object>())
            .def("save",&GroupWrapper::pysave)
            .def("getGroup",&Group::getGroup,p::return_value_policy<p::reference_existing_object>())
            //.def("getVariable",&Group::getVariable,&GroupWrapper::default_getVariable)
            .def("__setitem__",&assigntoGroup<size_t,Group>)
            .def("__setitem__",&assigntoGroup<std::string,Group>)
            .def("__setitem__",&assigntoGroup<size_t,VariableWrapper>)
            .def("__setitem__",&assigntoGroup<std::string,VariableWrapper>);
            //.def("__setitem__",&assigntoGroup<size_t,VariableDyn>)
            //.def("__setitem__",&assigntoGroup<std::string,VariableDyn>);
            p::register_ptr_to_python< std::shared_ptr<Group> >();                   
            
//            .def("__setitem__",&assigntoGroup<size_t,TreeNodeCollection>)
//            .def("__setitem__",&assignNode<std::string,TreeNodeCollection>)

            p::class_<VariableWrapper,p::bases<core::TreeNodeItem>,boost::noncopyable>("Variable")
            .def(p::init<std::string>());

            p::register_ptr_to_python< std::shared_ptr<VariableDyn> >();                   
            
            register_var<double>::reg("Variable_float64");
            register_var<int>::reg("Variable_int");

            p::class_<ArchiveWrapper,p::bases<Group>,boost::noncopyable>("Archive")
            .def(p::init<std::string,std::string>())
            .def(p::init<std::string>());

            p::register_ptr_to_python< std::shared_ptr<ArchiveBase> >();                   

//            .def("__getitem__",);
//            .def("__getitem__",);
        }
    }
}
