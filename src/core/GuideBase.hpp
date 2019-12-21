/*! \file
 \brief Holds the base for the Dimension class
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

#ifndef SRC_CPP_DIMENSIONBASE_HPP_
#define SRC_CPP_DIMENSIONBASE_HPP_

#include <string>
#include <vector>
//#include <boost/iterator/iterator_adaptor.hpp>
#include <memory>
#include <cmath>
#include "core/Logging.hpp"
#include "core/frommle.hpp"
namespace frommle {
namespace guides {
    template<class G>
    class MaskedGuide;


/*!
 * \brief Base class for describing  guided dimensions
 * The general idea behind the Guide classes is that they are used to add value to the dimension of an array/vector
 * Derived classes are expected to provide the following functionality:
 * -# Encapsulate functions (notably index functions) which are specific to the dimension
 * -# Allow serialization to read from/write to archives and other processes
 * -# Keep track of permutations so that an underlying linked array may be permuted but the indexing stays the same
 * -# Provide ways to compare and permute with other Dimensions so that they can be sorted relative to each other
 * -# Can be used as template arguments in order to select the correct functions at compile time (e.g. an observation equation)
 * Please see the class \ref TimeGuide or \ref IndexGuide for an example of a derived class
 * This abstract base class makes sure that the pure virtual functions are implemented and that
 * containers can store (smart) pointers to DimensionBase
 */





class GuideBase:public core::Frommle {
    public:
        using typehash=core::typehash;
        GuideBase():Frommle("GuideBase") {};
        GuideBase(const core::typehash &type, const size_t &sz) :Frommle("GuideBase",type), size_(sz) {}
        GuideBase(const std::string & name, const core::typehash &type, const size_t &sz) : Frommle(name,type), size_(sz){}

        GuideBase(const core::typehash &type) : Frommle("GuideBase",type) {}
        GuideBase(const std::string & name,const core::typehash &type):Frommle(name,type){}
        GuideBase(const std::string & name,size_t sz):Frommle(name),size_(sz){}
        virtual ~GuideBase() {
        }
        using Element=std::string;
        //implict conversion to a maskedversion
        template<class G>
        explicit   operator MaskedGuide<G>();
        
        size_t size() const { return size_; }

//        virtual bool operator==(const GuideBase &in) const {
//            return type_ == in.type_;
//        }
//
//        bool operator!=(const GuideBase &in) const {
//            return not this->operator==(in);
//        }

//        std::string name()const{return name_;}
//        void setName(const std::string &name){
//            name_=name;
//        }
        size_t idx(const Element & el )const{return 0;}
        virtual bool isMasked()const{return false;};
        virtual bool isPermuted()const{return false;};
        //static constexpr int ndim=1;
        virtual std::vector<Element> descriptor()const{
            //default implementation jsut makes a description based ont he typehash
            //allocate space in the vector
            char fmt[12];
            char el[50];
            sprintf(fmt,"%%s_idx%%0%dd",static_cast<int>(std::log10(size_))+1);
            //LOGINFO << fmt << std::endl;
            std::vector<Element> desc(size_);
            for(size_t i=0;i<size_;++i){
                sprintf(el,fmt,hash().name().c_str(),i);
                desc[i]=el;  
            }
            return desc;
        }
        using const_iterator=std::vector<Element>::const_iterator;
        const_iterator begin()const {
            //although the function is marekd const we do allow the cache variable to be set dynamically 
            const_cast<GuideBase*>(this)->initdesc();
            return descripcache_.begin();}
        const_iterator end()const{
            //although the function is marekd const we do allow the cache variable to be set dynamically 
            const_cast<GuideBase*>(this)->initdesc();
            return descripcache_.end();}
    private:
        void initdesc(){
            if(descripcache_.size() ==0 ){
                descripcache_=descriptor();
            }
        }
    protected:
//        using core::Frommle::type_;
        size_t size_ = 0;
//        std::string name_="Guide";
        std::vector<Element> descripcache_{}; 
    };


        template<class GD>
        class GuideIterator{
        public:
            //iterator traits
            using Element=typename GD::Element;
            using iterator_category = std::forward_iterator_tag;
            using value_type = Element;
            using difference_type = std::ptrdiff_t;
            using pointer = Element*;
            using reference = Element&;
            GuideIterator()=delete;
//        explicit Guideiterator(Element el, Element elstop):elVal(el),elStop(elstop){}
            GuideIterator(GD * guide,ptrdiff_t advance=0):idx_(advance),sz_(guide->size()),g_ptr(guide){
//                loadElVal();
            }
            GuideIterator(const GuideIterator & giter, ptrdiff_t advance=0):idx_(giter.idx_+advance),sz_(giter.sz_),g_ptr(giter.g_ptr){
                //truncate idx_ to 1 past the end
                idx_=idx_>sz_?sz_:idx_;
//                loadElVal();
            }

            GuideIterator& operator++(){
                ++idx_;
                idx_=idx_>sz_?sz_:idx_;
//                loadElVal();
                return *this;
            }

            GuideIterator operator++(int){
                GuideIterator retval(*this);
                ++(*this);
                return retval;
            }

            bool operator==(GuideIterator & other) const {return idx_ == other.idx_;}
            bool operator!=(GuideIterator & other) const {return !(*this == other);}


            Element & operator*();

            GuideIterator operator+(ptrdiff_t advance)const{
                return GuideIterator(this,advance);
            }

            ~GuideIterator(){}
        protected:
            size_t idx_=0;
            size_t sz_=0;
            //note the iterator does not own the guide pointer so we just use a normal one here
            GD* g_ptr=nullptr;
//            Element elVal{};
        private:
        };

        template<class GD, class Element>
        class InjectGuideIterator{
        public:
            using const_iterator=GuideIterator<const GD>;
            using iterator=GuideIterator<GD>;

            const_iterator begin()const{
                return const_iterator(dynamic_cast<const GD*>(this));
            }

            const_iterator end()const{
            //return iterator shifted to one passed the end
                auto gd=dynamic_cast<const GD*>(this);
                return const_iterator(gd,gd->size());
        }


//            iterator begin(){
//                return iterator(dynamic_cast<GD*>(this));
//            }
//
//            iterator end(){
//                //return iterator shifted to one passed the end
//                auto gd=dynamic_cast<GD*>(this);
//                return iterator(gd,gd->size());
//            }
            //we need at least 1 virtual function here so dynamic casts bask to GD work (so let's make the descructor virtual)
            virtual ~InjectGuideIterator(){};

        private:
            friend const_iterator;
            friend iterator;
            void setEl(Element el)const{
                ElementCache_=el;
            }
            Element & getEl()const{return ElementCache_;}
            mutable Element ElementCache_;

        };



        template<class GD>
        typename GD::Element & GuideIterator<GD>::operator*()
        {
            if (idx_ >= sz_){
                THROWINPUTEXCEPTION("Dereferencing non-existent Element in GuideIterator");
            }
            g_ptr->setEl(g_ptr->operator[](idx_));
            return g_ptr->getEl();
        }


    using GuideBasePtr=std::shared_ptr<GuideBase>;
    using constGuideBasePtr=std::shared_ptr<const GuideBase>;


    ///@brief this masked guide wraps another guide while masking part of it
    template<class G>
class MaskedGuide:public GuideBase{
public:
    //create iterators to iterate over the contained guide while skipping masked values
    using Element=typename G::Element;
    using GuideBase::size_;
    void mask(const ptrdiff_t idx=-1){
        if (idx!=-1){
            valid_[idx]=0;
            --size_;
        }else{
            //mask everything
            std::fill(valid_.begin(),valid_.end(),0);
            size_=0;
        }
    }
    void unmask(const ptrdiff_t idx=-1){
        if (idx!=-1){
            valid_[idx]=1;
            ++size_;
        }else{
            //mask everything
            std::fill(valid_.begin(),valid_.end(),1);
            size_=valid_.size();
        }
    }

    MaskedGuide(const G & in):guide_(&in),valid_(in.size(),1){

    }
//    void togglemask(const ptrdiff_t idx =-1){}
    MaskedGuide & operator=(const G & gin){
        *this=MaskedGuide(gin);
        return *this;
    }
    bool isMasked()const{return true;}
private:
    const G* guide_=nullptr;
    std::vector<int> valid_{};

};


///@brief guide which has a length of 1 (used to track dimensions which are to be reduced)
class SingleGuide: public GuideBase{
    public:
        using Element=size_t;
        SingleGuide(){}
        SingleGuide(const size_t idx ):idx_(idx){}
        inline size_t idx(){return idx_;}
    private:
        size_t idx_=0;
};

template<class G>
GuideBase::operator MaskedGuide<G>(){
            return MaskedGuide<G>(static_cast<const G&>(*this));
}


}
}

#endif /* SRC_CPP_DIMENSIONBASE_HPP_*/
