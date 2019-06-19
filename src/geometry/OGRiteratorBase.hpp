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
        //T iterator element
    template<class Derived,class T,class P>
    class OGRiteratorBase:public boost::iterator_facade<OGRiteratorBase<Derived,T,P>,T,std::random_access_iterator_tag>{
        public:

            OGRiteratorBase(Derived * Derivptr, ptrdiff_t count, const bool end ):Derivptr_(Derivptr),ncount_(count){
                if(end){
                    id_=ncount_;
                }
            }
            OGRiteratorBase(){}
            virtual ~OGRiteratorBase(){}
        private:
            ptrdiff_t id_=-1;
            ptrdiff_t ncount_=-1;
            T * OGRelem_=nullptr;
            Derived * Derivptr_=nullptr;

            friend class boost::iterator_core_access;
//            void setCount(ptrdiff_t count){ncount_=count;}
            T & dereference()const {return *OGRelem_;}
            bool equal(const OGRiteratorBase & other)const{
                return id_==other.id_;
            }
            void increment(){
                ++id_;
                if(id_>= 0 && id_<ncount_)
                {
                    OGRelem_=load(id_);
                }else{
                    id_=ncount_;
                    OGRelem_=nullptr;
                }
            }
            void decrement() {
                --id_;
                if (id_ >= 0) {
                    OGRelem_=load(id_);
                } else {
                    OGRelem_ = nullptr;
                }
            }

            void advance(ptrdiff_t n){
                id_+=n;
                if(id_>=0 && id_ < ncount_) {
                    OGRelem_=load(id_);
                }else{
                    OGRelem_=nullptr;
                }
            }
            ptrdiff_t distance_to(const OGRiteratorBase& other) const{return other.id_-id_;};
            //we need to forawad the load and count call to the dervied type
            inline T * load(const ptrdiff_t ith)const{return Derivptr_->load(ith);}
//            inline ptrdiff_t count()const{return Derivptr_->count();}
        };

        //some implementations (class specializations using iterator_adaptor)
        template<class OGRlinelike>
        class OGRiterator:public boost::iterator_adaptor<OGRiterator<OGRlinelike>,OGRiteratorBase<OGRiterator<OGRlinelike>,OGRPoint,OGRlinelike>>{
        public:
            using btype=OGRiteratorBase<OGRiterator<OGRlinelike>,OGRPoint,OGRlinelike>;

            OGRiterator(OGRlinelike * parent,bool end=true):OGRiterator<OGRlinelike>::iterator_adaptor_(btype(this,parent->getNumPoints(),end)),parent_(parent) {
                if(not end){
                    ++*this;
                }
            }
            OGRiterator(){}
        private:
            friend btype;
            OGRlinelike* parent_=nullptr;
            OGRPoint * load(const ptrdiff_t ith) {
                parent_->getPoint(ith,&ogrpnt_);
                return &ogrpnt_;
            }
//            ptrdiff_t count()const{return parent_->getNumPoints();}
            OGRPoint ogrpnt_={};
        };


        template<class OGRlinelike>
        class const_OGRiterator:public boost::iterator_adaptor<const_OGRiterator<OGRlinelike>,OGRiteratorBase<const_OGRiterator<OGRlinelike>,const OGRPoint,OGRlinelike>>{
        public:
            using btype=OGRiteratorBase<const_OGRiterator<OGRlinelike>,const OGRPoint,OGRlinelike>;
            const_OGRiterator(const OGRlinelike * parent, bool end=true ):const_OGRiterator<OGRlinelike>::iterator_adaptor_(btype(this,parent->getNumPoints(),end)),parent_(parent){
                ogrpnt_=std::make_shared<OGRPoint>(0,0,0);
                if(not end){
                    ++*this;
                }
            }
            const_OGRiterator(){}
            const_OGRiterator(const const_OGRiterator & other){
                parent_=other.parent_;
                ogrpnt_=other.ogrpnt_;

            }
            const_OGRiterator & operator=(const_OGRiterator & other){
                parent_=other.parent_;
                ogrpnt_=other.ogrpnt_;
                return *this;
            }
        private:
            friend btype;
            const OGRlinelike * parent_=nullptr;
            OGRPoint * load(const ptrdiff_t ith){
//                doubldde x,y,z;

//                OGRGeometryFactory::destroyGeometry(&ogrpnt_);
                parent_->getPoint(ith,ogrpnt_.get());
//                x=ogrpnt_->getX();
//                y=ogrpnt_->getY();
//                z=ogrpnt_->getZ();
//                char** wkt=new char*;
//                parent_->exportToWkt(wkt);

                return ogrpnt_.get();
            }
            std::shared_ptr<OGRPoint> ogrpnt_={};
        };

        //partial template specialization when the parent is a OGRpolygon (iterates over inner ringsi, instead of points)
        template<>
    class OGRiterator<OGRPolygon>:public boost::iterator_adaptor<OGRiterator<OGRPolygon>,OGRiteratorBase<OGRiterator<OGRPolygon>,OGRLinearRing,OGRPolygon>>{
        public:
            using btype=OGRiteratorBase<OGRiterator<OGRPolygon>,OGRLinearRing,OGRPolygon>;
            OGRiterator(OGRPolygon * parent, bool end=true):OGRiterator::iterator_adaptor_(btype(this,parent->getNumInteriorRings(),end)),parent_(parent){
                if(not end){
                    ++*this;
                }

            }
            OGRiterator(){}
        private:
            friend btype;
            OGRPolygon *parent_=nullptr;
            OGRLinearRing * load(const ptrdiff_t ith) {
                return parent_->getInteriorRing(ith);
            }
//            ptrdiff_t count()const{return parent_->getNumInteriorRings();}

        };


        //partial template specialization when the parent is a OGRpolygon (iterates over inner ringsi, instead of points)
        template<>
        class const_OGRiterator<OGRPolygon>:public boost::iterator_adaptor<const_OGRiterator<OGRPolygon>,OGRiteratorBase<const_OGRiterator<OGRPolygon>,const OGRLinearRing,OGRPolygon>>{
        public:
            using btype=OGRiteratorBase<const_OGRiterator<OGRPolygon>,const OGRLinearRing,OGRPolygon>;
            const_OGRiterator(const OGRPolygon * parent,bool end=true):const_OGRiterator::iterator_adaptor_(btype(this,parent->getNumInteriorRings(),end)),parent_(parent){
            if(not end){
                ++*this;
            }
        }
            const_OGRiterator(){}
        private:
            friend btype;
            const OGRPolygon * parent_=nullptr;
            const OGRLinearRing * load(const ptrdiff_t ith) {
                return parent_->getInteriorRing(ith);
            }
//            ptrdiff_t count()const{return parent_->getNumInteriorRings();}

        };

        template<class T>
        class OGRiterRange
        {
            OGRiterator<T> _begin;
            OGRiterator<T> _end;

            bool isIterSet=false;

            const_OGRiterator<T> _cbegin;
            const_OGRiterator<T> _cend;

            bool isCIterSet=false;

        public:

            OGRiterRange(T & parent) : _begin(&parent,false), _end(&parent), isIterSet(true) {
            }

            OGRiterRange( const T & parent) : _cbegin(&parent,false), _cend(&parent), isCIterSet(true) {
            }

            OGRiterator<T> begin()
            {
                assert(isIterSet);
                return _begin;
            }

            const_OGRiterator<T> cbegin() const
            {
                assert(isCIterSet);
                return _cbegin;
            }

            OGRiterator<T> end()
            {
                assert(isIterSet);
                return _end;
            }

            const_OGRiterator<T> cend() const
            {
                assert(isCIterSet);
                return _cend;
            }
        };


    }
}
#endif //FROMMLE_OGRITERATORBASE_HPP
