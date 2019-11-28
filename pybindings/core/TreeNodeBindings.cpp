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

//        class FrommleWrapper:public Frommle,public p::wrapper<Frommle>{
//        public:
//            void save(io::Group & grp)const{
//                if(auto saveop=this->get_override("save")){
//                    saveop(grp);
//
//                }else{
//                    Frommle::save(grp);
//
//                }
//
//            }
//
//            void save_default(io::Group & grp)const{
//                Frommle::save(grp);
//
//            }
//
//
//            void load(io::Group & grp){
//                if(auto loadop=this->get_override("load")){
//                    loadop(grp);
//
//                }else{
//                    Frommle::load(grp);
//
//                }
//
//            }
//
//            void load_default(io::Group & grp){
//                Frommle::load(grp);
//
//            }
//
//
//        };

        const TreeNodeRef (TreeNodeBase::*cgeti1)(size_t )const=&TreeNodeBase::operator[];
        const TreeNodeRef (TreeNodeBase::*cgeti2)(const std::string &)const=&TreeNodeBase::operator[];

        TreeNodeRef  (TreeNodeBase::*geti1)(size_t )=&TreeNodeBase::operator[];
        TreeNodeRef  (TreeNodeBase::*geti2)(const std::string &)=&TreeNodeBase::operator[];

        bool contains(const TreeNodeCollection & coll, std::string name){
            return coll.findidx(name) != -1;
        }

        template<class I,class NODE>
        void assignNode(TreeNodeCollection &coll,const I & idx, const NODE & Value ){
            coll.upsertChild(idx,Value);
        }


        void registerTreeNodes() {
            using namespace boost::python;

            p::class_<TreeNodeBase,TreeNodeRef,p::bases<Frommle>>("TreeNode")
                    .def(p::init<std::string>())
                    .def("__getitem__",geti1)
                    .def("__getitem__",geti2)
                    .def("isCollection",&TreeNodeBase::isCollection);

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

        }


    }
}
