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
#include <boost/serialization/split_free.hpp>
#include <boost/variant.hpp>


#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP
//fowrward declare classes contained within the variant class below
class OGRGeometry;

namespace frommle{
    namespace io{

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
    class Group:public core::TreeNodeCollection {
        public:
            Group():TreeNodeCollection(){}
            //construct when only the groupname is known
            Group(const std::string & name ):TreeNodeCollection(name){}
            Group(const std::string && name ):TreeNodeCollection(std::move(name)){}
            Group(const std::string name, core::Attribs && attr):TreeNodeCollection(name,std::move(attr)){}
            //construct when both the name and parent are known
            template<class P>
            Group(const std::string & name, const P * const parent ):TreeNodeCollection(name,parent){}

            //@brief convenience function to get the first OGR 'geom' variable in a group (defautl takes the one called 'geom')
            virtual core::TreeNodeRef & geomVar(){return this->operator[]("geom");};

            //serialization goodies (objects can be read/written to groups by implementing a serialization function
            template<class Y>
            Group & operator >> (Y & out){boost::serialization::serialize(*this,out,file_version()); return *this;}

//            typedef boost::mpl::bool_<true> is_saving;
//            typedef boost::mpl::bool_<true> is_loading;
//            using const_iterator = core::iteratorWrap<Variable,cvec::const_iterator>;
//            using iterator = core::iteratorWrap<Variable,cvec::iterator>;
//            const_iterator cbegin()const{ return const_iterator(this->TreeNodeCollection::cbegin());}
//            const_iterator cend()const{return const_iterator(this->TreeNodeCollection::cend()); }
//            iterator begin(){return iterator(this->TreeNodeCollection::begin()); }
//            iterator end(){return iterator(this->TreeNodeCollection::end()); }

            bool readable()const{return is_loading;}
            bool writable()const{return is_saving;}
            void setAmode(const std::string & mode){

                if (mode == "r"){
                    is_loading=true;
                    is_saving=false;
                }else if( mode == "w"){
                    is_loading=false;
                    is_saving=true;
                }else if(mode == "rw"){
                    is_loading=true;
                    is_saving=true;
                }else{
                    throw core::InputException("cannot understand the access mode of the group");
                }

            }
            void setAmode(){
                auto ogrgrp=static_cast<Group*>(getParent());
                if (ogrgrp){
                    //take the accessmode from above
                    ogrgrp->setAmode();
                    is_loading=ogrgrp->readable();
                    is_saving=ogrgrp->writable();
                }

            }

        protected:
            friend boost::serialization::access;

            //needed to be compatible with the boost serialization library(don't ask me why)
            void load_binary(void * address,std::size_t count){assert(0);};
            template <class T>
            core::TreeNodeRef & operator & ( T & t){
                return *this >> t;
            }

            virtual unsigned int file_version(){return 0;};
    private:
            bool is_loading=true;
            bool is_saving=false;
        };

    using valueVariant=boost::variant<double,int,long long int,std::string,std::unique_ptr<OGRGeometry>>;
    template<class T=valueVariant>
    class Variable:public core::TreeNodeItem{
    public:
        using single=T;
        using singlePtr=std::shared_ptr<single>;

        Variable():TreeNodeItem(){}
        Variable(const std::string & name):TreeNodeItem(name){}
        Variable(const std::string name, core::Attribs && attr):TreeNodeItem(name,std::move(attr)){}
        virtual singlePtr getValue(const size_t idx=-1)const=0;
        virtual void setValue(singlePtr & val,const size_t idx)=0;
        virtual void push_back(singlePtr &val){setValue(val,-1);}
        singlePtr operator[](const size_t idx)const{return getValue(idx);}
        bool readable()const{return static_cast<const Group *>(getParent())->readable();}
        bool writable()const{return static_cast<const Group *>(getParent())->writable();}
        ///@brief iterator which loops over the values in this variable
        class iterator{
        public:
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = single;
            using difference_type = std::ptrdiff_t;
            using pointer = single*;
            using reference = single&;
            iterator& operator++(){value_=parent_->getValue(-1);return *this;};
            bool operator==(const iterator & other) const {return value_ == other.value_;}
            bool operator!=(const iterator & other) const {return !(*this == other);}
            single & operator*() {return *value_;}
            iterator(){}
            iterator(singlePtr firstValue, Variable *parent):value_(firstValue),parent_(parent){}
        protected:
            //note the iterator may not necessarily own the resource of the pointer!!
            singlePtr value_{};
        private:
            const Variable *parent_{};
        };
        iterator begin(){return iterator(getValue(),this);}
        iterator end(){return iterator();}
    private:

    };

    }

}



#endif //FROMMLE_GROUPBASE_HPP
