/*! \file
 \brief
 \copyright Roelof Rietbroek 2018
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

#include <string>
#include <memory>
#include <map>
#include <boost/any.hpp>
#include "core/Exceptions.hpp"
#ifndef FROMMLE_TREENODE_HPP
#define FROMMLE_TREENODE_HPP

namespace frommle{
    namespace core{
        class TreeNodebase{
        public:
            TreeNodebase(const std::string & name):name_(name){}
            TreeNodebase(const std::string && name):name_(std::move(name)){}
            std::string getName()const{return name_;}
            void setName(const std::string name){name_=name;}
            virtual bool isCollection();//dynamically determines whether this is a collection or whether it holds a single value
            TreeNodebase & operator[](const std::string & name)const;
            TreeNodebase & operator[](const std::string & name);

            template<class T>
            T & as(){
                if (isCollection()){
                    throw MethodException("Cannot cast a collection Treenode to a single value");
                }
                return boost::any_cast<T>(val_);
            }

            template<class Value>
            void setAttribute(const std::string & name,const Value & val){
                attrib_[name]=boost::any(val);
            }

            template<class Value>
            Value getAttribute(const std::string & name){
                return boost::any_cast<Value>(attrib_["name"]);
            }

        protected:
//            void setParent(const TreeNodebase & parent){parent_=&parent}
        private:
            std::string name_="TreeNode";
            TreeNodebase * parent_=nullptr;
            std::map<std::string,boost::any> attrib_{};
            boost::any val_{};
        };

        //@brief wrapper class around a shared_ptr which also forwards operator[] to the pointee. Note that this is implemented in the shared_ptr from c++17
        template<class T>
        class TreeNodeRef{
        public:
            TreeNodeRef(T* Tptr):ptr_(Tptr){}// note: this takes ownership of  the passed pointer (i.e. it will be destructed when the TreeNodeRef goes out of scope)
            TreeNodeRef(){}
            T & operator *()const{return *ptr_;}
            T * operator ->()const{return ptr_.get();}
            T * get()const{return ptr_.get();}
            //forward [] operator to the underlying pointer
            template<class I, class child=typename T::child_type>
            typename std::enable_if<!std::is_void<child>::value, child>::type operator[](const I & idx) {return *(ptr_)[idx];}
        private:
            std::shared_ptr<T> ptr_{};
        };

        template<class T,class P>
        class TreeNodeIterator{
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;
            using Parent=P;
            TreeNodeIterator& operator++(){++*ptr_;return *this;};
            bool operator==(const TreeNodeIterator & other) const {return ptr_->id_ == other.ptr_->id_;}
            bool operator!=(const TreeNodeIterator & other) const {return !(*this == other);}
            T * operator*() {return ptr_.get();}
            ~TreeNodeIterator(){}
            TreeNodeIterator(const TreeNodeIterator & in){
                    ptr_=in.ptr_;
            }
            TreeNodeIterator();
            TreeNodeIterator(const P* const parentPtr);
        private:
            TreeNodeRef<T>  ptr_{};

        };




        template<class T, class K, class P>
        class TreeNode: public TreeNodebase{
        public:
            //refers to the iterator containing elements of T itself
            using iterator=TreeNodeIterator<K,T>;
            using child_type=K;
            TreeNode(const std::string name):TreeNodebase(name){}
            TreeNode(const std::string && name):TreeNodebase(std::move(name)){}
            TreeNode(const std::string name,const P * parent ):TreeNodebase(name),parent_(parent){}

            iterator begin()const{return iterator(this);}
            iterator end()const{return iterator();}

            virtual TreeNodeRef<K> operator[](const std::string & name)=0;
            virtual TreeNodeRef<K> operator[](const size_t & indx)=0;
            virtual TreeNode & operator ++(){++id_;}
            const P* parent()const{return parent_;}
//            TreeNodeRef<T> ref();
        private:
            P* parent_=nullptr;
            ptrdiff_t id_=-1;
            std::vector<TreeNodeRef<K>> children_{};
        };

        //specialization when a Treenode has no parent
        template<class T,class K>
        class TreeNode<T,K,void>:public TreeNodebase{
        public:
            using iterator=TreeNodeIterator<K,T>;
            using child_type=K;

            TreeNode(const std::string name):TreeNodebase(name){}
            TreeNode(const std::string && name):TreeNodebase(std::move(name)){}

            iterator begin()const{return iterator(this);}
            iterator end()const{return iterator();}

            virtual TreeNodeRef<K> operator[](const std::string & name)=0;
            virtual TreeNodeRef<K> operator[](const size_t & indx)=0;
            virtual TreeNode & operator ++(){++id_;}

        private:
            ptrdiff_t id_=-1;
        };

        //specialization when a Treenode has no children
        template<class T,class P>
        class TreeNode<T,void,P>:public TreeNodebase{
        public:
            using child_type=void;
            TreeNode(const std::string name):TreeNodebase(name){}
            TreeNode(const std::string && name):TreeNodebase(std::move(name)){}
            TreeNode(const std::string name,const P * parent ):TreeNodebase(name),parent_(parent){}
            virtual TreeNode & operator ++(){++id_;}

        private:
            P* parent_=nullptr;
            ptrdiff_t id_=-1;
        };



    }
}


#endif //FROMMLE_TREENODE_HPP
