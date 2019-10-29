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

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include "core/Exceptions.hpp"
#include <cassert>
#include <functional>
#ifndef FROMMLE_TREENODE_HPP
#define FROMMLE_TREENODE_HPP

namespace frommle{
    namespace core{

        using Attribs=std::map<std::string,boost::any>;

        class TreeNodeBase;
        class TreeNodeCollection;
        //@brief wrapper class around a shared_ptr which also forwards operator[] to the pointee. Note that this is implemented in the shared_ptr from c++11
        class TreeNodeRef{
        public:
            using cvec=std::vector<TreeNodeRef>;
            TreeNodeRef(){}
            explicit TreeNodeRef(const std::string & name );
            template<class T>
            TreeNodeRef(T && nodeIn)noexcept{
                ptr_=std::make_shared<T>(std::move(nodeIn));
            }

            template<class T>
            TreeNodeRef(T & nodeIn)noexcept;
            TreeNodeRef(TreeNodeRef & in){
                ptr_=in.ptr_;
            }
            explicit TreeNodeRef(const TreeNodeCollection * parent);
//            explicit TreeNodeRef(const std::string name){
//
//            }
            ///@brief replace the current treenode while keeping the original name and parent
            const TreeNodeRef & operator=(TreeNodeRef && in);
            //@brief copy a TreenodeRef in the current position but don't update the parent and name in the original
            const TreeNodeRef & operator=(const TreeNodeRef & in){
                ptr_=in.ptr_;
                return *this;
            }
            ///@brief assign a treenoderef and make sure that the righthandside has the correct name and parent set
            const TreeNodeRef & operator=(TreeNodeRef & in);

            TreeNodeRef(const TreeNodeRef & in){
                ptr_=in.ptr_;
            }

            TreeNodeRef(TreeNodeRef && in)noexcept{
                *this=std::move(in);
            }
//            TreeNodeRef(TreeNodeRef && in){
//                *this=std::move(in);
//            }

            cvec::const_iterator cbegin()const;
            cvec::const_iterator cend()const;

            cvec::iterator begin();
            cvec::iterator end();


//            template<class T>
//            TreeNodeRef & upsertChild(const std::string name,T && in);
//            template<class T>
//            TreeNodeRef & upsertChild(const size_t idx,T && in);



            template<class T>
            TreeNodeRef & operator =( T && in);

            TreeNodeBase & operator *()const{return *ptr_;}
            TreeNodeBase * operator ->()const{return ptr_.get();}
            TreeNodeBase * get()const{return ptr_.get();}
            template<class T>
            T & as(){return dynamic_cast<T&>(*(ptr_.get()));}

            template<class I>
            const TreeNodeRef  operator[](const I &idx) const;

            template<class I>
            TreeNodeRef operator[](const I &idx);

            explicit operator bool()const;
            //forward some calls to underlying ptr
            const std::string & getName()const;
        private:
            std::shared_ptr<TreeNodeBase> ptr_{};
        };

        class TreeNodeBase{
        public:
            TreeNodeBase(){}
            ~TreeNodeBase(){}
            TreeNodeBase(const std::string & name):name_(name){}
            TreeNodeBase(const std::string && name):name_(std::move(name)){}
            TreeNodeBase(const std::string name, Attribs && attr):name_(name),attrib_(std::move(attr)){}
            TreeNodeBase(TreeNodeRef && in);
            TreeNodeBase(const TreeNodeBase & in)=default;

//            {
//                name_=in.name_;
//                attrib_=in.attrib_;
//                parent_=in.parent_;
//            }

            const std::string & getName()const{return name_;}
            void setName(const std::string name){name_=name;}
            virtual bool isCollection()const{return false;};//dynamically determines whether this is a collection or whether it holds a single value

            template<class Value>
            void setAttribute(const std::string & name,const Value & val){
                attrib_[name]=boost::any(val);
            }

//            template<class Value>
//           Value getAttribute(const std::string &name){
//                return boost::any_cast<Value>(attrib_[name]);
//            }

            size_t getAttributeCount(const std::string & name){return attrib_.count(name);}
            const Attribs & getAttribMap(){return attrib_;}

            template <class T>
            bool findUpstream(std::function<bool(const TreeNodeBase*,T&)> testfunc,T& retval);
            ///@brief standard way to retrieve an attribute
            template<class Value>
            typename std::enable_if<!std::is_same<Value,std::string>::value,Value>::type getAttribute(const std::string &name){
                return boost::any_cast<Value>(attrib_[name]);
            }

            ///@brief special treatment of std::string attribute retrieval
            template<class Value>
            typename std::enable_if<std::is_same<Value,std::string>::value,Value>::type getAttribute(const std::string &name){
                try{
                    return boost::any_cast<std::string>(attrib_[name]);
                }catch(boost::bad_any_cast & excep){
                    return std::string(boost::any_cast<char const * >(attrib_[name]));
                }
            }

            virtual const TreeNodeRef operator[](const std::string & name)const {throwMethExcept();return TreeNodeRef();};
            virtual const TreeNodeRef operator[](const size_t & idx)const {throwMethExcept();return TreeNodeRef();};

            virtual TreeNodeRef operator[](const std::string & name){throwMethExcept();return TreeNodeRef();}
            virtual TreeNodeRef operator[](const size_t & idx){throwMethExcept();return TreeNodeRef();};

//            virtual std::shared_ptr<TreeNodeBase> getSelf()const=0;
//            virtual const TreeNodeRef & ref()const=0;
            TreeNodeCollection* getParent()const{return parent_;}
//            std::shared_ptr<TreeNodeBase> getParent()const{return parent_;}
            template<class T>
            TreeNodeBase & setParent(T & parent) &{
                parent_=&parent;
                parentHook();
                return *this;
            }

            template<class T>
            TreeNodeBase && setParent(T & parent) &&{
                parent_=&parent;
                parentHook();
                return std::move(*this);
            }
//            void setParent(std::shared_ptr<T> && parent){
//                parent_=std::move(parent);
//                parentHook();
//            }
        protected:

        private:
            void throwMethExcept()const;
            ///@brief possibly overload this function to perform actions after assigning a new parent
            virtual void parentHook(){};
            std::string name_="";
            Attribs attrib_{};
            TreeNodeCollection *parent_=nullptr;
        };


        class TreeNodeItem:public TreeNodeBase{
        public:
            TreeNodeItem(){}
            TreeNodeItem(const std::string & name):TreeNodeBase(name){}
            TreeNodeItem(const std::string name, Attribs && attr):TreeNodeBase(name,std::move(attr)){}
            TreeNodeItem(TreeNodeRef && in):TreeNodeBase(std::move(in)){}
        private:
        };


        class TreeNodeCollection: public TreeNodeBase{
        public:
            using cvec=std::vector<TreeNodeRef>;
            TreeNodeCollection():TreeNodeBase(){}
            TreeNodeCollection(const std::string & name):TreeNodeBase(name){}
            TreeNodeCollection(const std::string && name):TreeNodeBase(std::move(name)){}
            TreeNodeCollection(const std::string name, Attribs && attr):TreeNodeBase(name,std::move(attr)){}
            TreeNodeCollection(TreeNodeRef && in):TreeNodeBase(std::move(in)){}
            typename cvec::const_iterator cbegin()const{return collection_.cbegin();}
            typename cvec::const_iterator cend()const{return collection_.cend();}
            typename cvec::iterator begin(){ loadCollection();return collection_.begin();}
            typename cvec::iterator end(){return collection_.end();}
            size_t size()const{return collection_.size();}

            virtual const TreeNodeRef operator[](const std::string & name)const;
            virtual const TreeNodeRef operator[](const size_t & idx)const;

            virtual TreeNodeRef operator[](const std::string & name);
            virtual TreeNodeRef operator[](const size_t & idx);

            bool isCollection()const final{return true;}
//            virtual std::shared_ptr<TreeNodeBase> getSelf()const;

//            virtual TreeNodeRef ref()const{
//                return TreeNodeRef(new TreeNodeCollection(*this));
//            }
            template<class T>
            TreeNodeRef & upsertChild(const std::string name,T && in);
            template<class T>
            TreeNodeRef & upsertChild(const size_t idx,T && in);

            TreeNodeRef & upsertChild(const size_t idx,TreeNodeRef && in);
            TreeNodeRef & upsertChild(const std::string name,TreeNodeRef && in);
            void deleteCollection(){
                collection_=cvec{};
            }

            ptrdiff_t findidx(const std::string name)const;
            virtual TreeNodeRef convertChild(TreeNodeRef &&in);
        protected:
            //@brief optional virtual function to load all or specific available children
            // call loadCollection() to load all, or specify either an id or name
            virtual void loadCollection(){};
            //other overloads which maye be implemented in derived classes (default calls back to loading the entire collection
            virtual void loadCollection(const size_t id){loadCollection();}
            virtual void loadCollection(const std::string name ){loadCollection();}
        private:
            cvec collection_{};
//            TreeNodeRef ref_{};
        };

        template<class T>
        TreeNodeRef &TreeNodeRef::operator=(T &&in) {

            if(ptr_) {
                std::string iname = ptr_->getName();
                if (iname.empty()){
                    iname=in.getName();
                }

                auto currentParent = ptr_->getParent();

                if (currentParent) {
                    //also inherit the parent from the current TreeNodeRef
//                    ptr_->setParent(*currentParent);

//                    ptr_ = currentParent->convertChild(std::move(in)).ptr_;//std::make_shared<T>(std::move(in));
                    //register as a child at the parent
                    currentParent->upsertChild(iname,currentParent->convertChild(TreeNodeRef(std::move(in))));
                    ptr_=currentParent->operator[](iname).ptr_;
                }
            }else{
                *this=std::move(in);
            }
            return *this;
        }

        template<class I>
        const TreeNodeRef TreeNodeRef::operator[](const I &idx) const {
            if (!ptr_) {
                //just return this empty instance
                return *this;
//                    throw IndexingException("TreeNodeRef points to nothing");
            }

            return ptr_->operator[](idx);
        }


        template<class I>
        TreeNodeRef TreeNodeRef::operator[](const I &idx) {
            if (!ptr_) {
                return *this;
//                    throw IndexingException("TreenodeRef points to nothing");
            }
            return ptr_->operator[](idx);
        }

        template<class T>
        TreeNodeRef::TreeNodeRef(T &nodeIn) noexcept {
            //use shared_ptr aliasing so the pointer points to the memory of nodein, but does not own it
            ptr_=std::shared_ptr<T>(std::shared_ptr<T>(),& nodeIn);
        }







        //tempalted member functions can be found below



        template<class T>
        bool TreeNodeBase::findUpstream(std::function<bool(const TreeNodeBase *, T &)> testfunc, T &retval) {
            if (!testfunc(this,retval)){
                if (parent_) {
                    return parent_->findUpstream(testfunc, retval);
                }else{
                    return false;
                }
            }else{
                return true;
            }
        }





        template<class T>
        TreeNodeRef & TreeNodeCollection::upsertChild(const std::string name, T &&in) {
            //dynamically check for the names of the children
            auto idx=findidx(name);
            if (idx == -1){
                collection_.push_back(TreeNodeRef(std::move(in)));
                idx=collection_.size()-1;
                //and possibly overwrite the name of the node
                collection_.back()->setName(name);
            }else{
                collection_[idx]=TreeNodeRef(std::move(in));
            }

            collection_[idx]->setParent(*this);
            return collection_[idx];


        }

        template<class T>
        TreeNodeRef & TreeNodeCollection::upsertChild(const size_t idx,T && in){
            std::string name=in.getName();
            if (idx >= collection_.size()) {
                    //first resize the continer to accomodate the placement at idx}
                    collection_.resize(idx+1);
            }
            collection_[idx]=TreeNodeRef(std::move(in));
            collection_[idx]->setParent(*this);
//            collection_[idx]->setParent((*this).getSelf());
            return collection_[idx];

        }

//        template<class T>
//        TreeNodeRef & TreeNodeRef::upsertChild(const size_t idx, T &&in) {
//            if(!ptr_->isCollection()) {
//                throw MethodException("Can only upsert a child in a TreeNodecollection");
//
//            }
//            return static_cast<TreeNodeCollection*>(ptr_.get())->upsertChild(idx,std::move(in));
//            //also set the parent of the child
////            ref.parent_=ptr_;
////            return ref;
//        }

//        template<class T>
//        TreeNodeRef & TreeNodeRef::upsertChild(const std::string name, T &&in) {
//
//            if(!ptr_->isCollection()) {
//                throw MethodException("Can only upsert a child in a TreeNodecollection");
//
//            }
//            return static_cast<TreeNodeCollection*>(ptr_.get())->upsertChild(name,std::move(in));
////            ref.setParent(ptr_);
////            return ref;
//
//        }

        template<class T, class It>
        class iteratorWrap: public It{
            public:
                //wrap some calls to derefence the values
            T & operator*() {return static_cast<T&>((this->It::operator*()));}
            iteratorWrap():It(){}
            template<class ...Args>
            iteratorWrap(Args && ...args):It(std::forward<Args>(args)...){}
            };
    }
}


#endif //FROMMLE_TREENODE_HPP
