/*! \file
 \brief hold the archive VariableBAse class and some major derived versions
 \copyright Roelof Rietbroek 2020
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
#include "core/Hyperslab.hpp"
#include "core/VisitorTools.hpp"

#ifndef FROMMLE_ARVARIABLE_HPP
#define FROMMLE_ARVARIABLE_HPP

namespace frommle{
    namespace io{

    class VariableBase:public core::TreeNodeItem{
    public:
        VariableBase():TreeNodeItem(){}
        VariableBase(std::string name):TreeNodeItem(name){}
        VariableBase(std::string name, core::Attributes && attr):TreeNodeItem(name,std::move(attr)){}
        VariableBase(core::TreeNodeRef && in):TreeNodeItem(std::move(in)){}
        template<class T>
        void setValue(const core::HyperSlabBase<T> & hslab);

        template<class T>
        void getValue(core::HyperSlabBase<T> & hslab)const;

        ///@brief getValue of frommle object at once
        virtual bool getValue(std::shared_ptr<core::Frommle> frptr)const{
            return false;
       }

        constexpr bool readable()const{
            return static_cast<Group*>(getParent())->readable();
        }

        constexpr bool writable()const {
            return static_cast<Group *>(getParent())->writable();
        }

        virtual core::typehash hash() const {
            return attr().get<core::typehash>("frhash");
        }

    };

        class VariableFrommle:public VariableBase{
        public:
            VariableFrommle():VariableBase(){}
            VariableFrommle(std::string name):VariableBase(name){}
            VariableFrommle(std::string name, core::Attributes && attr):VariableBase(name,std::move(attr)){}
            VariableFrommle(std::shared_ptr<core::Frommle> frobj ):VariableBase(frobj->name()),frptr_(frobj){}
            VariableFrommle(core::TreeNodeRef && in):VariableBase(std::move(in)){}

            ///@brief setValue of frommle object at once
            virtual void setValue(std::shared_ptr<const core::Frommle> frptr){
                //overloading this function in derived classes allows for runtime writing of Frommle classes
                //The  default just sets the internal pointer to the payload
                frptr_=std::const_pointer_cast<core::Frommle>(frptr);
            }

            virtual std::vector<size_t> shape()const{
                //default forwards the request to the internal frommle pointer
                if (frptr_){
                    return frptr_->shape();
                }else {
                    THROWNOTIMPLEMENTED("This Variable type is empty or does not provide shape information");
                }
            }
            core::typehash hash() const override {
                if( frptr_){
                    return frptr_->hash();
                }else {
                    //try to retreieve the hash from the attributes
                    return attr().get<core::typehash>("frhash");
                }
            }
        ///@brief getValue of frommle object at once
            bool getValue(std::shared_ptr<core::Frommle> frptr)const override{
                //otherwise we need to be a bit more delicate and adapt the input
                if (frptr_ ){
                    frptr->createFrom(frptr_);
                }else{
                    //try reconstructing the frommle derived object from a typehash attribute
                    //This only works when the derived types of frptr have createFrom implemented
                    frptr->createFrom(attr().get<core::typehash>("frhash"));

                }
                return true;
            }


            std::shared_ptr<core::Frommle> getPtr()const{
                if (frptr_){
                    return frptr_;
                }else{
                    THROWINPUTEXCEPTION("No value in VariableFrommle is set");

                }

            }



        private:
            //we can store a pointer to anything which is derived from a Frommle base class
            std::shared_ptr<core::Frommle> frptr_{};

        };

//        using valueVariant=boost::variant<double,int,long long int,std::string,OGRGeometry*>;
        template<class T>
        class Variable:public VariableBase{
        public:
            using single=T;
            using singlePtr=std::shared_ptr<single>;
            virtual int ndim()const{return 1;}
            Variable(core::TreeNodeRef && in):VariableBase(std::move(in)){}
            Variable():VariableBase(){}
            Variable(std::string name):VariableBase(name){}
            Variable(std::string name, core::Attributes && attr):VariableBase(name,std::move(attr)){}
            void setValue(const single & val,const ptrdiff_t idx) {
                //here we construct a shared_ptr but don't dealloacate the actual value after calling by using an alias constructor
                setValue(singlePtr(singlePtr(), const_cast<single*>(&val)),idx);
            }


            virtual void getValue(singlePtr & in,const ptrdiff_t idx)const{THROWMETHODEXCEPTION("getValue not implemented");}
            virtual void setValue(const singlePtr & val,const ptrdiff_t idx){THROWMETHODEXCEPTION("setValue not implemented");}
            virtual void setValue(const core::HyperSlabBase<T> & hslab){THROWMETHODEXCEPTION("hyperslab writing not supported");}
            virtual void getValue(core::HyperSlabBase<T> & hslab)const{THROWMETHODEXCEPTION("hyperslab reading not supported");}

            ///@brief iterator which loops over the values in this variable
            class iterator{
            public:
                //iterator traits

                using iterator_category = std::forward_iterator_tag;
                using value_type = singlePtr;
                using difference_type = std::ptrdiff_t;
                using pointer = singlePtr*;
                using reference = singlePtr&;
                iterator& operator++(){
                    parent_->getValue(value_,-1);
                    return *this;
                };
                bool operator==(const iterator & other) const {return value_ == other.value_;}
                bool operator!=(const iterator & other) const {return !(*this == other);}
                singlePtr & operator*() {return value_;}
                iterator(){}
                iterator(const Variable *parent):value_(std::make_shared<single>()),parent_(parent){
                    parent_->getValue(value_,0);
                }
            protected:
                //note the iterator does not own the resource of the pointer!!
//            single* value_=nullptr;
                std::shared_ptr<single> value_{};
            private:
                const Variable *parent_{};
            };
            iterator begin(){return iterator(this);}
            iterator end(){return iterator();}
        private:

        };

        template<class T>
        Variable<T> & Group::getVariable(const std::string &name) {
            auto idx=findidx(name);
            if (idx == -1){
                //create a new Variable
                this->operator[](name)=Variable<T>();
            }
            //check if the variable is derived
            return this->operator[](name).as<Variable<T>>();

        }


        template<template<class> class Vd,class F, class ...Ts>
        struct tryVarCasts{};

        template<template<class> class Vd,class F, class ...Ts>
        struct tryVarCasts<Vd,core::typelist<F,Ts...>> {
        core::TreeNodeRef operator()(core::TreeNodeRef &&in) {
            auto tmp=dynamic_cast<Variable<F> *>(in.get());
            if(tmp){
                //yeah, success let's proceed by returning a converted type
                return core::TreeNodeRef(Vd<F>(std::move(in)));
            } else{
                //no success try the next type
                return tryVarCasts<Vd,core::typelist<Ts...>>()(std::move(in));
            }

        }
    };

    template<template<class> class Vd,class F>
    struct tryVarCasts<Vd,core::typelist<F>> {
    core::TreeNodeRef operator()(core::TreeNodeRef &&in) {
        auto tmp=dynamic_cast<Variable<F> *>(in.get());
        if(tmp){
            //yeah, success let's proceed by returning a converted type
            return core::TreeNodeRef(Vd<F>(std::move(in)));
        } else{
            //no success  and nothing left to try
            THROWINPUTEXCEPTION("No more casting possibilities for Variable");
        }

    }
};

///@brief forwards setrValue call to underlying variable specialization
template<class T>
void VariableBase::setValue(const core::HyperSlabBase<T> & hslab){
    VariableBase* ptr=this;
    Variable<T> * varptr=dynamic_cast<Variable<T>*>(ptr);

    if(varptr){
        varptr->setValue(hslab);
    }else{
        THROWINPUTEXCEPTION("Cannot dynamically downcast VariableDyn to requeste type");
    }


}

///@brief forwards setrValue call to underlying variable specialization
template<class T>
void VariableBase::getValue(core::HyperSlabBase<T> & hslab)const{
    const VariableBase* ptr=this;
    const Variable<T> * varptr=dynamic_cast<const Variable<T>*>(ptr);

    if(varptr){
        varptr->getValue(hslab);
    }else{
        THROWINPUTEXCEPTION("Cannot dynamically downcast VariableDyn to requested type");
    }


}


    }

}

#endif //FROMMLE_ARVARIABLE_HPP
