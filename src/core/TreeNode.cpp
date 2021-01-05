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

#include "core/TreeNode.hpp"
#include "TreeNode.hpp"

namespace frommle {
    namespace core {

        ///Implement (non-templated) constructors of Treenoderef
        TreeNodeRef::TreeNodeRef(){}
        TreeNodeRef::TreeNodeRef(const std::string &name) {
            ptr_=std::make_shared<TreeNodeItem>(name);
        }
        TreeNodeRef::TreeNodeRef(TreeNodeBase* ptr){
            //This takes ownership of the pointer deallocation duties!
            ptr_=std::shared_ptr<TreeNodeBase>(ptr);
        }

        TreeNodeRef::TreeNodeRef(TreeNodeRef & in){
            ptr_=in.ptr_;
        }


        TreeNodeRef::cvec::const_iterator
        TreeNodeRef::cbegin() const {
            if (!ptr_->isCollection()) {
                throw MethodException("Can only iterate over a TreeNodeItem");

            }

            return static_cast<const TreeNodeCollection *>(ptr_.get())->cbegin();
        }

        TreeNodeRef::cvec::const_iterator
        TreeNodeRef::cend() const {
            if (!ptr_->isCollection()) {
                throw MethodException("Can only iterate over a TreeNodeItem");

            }
            return static_cast<const TreeNodeCollection *>(ptr_.get())->cend();
        }

        TreeNodeRef::cvec::iterator
        TreeNodeRef::begin() {
            if (!ptr_->isCollection()) {
                throw MethodException("Can only iterate over a TreeNodeCollection");

            }
            return static_cast<TreeNodeCollection *>(ptr_.get())->begin();
        }

        TreeNodeRef::cvec::iterator
        TreeNodeRef::end() {
            if (!ptr_->isCollection()) {
                throw MethodException("Can only iterate over an TreeNodeCollection");

            }
            return static_cast<TreeNodeCollection *>(ptr_.get())->end();

        }

        const TreeNodeRef &TreeNodeRef::operator=(TreeNodeRef &&in) {
            if (!ptr_) {
                //quick shortcut if current TreeNode is informationless
                ptr_=std::move(in.ptr_);
                return *this;
            }


            std::string iname = ptr_->name();
            auto currentParent = ptr_->getParent();

            if (currentParent) {
                ptr_ = currentParent->convertChild(std::move(in)).ptr_;
                //also inherit the parent from the current TreeNodeRef
                ptr_->setParent(*currentParent);
            }

            if (!iname.empty()) {
                //Note existing name takes precedence so here we put it back in
                ptr_->setName(iname);
            }

            return *this;
        }

        const TreeNodeRef &TreeNodeRef::operator=(TreeNodeRef &in) {
            if (!ptr_) {
                //quick shortcut if current TreeNode is informationless
                ptr_=in.ptr_;
                return *this;
            }
            std::string iname= ptr_->name();
            auto currentParent=ptr_->getParent();

            if(currentParent) {
                //possibly modify the underlying type so it can work together with the parent
                ptr_=currentParent->convertChild(TreeNodeRef(in)).ptr_;
                //also let the input.ptr point to the same ptr_
                in.ptr_=ptr_;
                //also inherit the parent from the current TreeNodeRef
                ptr_->setParent(*currentParent);
            }else{
                ptr_=in.ptr_;
            }

            if (!iname.empty()){
                //Note existing name takes precedence so here we put it back in
                ptr_->setName(iname);
            }

            return *this;
        }

        std::string TreeNodeRef::name() const {return ptr_->name();}


        TreeNodeRef::operator bool() const {
            if (ptr_) {
                //try casting this to a TreeNodeCollection or a TreeNodeItem if not sucessful return false
                auto test = dynamic_cast<const TreeNodeItem *>(ptr_.get());
                if (test) {
                    return true;
                }
                //try casting to a collection
                auto test2 = dynamic_cast<const TreeNodeCollection * >(ptr_.get());
                if(test2){
                    return true;
                }
            }

            //if we end up here the TreeNoderef is invalid
            return false;
        }







        const TreeNodeRef & TreeNodeCollection::operator[](const std::string &name) const {
            auto idx = findidx(name);
            if (idx == -1) {
                auto message=std::string("Treenode not found: ")+ name;
                THROWINPUTEXCEPTION(message);
//                return TreeNodeBase(name);
            }
            return collection_[idx];
        }

        ///@brief at operator which autmomatically creates a new entry in the collection
        TreeNodeRef & TreeNodeCollection::operator[](const std::string &name) {
            loadCollection(name);
            auto idx = findidx(name);
            if (idx == -1) {
                //create a new Treenode with the parent appriopriately assigned
                collection_.push_back(TreeNodeBase(name).setParent(*this));
                idx=collection_.size()-1;
            }
            return collection_[idx];
        }


        const TreeNodeRef & TreeNodeCollection::operator[](size_t idx) const  {
            if (idx >= collection_.size()) {
                auto message=std::string("Treenode index not found: ");
                THROWINPUTEXCEPTION(message);
//                return TreeNodeBase();
            }
            return collection_[idx];
        }

        TreeNodeRef & TreeNodeCollection::operator[](size_t idx) {
            loadCollection(idx);
            if (idx >= collection_.size()) {
                collection_.resize(idx); 
                //create a new Treenode with the parent appriopriately assigned
                collection_.push_back(TreeNodeBase().setParent(*this));
            }
            return collection_[idx];
        }

        ///@brief delete an entry form a collection
        void TreeNodeCollection::delitem(std::string name){
        
            auto idx = findidx(name);
            if (idx != -1){
                //only erase if the element is actually present
                collection_.erase(collection_.begin()+idx);
            }


        }

        ptrdiff_t TreeNodeCollection::findidx(const std::string name) const {
            for (ptrdiff_t i = 0; i < collection_.size(); ++i) {
                if (collection_[i]) {
                    if (collection_[i]->name() == name) {
                        return i;
                    }
                }
            }
            return -1;
        }

        void TreeNodeBase::throwMethExcept() const {
            throw MethodException("Cannot call operator [] on this TreeNodeBase");
        }

        ///@brief default just passes the input
        TreeNodeRef TreeNodeCollection::convertChild(TreeNodeRef &&in) {
            return std::move(in);
        }

        TreeNodeRef &TreeNodeCollection::upsertChild(std::string name, TreeNodeRef &&in) {
            auto idx =findidx(name);
            in->setName(name);
            if(idx ==-1){
                collection_.push_back(std::move(in));
                idx=collection_.size()-1;
            }else{
                collection_[idx]=std::move(in);

            }

            collection_[idx]->setParent(*this);
            return collection_[idx];



        }

        TreeNodeRef &TreeNodeCollection::upsertChild(const size_t idx, TreeNodeRef &&in) {
            std::string name=in.name();
            if (idx >= collection_.size()) {
                //first resize the container to accomodate the placement at idx}
                collection_.resize(idx+1);
            }

            collection_[idx]=std::move(in);
            collection_[idx]->setParent(*this);
            return collection_[idx];

        }


        TreeNodeBase::TreeNodeBase(TreeNodeRef &&in):name_(in.name()) {
            attrib_ = in->attrib_;
            parent_ = in->parent_;
        }


    }
}
