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
#include <cassert>
#include "core/TreeNode.hpp"
//#include <boost/serialization/split_free.hpp>
#include "core/VisitorTools.hpp"
#include "core/Hyperslab.hpp"
#include "core/GuideBase.hpp"
#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP
//fowrward declare classes contained within the variant class below
class OGRGeometry;

namespace frommle{
    namespace io{
        class Group;
        struct serialize{
            template<class G,class T>
            inline static void save(const G & grp, T & val){val.save(const_cast<Group&>(grp));}

            template<class G,class T>
            inline static void load(const G & grp, T & val){val.load(const_cast<Group&>(grp));}
        };


        //forward declare a variable here
        class VariableDyn;
        template<class T>
        class Variable;

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class Group:public core::TreeNodeCollection {
        public:
            Group():TreeNodeCollection(){}
            //construct when only the groupname is known
            Group(const std::string & name ):TreeNodeCollection(name){}
//            Group(const std::string && name ):TreeNodeCollection(std::move(name)){}
            Group(const std::string name, core::Attributes && attr):TreeNodeCollection(name,std::move(attr)){}
            //construct when both the name and parent are known
            template<class P>
            Group(const std::string & name, const P * const parent ):TreeNodeCollection(name,parent){}
            Group(core::TreeNodeRef && in):TreeNodeCollection(std::move(in)){};

            //serialization goodies (objects can be read/written to groups by implementing a serialization function
            template<class Y>
            Group & operator >> (Y & out){
                out.load(*this);
//                serialize::load(*this,out);
                return *this;
            }

            template<class Y>
            Group & operator << (Y & out){
                out.save(*this);
//                serialize::save(*this,out);

                return *this;
            }

            template<class Y>
            Group & operator << (Y && out){
                out.save(*this);
                //serialize::save(*this,out);
                return *this;
            }

//            template<class Y>
//            Group & operator << (Y & in){boost::serialization::serialize(*this,in,file_version()); return *this;}

//            using const_iterator = core::iteratorWrap<Variable,cvec::const_iterator>;
//            using iterator = core::iteratorWrap<Variable,cvec::iterator>;
//            const_iterator cbegin()const{ return const_iterator(this->TreeNodeCollection::cbegin());}
//            const_iterator cend()const{return const_iterator(this->TreeNodeCollection::cend()); }
//            iterator begin(){return iterator(this->TreeNodeCollection::begin()); }
//            iterator end(){return iterator(this->TreeNodeCollection::end()); }

            bool readable()const;
            bool writable()const;
            virtual Group & getGroup(const std::string &name);

            virtual std::shared_ptr<VariableDyn> getVariable(const std::string &name);

            template<class T>
            Variable<T> & getVariable(const std::string & name);

            void setAmode(std::string mode){
                attr().set("mode",mode);
                if (mode == "r"){
                    openForReading=true;
                    openForWriting=false;
                }else if( mode == "w"){
                    openForReading=false;
                    openForWriting=true;
                }else if(mode == "rw"){
                    openForReading=true;
                    openForWriting=true;
                }else{
                    THROWINPUTEXCEPTION("cannot understand the access mode of the group");
                }

            }
            void setAmode(){
                auto ogrgrp=static_cast<Group*>(getParent());
                if (ogrgrp){
                    //take the accessmode from above
                    ogrgrp->setAmode();
                    openForReading=ogrgrp->readable();
                    openForWriting=ogrgrp->writable();
                }else if (attr().getCount("mode") != 0) {
                    auto mode = attr().get<std::string>("mode");
                    if( mode == "r"){
                        openForReading=true;
                        openForWriting=false;
                    }else if (mode == "w"){
                        openForReading=false;
                        openForWriting=true;

                    }else if (mode == "rw"){
                        openForReading=true;
                        openForWriting=true;

                    }else{
                        throw core::InputException("cannot understand the access mode of this Group");
                    }
                }

            }

        protected:
            friend serialize;
            bool openForReading=true;
            bool openForWriting=false;
    private:
        };


    class VariableDyn:public core::TreeNodeItem{
    public:
        VariableDyn():TreeNodeItem(){}
        VariableDyn(std::string name):TreeNodeItem(name){}
        VariableDyn(std::string name, core::Attributes && attr):TreeNodeItem(name,std::move(attr)){}
        VariableDyn(core::TreeNodeRef && in):TreeNodeItem(std::move(in)){}
//        virtual guides:typehash typehash()const{return guides::typehash();}

        constexpr bool readable()const{
            return static_cast<Group*>(getParent())->readable();
        }

        constexpr bool writable()const {
            return static_cast<Group *>(getParent())->writable();
        }



    };

//        using valueVariant=boost::variant<double,int,long long int,std::string,OGRGeometry*>;
    template<class T>
    class Variable:public VariableDyn{
    public:
        using single=T;
        using singlePtr=std::shared_ptr<single>;
        virtual int ndim(){return 1;}
        Variable(core::TreeNodeRef && in):VariableDyn(std::move(in)){}
        Variable():VariableDyn(){}
        Variable(std::string name):VariableDyn(name){}
        Variable(std::string name, core::Attributes && attr):VariableDyn(name,std::move(attr)){}
        void setValue(const single & val,const ptrdiff_t idx) {
            //here we construct a shared_ptr but don't dealloacate the actual value after calling by using an alias constructor
            setValue(singlePtr(singlePtr(), const_cast<single*>(&val)),idx);
        }
        virtual void getValue(singlePtr & in,const ptrdiff_t idx)const{THROWMETHODEXCEPTION("getValue not implemented");}
        virtual void setValue(const singlePtr & val,const ptrdiff_t idx){THROWMETHODEXCEPTION("setValue not implemented");}
        virtual void setValue(const core::Hyperslab<T> & hslab){THROWMETHODEXCEPTION("hyperslab writing not supported");}
        virtual void getValue(core::Hyperslab<T> & hslab){THROWMETHODEXCEPTION("hyperslab reading not supported");}


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



    }

}



#endif //FROMMLE_GROUPBASE_HPP
