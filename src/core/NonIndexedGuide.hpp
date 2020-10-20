/*! \file
 \brief 
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

#ifndef SRC_CORE_NONINDEXEDGUIDE_HPP_
#define SRC_CORE_NONINDEXEDGUIDE_HPP_

namespace frommle{

    namespace guides{
    //tags for indexing by id or element

    template<class ELEM>
        class NonIndexedGuide:public GuideBase{
        public:
        using Element=ELEM;
        NonIndexedGuide()=default;        
        NonIndexedGuide(std::string name):GuideBase(name){}
        
        //derived type needs to implement fullsize() (size of unmasked guide) 
        virtual size_t fullsize()const =0;
        //derived type needs to implement elfromi, and ifromel
        virtual Element elfromi(size_t i)const=0;
        virtual size_t  ifromel(const ELEM & elem)const=0;
        
        size_t size()const override{ return fullsize()-nmask_;}
        
        

        inline bool isMasked(const Element &el)const{
           if (maskf_){
                return maskf_(el);
           }
           return false;
        }
        
        ELEM operator[](ptrdiff_t i)const{

            if( i < 0){
                //Ala python look at the back of the container
                i+=size();
            }else if (i >= size()){
                THROWINDEXCEPTION("out of range");
            }
            
            if (nmask_ == 0){
                return elfromi(i);
            }
            size_t ith=0; 
            //we need to find the corresponding unmasked element 
            for(size_t ii=0;ii< fullsize();++ii){
                auto el=elfromi(ii);
                if (isMasked(el)){
                    continue;
                }else{
                    if(ith == i){
                        //found it!
                        return el;
                    }else{
                        //we hit an unmasked value but need to search further
                        ++ith;
                        continue;
                    }

                }
            
            }
           //if we arrive here we didn't find a umasked value with the index 
                THROWINDEXCEPTION("masked value out of range");
        }
        class const_iterator{
        public:
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = const ELEM;
            using difference_type = std::ptrdiff_t;
            using pointer = const ELEM*;
            using reference = const ELEM&;
            const_iterator()=delete;
            ~const_iterator(){
                
            }

            const_iterator & operator = (const const_iterator & itin){
                idx_=itin.idx_;
                fullidx_=itin.fullidx_;
                sz_=itin.sz_;
                gd_ptr=itin.gd_ptr;
                ElementCache_=itin.ElementCache_;
            }
            const_iterator(const const_iterator & in):idx_(in.idx_),fullidx_(in.fullidx_),sz_(in.sz_),fullsz_(in.fullsz_),nmask_(in.nmask_),gd_ptr(in.gd_ptr),ElementCache_(in.ElementCache_){
            }
            const_iterator(const NonIndexedGuide * gd, ptrdiff_t advance=0):idx_(-1),fullidx_(-1),fullsz_(gd->fullsize()),sz_(gd->size()),gd_ptr(gd){
                
                if(sz_ <= advance){
                    //all values already encountered , snap to 1 past the end
                    idx_=sz_;
                    fullidx_=fullsz_;
                    return;
                }


                if( 0 < gd_ptr->nmask_ ){
                    
                    //we need to pass through the iterator and test for masked values on the way to know at which idx_ we are
                    
                    do{
                        ++(*this);
                    }while(idx_ < advance);

                }else{
                    //nothing is masked
                    //idx_ == fullidx_ when nothin is masked
                    idx_=advance;
                    fullidx_=idx_;
                    ElementCache_=gd_ptr->elfromi(idx_);
                }


            }

            const_iterator& operator++(){
               ++fullidx_; 
               
               if (fullidx_ < fullsz_){
                   ElementCache_=gd_ptr->elfromi(fullidx_);
               }else{
                   //quick return when we've reached the end of the iterator
                   idx_=sz_; 
                   return *this;
               }

                            
                if (gd_ptr->maskf_){
                    //skip forward through masked entries
                    while( gd_ptr->maskf_(ElementCache_)){
                        ++fullidx_;
                        if(fullsz_ == fullidx_){
                            //reached the end quick return
                            idx_=sz_;
                            return *this;
                        }
                        ElementCache_=gd_ptr->elfromi(fullidx_);      
                    }

                }
                ++idx_;
                
                return *this;
            }

            const_iterator operator++(int){
                const_iterator retval(*this);
                ++(*this);
                return retval;
            }

            bool operator==(const_iterator & other) const {return idx_ == other.idx_;}
            bool operator!=(const_iterator & other) const {return !(*this == other);}


            ELEM & operator*()const{
                return ElementCache_;
            }


            const_iterator operator+(ptrdiff_t advance)const{
                ptrdiff_t advnew=idx_+advance;

                if ( advnew < 0){
                    THROWINPUTEXCEPTION("Advance moves iterator before start");

                }else if(advnew > sz_){
                    //don't complain but snap to 1 past the end
                    advnew=sz_;
                }

                return const_iterator(gd_ptr,advnew);
            }

        protected:
            ptrdiff_t idx_=0;
            ptrdiff_t fullidx_=0;
            size_t sz_=0;
            size_t fullsz_=0;
            size_t nmask_=0;
            //note the iterator does not own the guide pointer so we just use a normal one here
            const NonIndexedGuide* gd_ptr=nullptr;
            mutable Element ElementCache_;
        private:
        };
        
            const_iterator begin()const{
                return const_iterator(this);
            }

            const_iterator end()const{
            //return iterator shifted to one passed the end
                return const_iterator(this,fullsize());
            }

        void mask (std::function<bool(const Element &)> maskf){
            if( maskf){
                THROWINPUTEXCEPTION("Cannot currently apply 2 mask functions");
            }
            maskf_=maskf;

            //check condition and increase nmask
            for(const auto el:*this){
                if (maskf_(el)){
                    ++nmask_;
                }
            }

        }            

       void unmask(){
            maskf_=maskf_t();
            nmask_=0;
       }


        private:
        using maskf_t=std::function<bool(const Element &)>; 
        maskf_t maskf_{};
        size_t nmask_=0; 
        };
    }
}



#endif /*  SRC_CORE_NONINDEXEDGUIDE_HPP_ */
