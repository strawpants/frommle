/*! \file
 \brief Provides an abstract base to work with groups and variables which can be sued to navigate in Archives
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

#ifndef FROMMLE_ARITEM_HPP
#define FROMMLE_ARITEM_HPP
namespace frommle{
    namespace io{
        //We only need to forwad declare this class here
        class InputArchiveBase;
        template<class T,class P>
        class ArItemiterator;
        class Group;
        class VarItem;
        class ValueItem;

        class ArItemBase{
            public:
                ArItemBase(){}
                //construct when only the groupname is known
                ArItemBase(const std::string & name ):name_(name){}
                ArItemBase(const ArItemBase & other){
                        name_=other.name_;
                        id_=other.id_;

                }
                void setName(const std::string name){name_=name;}
                std::string getName()const{return name_;}
                virtual ArItemBase & operator ++(){++id_;}

            protected:
                friend ArItemiterator<Group,InputArchiveBase>;
                friend ArItemiterator<VarItem,Group>;
                friend ArItemiterator<ValueItem,VarItem>;
                std::string name_="";
                ptrdiff_t id_=-1;
                //possibly add attributes and variables names here
        private:
        };

        template<class T, class P>
        class ArItemiterator{
        public:
            //iterator traits
            using AritemRef=std::shared_ptr<T>;
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;
            using Parent=P;
            ArItemiterator& operator++(){++*aritemPtr_;return *this;};
            bool operator==(const ArItemiterator & other) const {return aritemPtr_->id_ == other.aritemPtr_->id_;}
            bool operator!=(const ArItemiterator & other) const {return !(*this == other);}
            T * operator*() {return aritemPtr_.get();}
            ~ArItemiterator(){}
            ArItemiterator(const ArItemiterator & in){
                aritemPtr_=in.aritemPtr_;
            }
            ArItemiterator();
            ArItemiterator(const P* const parentPtr);



        private:
            AritemRef  aritemPtr_{};
        };


    }


}

#endif //FROMMLE_ARITEM_HPP
