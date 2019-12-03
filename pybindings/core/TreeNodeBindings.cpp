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
#include "core/TreeNode.hpp"
#include <boost/python/return_internal_reference.hpp>
#include "core/GuideBase.hpp"
#include <boost/date_time.hpp>
namespace p = boost::python;
namespace boost {
    namespace python {
        template <>
        struct pointee<frommle::core::TreeNodeRef>
        {
            using type = frommle::core::TreeNodeBase;
        };
    }
}


namespace frommle {
    namespace core {

        TreeNodeBase *get_pointer(frommle::core::TreeNodeRef const &treePtr) {
            return const_cast<TreeNodeBase *>(treePtr.get());
        }

        const TreeNodeRef & (TreeNodeBase::*cgeti1)(size_t )const=&TreeNodeBase::operator[];
        const TreeNodeRef & (TreeNodeBase::*cgeti2)(const std::string &)const=&TreeNodeBase::operator[];

        TreeNodeRef  (TreeNodeBase::*geti1)(size_t )=&TreeNodeBase::operator[];
        TreeNodeRef  (TreeNodeBase::*geti2)(const std::string &)=&TreeNodeBase::operator[];

        bool contains(const TreeNodeCollection & coll, std::string name){
            return coll.findidx(name) != -1;
        }

        template<class I,class NODE>
        void assignNode(TreeNodeCollection &coll,const I & idx, std::shared_ptr<NODE> Value ){
            coll.upsertChild(idx,Value);
        }




        Attributes & (TreeNodeBase::*attrf)()=&TreeNodeBase::attr;


        using attGP=p::class_<Attributes>;

        template<class ...T>
        struct registerAttributes;

        template<class T, class ... TOthers>
        struct registerAttributes<T,TOthers...>{
            static void reg(){

                regothers(p::class_<Attributes>("Attributes")
                    .def("__contains__",&Attributes::contains).def("__getitem__",&registerAttributes::tryget));
            }

            static attGP & regothers(attGP & bpatt){
                return registerAttributes<TOthers...>::regothers(bpatt
                        .def("__setitem__",&Attributes::set<T>));
            }

            static PyObject* tryget(Attributes & attr,const std::string & key){
                    
                    try{
                        LOGINFO << "tryincast " << typeid(T).name() << std::endl;
                        return p::incref(p::object(attr.get<T>(key)).ptr());
                    }catch (boost::bad_any_cast & exc){
                        //OK tjust try the next possiblity
                        return registerAttributes<TOthers...>::tryget(attr,key);

                    }
            }

        };


        template<>
        struct registerAttributes<>{
            static attGP & regothers(attGP & bpatt){
                //nothing to add just return input
                return bpatt;
            }

            static PyObject* tryget(Attributes & attr,const std::string & key){
                THROWNOTIMPLEMENTED("cannot cast boost any to known python type");

            }
        };


        void registerTreeNodes() {
            using namespace boost::python;
            //note the order of the types is important (first will be tested first)
            registerAttributes<p::object,double,int,size_t,std::string, guides::typehash,boost::gregorian::date,boost::posix_time::ptime>::reg();


            p::class_<TreeNodeBase,TreeNodeRef,p::bases<Frommle>>("TreeNode")
                    .def(p::init<std::string>())
                    .def("__getitem__",cgeti1,p::return_internal_reference<>())
                    .def("__getitem__",cgeti2,p::return_internal_reference<>())
                    .def("isCollection",&TreeNodeBase::isCollection)
                    .add_property("attr",p::make_function(attrf,p::return_internal_reference<>()))
                    .add_property("parent",p::make_function(&TreeNodeBase::getParentCollection,p::return_internal_reference<>()));
            p::register_ptr_to_python< std::shared_ptr<TreeNodeBase> >();                   
  
            p::class_<TreeNodeItem,p::bases<TreeNodeBase>>("TreeNodeItem")
            .def(p::init<std::string>());


            p::class_<TreeNodeCollection,p::bases<TreeNodeBase>>("TreeNodeCollection")
            .def(p::init<std::string>())
            .def("__contains__",&contains)
            .def("__setitem__",&assignNode<size_t,TreeNodeItem>)
            .def("__setitem__",&assignNode<std::string,TreeNodeItem>)
            .def("__setitem__",&assignNode<size_t,TreeNodeCollection>)
            .def("__setitem__",&assignNode<std::string,TreeNodeCollection>)
            ;

            p::register_ptr_to_python< std::shared_ptr<TreeNodeCollection> >();                   
        }


    }
}
