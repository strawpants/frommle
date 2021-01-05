/*! \file
 \brief Contains templates to setup mutliindex guides acting as containers
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

#include "core/GuideBase.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
//#include <boost/multi_index/sequenced_index.hpp>
//#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#ifndef FROMMLE_CONTAINERGUIDE_HPP
#define FROMMLE_CONTAINERGUIDE_HPP
using boost::multi_index_container;
namespace bmi=boost::multi_index;

namespace frommle{

    namespace guides{
    //tags for indexing by id or element

    template<class ELEM>
        class IndexedGuide:public GuideBase{
        public:
        IndexedGuide()=default;        
        IndexedGuide(std::string name):GuideBase(name){}
        IndexedGuide(std::string name, const std::vector<ELEM> & invec):GuideBase(name){
            mindx_.reserve(invec.size());
            for (const auto & elem:invec){
                push_back(elem);
            }

        }
        struct indx_t{
            indx_t(ELEM elin, ptrdiff_t iin):el(elin),i(iin){}
            ELEM el;
            ptrdiff_t i;//keeps track of the storage location (negative when masked)

        };

        size_t size()const override{

           return mindx_.size()-nmask_;  

        }
        
        const ELEM & operator[](ptrdiff_t i)const {
            if( i< 0){
                i+=size();
            }
            return mindx_[i].el; 
        }

        template<class ... Args>
        void emplace_back(Args && ... args){
            mindx_.emplace_back(ELEM(std::forward<Args>(args)...),size());
        }
        
        void push_back(ELEM el){
            mindx_.push_back(indx_t(el,size()));
        }
       
        void reserve(size_t sz){mindx_.reserve(sz);}        
        //structs used for taggging indexes
        struct bymem{};//tag reflects insertion order
        struct byi{};//tag reflects by permution/masked order
        struct byel{};//tag reflects order as sorted by the element content
           
        using mindx_t= multi_index_container<
            indx_t,
            bmi::indexed_by<
                bmi::random_access<bmi::tag<bymem>>
                ,bmi::ordered_unique<bmi::tag<byi>,bmi::member<indx_t,ptrdiff_t,&indx_t::i> >
                ,bmi::ordered_unique<bmi::tag<byel>,bmi::member<indx_t,ELEM,&indx_t::el>>
                > > ;
        
        using memindx_t=typename bmi::index<mindx_t,bymem>::type;
        using iindx_t=typename bmi::index<mindx_t,byi>::type;
        using elindx_t=typename bmi::index<mindx_t,byi>::type;
        
        ///@brief an iterator wrapper which exposes only the element but refers to the iterator of  the underlying multiindex
        class const_iterator{
            public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = const ELEM;
            using difference_type = std::ptrdiff_t;
            using pointer = const ELEM*;
            using reference = const ELEM&;
        
            const_iterator(typename iindx_t::iterator it):it_(it){}
            const_iterator& operator++(){
                ++it_;
            return *this;
            }

            const_iterator operator++(int){
                const_iterator retval (*this);
                ++(*this);
                return retval;
            }

            const_iterator operator+(ptrdiff_t advance)const{
                const_iterator retval(*this);
                retval+=advance;
                return retval;
            }

            bool operator==(const const_iterator & other) const {return it_ == other.it_;}
            bool operator!=(const const_iterator & other) const {return !(*this == other);}


            const ELEM & operator*()const {return it_->el;};

            private:
                typename iindx_t::iterator it_;

        };

        const_iterator begin()const{
            //only return entries which are not masked
            return const_iterator(bmi::get<byi>(mindx_).lower_bound(0));
        }

        const_iterator end()const{
            return const_iterator(bmi::get<byi>(mindx_).end());
        }
        
        const memindx_t & memindx() const{
            return bmi::get<bymem>(mindx_);
        }
        
        memindx_t & memindx(){
            return bmi::get<bymem>(mindx_);
        }

        const iindx_t & iindx() const{
            return bmi::get<bymem>(mindx_);
        }

        iindx_t & iindx(){
            return bmi::get<byi>(mindx_);
        }

        const elindx_t & elindx() const{
            return bmi::get<byel>(mindx_);
        }

        elindx_t & elindx(){
            return bmi::get<byel>(mindx_);
        }

        void mask (std::function<bool(const ELEM &)> maskf){
            auto & idx=memindx();
            auto it0=idx.begin();
            auto it1=idx.end();
            for(auto it=it0;it!=it1;++it){
                if (maskf(it->el)){
                    idx.modify(it,[&](indx_t & entry){
                            entry.i=-entry.i-1; 
                        });
                        ++nmask_;
                }
            }
        
        }            

       void unmask(){
            //iterate over negative  i's and change back 
            auto & idx=memindx(); 
            auto it0=idx.begin();
            auto it1=idx.end();
            
            for(auto it=it0;it!=it1;++it){
                    if (it->i < 0){
                        //LOGINFO << it->i << " " << nmask_<<std::endl;
                        idx.modify(it,[&](indx_t & entry){
                            entry.i=-entry.i-1; 
                            });
                        --nmask_;
                    }
             }

       }
        private:
        size_t nmask_=0; 
        mindx_t mindx_;
        };
    }
}


#endif //FROMMLE_CONTAINERGUIDE_HPP
