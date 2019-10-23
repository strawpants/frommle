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

#include <boost/variant.hpp>
#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include "sh/SHGuide.hpp"
#include "geometry/OGRGuide.hpp"

#ifndef FROMMLE_GUIDEREGISTRY_HPP
#define FROMMLE_GUIDEREGISTRY_HPP
namespace frommle{
    namespace guides{

        template<class ... T>
        struct GuideTlist{
            static const int nguides=sizeof...(T);
            using Gvar=boost::variant<std::shared_ptr<T>...>;
//            using Elvar=boost::variant<typename T::Element...>;
        };


        using GuideRegistry=GuideTlist<IndexGuide,DateGuide,SHnmHalfGuide,SHtmnGuide,OGRPointGuide,OGRPolyGuide,GuideBase>;


     //some useful visitors to be applied to the boost variant of the registered guides
    class gvar_size:public boost::static_visitor<size_t>{
    public:
        template<class G>
        size_t operator()(const G & guide)const{
            return guide->size();
        }
    };


        class gvar_baseptr: public boost::static_visitor<GuideBasePtr>{
        public:
            template<class T>
            GuideBasePtr operator()(T & gvar)const{
                return std::static_pointer_cast<GuideBase>(gvar);
            }

        };



        template<class Element>
    class gvar_idx:public boost::static_visitor<size_t>{
    public:
        gvar_idx(Element el):el_(el){}
//        size_t operator()(const std::shared_ptr<SHtmnGuide> & gvar)const{
//            return gvar->idx(el_);
//        }

        template<class T>
                size_t operator()(const T & gvar)const{
                //the correct function will be selected based on whether Element is a member of T
                return idx<T,Element>(gvar);
        }

        Element el_;

        template<class G,class El,typename std::enable_if<std::is_same<El,typename G::element_type::Element>::value,int>::type =0>
        size_t idx(const G &gvar)const{
            return gvar->idx(el_);
        }

        template<class G,class El,typename std::enable_if<!std::is_same<El,typename G::element_type::Element>::value,int>::type =0>
        size_t idx(const G &gvar)const{
            THROWNOTIMPLEMENTED(std::string("cannot get index by Element from this guide").append(gvar->name()));
            return 0;
        }

    };
//        template<class P>
//    class gvar_asptr: public boost::static_visitor<std::shared_ptr<P>>{
//        public:
//            template<class T>
//            std::shared_ptr<P> operator()(T & gvar)const{
//                return std::static_pointer_cast<P>(gvar);
//            }
//
//        };

        //!@brief forward iterator which wraps around  a std:array of guide variants
        class GVIterator{
        public:
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = GuideBasePtr;
            using difference_type = std::ptrdiff_t;
            using pointer = GuideBasePtr;
            using reference = GuideBase&;
            explicit GVIterator(const GuideRegistry::Gvar * el ):el_(el){}
            GVIterator & operator++(){
                ++el_;
            }
            GVIterator operator++(int){
                GVIterator retval(*this);
                ++(*this);
                return retval;
            }

            bool operator==(GVIterator & other) const {return el_ == other.el_;}
            bool operator!=(GVIterator & other) const {return !(*this == other);}
            GuideBasePtr operator*()const {return boost::apply_visitor(gvar_baseptr(),*el_);}
        protected:
            const GuideRegistry::Gvar* el_={};
//        Element elStop={};
        private:
        };
    }


}



#endif //FROMMLE_GUIDEREGISTRY_HPP
