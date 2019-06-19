/*! \file
 \brief iterate over child elements (e.g. points, linear rings, etc) within an OGRGeometry class
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
#include <ogrsf_frmts.h>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#ifndef FROMMLE_OGRITERATORBASE_HPP
#define FROMMLE_OGRITERATORBASE_HPP
namespace frommle{
    namespace geometry{

        template<class T,class P>
        class OGRPiterator:public boost::iterator_facade<OGRPiterator<T,P>,T,std::random_access_iterator_tag,T>{
        public:

            OGRPiterator(const P * linelike ):parent_(linelike),id_(0),ncount_(linelike->getNumPoints()){
            }
            OGRPiterator(){}
//            OGRPiterator(const OGRPiterator & other)=default;
//            {
//                id_=other.id_;
//                ncount_=other.ncount_;
//                Derivptr_=other.Derivptr_;
//            }
//            OGRiteratorBase & operator = (const OGRiteratorBase & other){
//                id_=other.id_;
//                ncount_=other.ncount_;
//                Derivptr_=other.Derivptr_;
//                return *this;
//            }

            ptrdiff_t size()const{return ncount_;}
        private:
            friend class boost::iterator_core_access;
            ptrdiff_t id_=0;
            ptrdiff_t ncount_=0;
            const P * parent_=nullptr;

//            T & dereference()const {return Derivptr_->dereference();}
            bool equal(const OGRPiterator & other)const{
                return id_==other.id_;
            }
            void increment(){
                ++id_;
            }
            void decrement() {
                --id_;
            }

            void advance(ptrdiff_t n){
                id_+=n;
            }
            ptrdiff_t distance_to(const OGRPiterator& other) const{return other.id_-id_;};
            OGRPoint dereference()const{
                auto ogrpnt=std::make_shared<OGRPoint>(0,0,0);
                parent_->getPoint(id_,ogrpnt.get());

                return *ogrpnt;

            }

        };


        //T iterator element
    template<class Derived,class T,class P>
    class OGRiteratorBase:public boost::iterator_facade<OGRiteratorBase<Derived,T,P>,T,std::random_access_iterator_tag>{
        public:

            OGRiteratorBase(const Derived * Derivptr, ptrdiff_t count):Derivptr_(Derivptr),ncount_(count){
            }
            OGRiteratorBase(){}
            virtual ~OGRiteratorBase(){}
            OGRiteratorBase(const OGRiteratorBase & other){
                id_=other.id_;
                ncount_=other.ncount_;
                Derivptr_=other.Derivptr_;
            }
        OGRiteratorBase & operator = (const OGRiteratorBase & other){
            id_=other.id_;
            ncount_=other.ncount_;
            Derivptr_=other.Derivptr_;
            return *this;
        }

            ptrdiff_t id()const{return id_;}
            ptrdiff_t size()const{return ncount_;}
        private:
            ptrdiff_t id_=0;
            ptrdiff_t ncount_=0;
            const Derived * Derivptr_=nullptr;

            friend class boost::iterator_core_access;
//            T & dereference()const {return Derivptr_->dereference();}
            bool equal(const OGRiteratorBase & other)const{
                return id_==other.id_;
            }
            void increment(){
                ++id_;
            }
            void decrement() {
                --id_;
            }

            void advance(ptrdiff_t n){
                id_+=n;
            }
            ptrdiff_t distance_to(const OGRiteratorBase& other) const{return other.id_-id_;};
        };

        //some implementations (class specializations using iterator_adaptor)
        template<class OGRlinelike>
        class OGRiterator:public boost::iterator_adaptor<OGRiterator<OGRlinelike>,OGRiteratorBase<OGRiterator<OGRlinelike>,OGRPoint,OGRlinelike>>{
        public:
            using btype=OGRiteratorBase<OGRiterator<OGRlinelike>,OGRPoint,OGRlinelike>;
            using adaptor=boost::iterator_adaptor<OGRiterator,btype>;
            OGRiterator(OGRlinelike * parent):adaptor(btype(this,parent->getNumPoints())),parent_(parent) {
            }
            OGRiterator(){}
            OGRiterator(const OGRiterator & other):adaptor(btype(&other,other.parent_->getNumPoints())),parent_(other.parent_){
            }
//            OGRiterator & operator=(OGRiterator & other){
//                                parent_=other.parent_;
//                return *this;
//            }
            using adaptor::base_reference;
            ptrdiff_t size()const{return base_reference().size();}
            OGRPoint dereference(){
                OGRPoint ogrpnt{};
                parent_->getPoint(base_reference().id(),&ogrpnt);
                return ogrpnt;
            }
        private:
            OGRlinelike* parent_=nullptr;
        };


        template<class OGRlinelike>
        class const_OGRiterator:public boost::iterator_adaptor<const_OGRiterator<OGRlinelike>,OGRiteratorBase<const_OGRiterator<OGRlinelike>,const OGRPoint,OGRlinelike>>{
        public:
            using btype=OGRiteratorBase<const_OGRiterator<OGRlinelike>,const OGRPoint,OGRlinelike>;
            using adaptor=boost::iterator_adaptor<const_OGRiterator,btype>;
            const_OGRiterator(const OGRlinelike * parent ):adaptor(btype(this,parent->getNumPoints())),parent_(parent){
            }
            const_OGRiterator(){}
            const_OGRiterator(const const_OGRiterator & other):adaptor(btype(&other,other.parent_->getNumPoints())),parent_(other.parent_){
            }
//            const_OGRiterator & operator=(const_OGRiterator & other){
//                parent_=other.parent_;
//                return *this;
//            }
            using adaptor::base_reference;
            ptrdiff_t size()const{return base_reference().size();}
            OGRPoint  dereference()const{
                OGRPoint ogrpnt{};
                parent_->getPoint(base_reference().id(),&ogrpnt);
                return ogrpnt;


            }
        private:
            const OGRlinelike * parent_=nullptr;
        };

//        //partial template specialization when the parent is a OGRpolygon (iterates over inner ringsi, instead of points)
//        template<>
//    class OGRiterator<OGRPolygon>:public boost::iterator_adaptor<OGRiterator<OGRPolygon>,OGRiteratorBase<OGRiterator<OGRPolygon>,OGRLinearRing,OGRPolygon>>{
//        public:
//            using btype=OGRiteratorBase<OGRiterator<OGRPolygon>,OGRLinearRing,OGRPolygon>;
//            OGRiterator(OGRPolygon * parent):OGRiterator::iterator_adaptor_(btype(this,parent->getNumInteriorRings())),parent_(parent){
//            }
//            OGRiterator(){}
//        private:
//            OGRPolygon *parent_=nullptr;
//            OGRLinearRing &derefence(){
//                OGRLinearRing * ogrring{};
//                parent_->getInteriorRing(base_reference().id());
//            }
//
//        };
//
//
//        //partial template specialization when the parent is a OGRpolygon (iterates over inner ringsi, instead of points)
//        template<>
//        class const_OGRiterator<OGRPolygon>:public boost::iterator_adaptor<const_OGRiterator<OGRPolygon>,OGRiteratorBase<const_OGRiterator<OGRPolygon>,const OGRLinearRing,OGRPolygon>>{
//        public:
//            using btype=OGRiteratorBase<const_OGRiterator<OGRPolygon>,const OGRLinearRing,OGRPolygon>;
//            const_OGRiterator(const OGRPolygon * parent):const_OGRiterator::iterator_adaptor_(btype(this,parent->getNumInteriorRings())),parent_(parent){
//        }
//            const_OGRiterator(){}
//        private:
//            const OGRPolygon * parent_=nullptr;
//            const OGRLinearRing & dereference() {
//                return parent_->getInteriorRing(base_reference().id());
//            }
//
//        };
//
//        template<class T>
//        class OGRiterRange
//        {
//            OGRiterator<T> _begin;
//            OGRiterator<T> _end;
//
//            bool isIterSet=false;
//
//            const_OGRiterator<T> _cbegin;
//            const_OGRiterator<T> _cend;
//
//            bool isCIterSet=false;
//
//        public:
//
//            OGRiterRange(T & parent) : _begin(&parent,false), _end(&parent), isIterSet(true) {
//            }
//
//            OGRiterRange( const T & parent) : _cbegin(&parent,false), _cend(&parent), isCIterSet(true) {
//            }
//
//            OGRiterator<T> begin()
//            {
//                assert(isIterSet);
//                return _begin;
//            }
//
//            const_OGRiterator<T> cbegin() const
//            {
//                assert(isCIterSet);
//                return _cbegin;
//            }
//
//            OGRiterator<T> end()
//            {
//                assert(isIterSet);
//                return _end;
//            }
//
//            const_OGRiterator<T> cend() const
//            {
//                assert(isCIterSet);
//                return _cend;
//            }
//        };
//

    }
}
#endif //FROMMLE_OGRITERATORBASE_HPP
