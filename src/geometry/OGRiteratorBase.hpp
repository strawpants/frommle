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
            ptrdiff_t size()const{return ncount_;}
        private:
            friend class boost::iterator_core_access;
            ptrdiff_t id_=0;
            ptrdiff_t ncount_=0;
            const P * parent_=nullptr;
            bool equal(const OGRPiterator & other)const{
                return parent_ == other.parent_ && id_==other.id_;
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
                if(parent_){
                    parent_->getPoint(id_,ogrpnt.get());
                }

                return *ogrpnt;

            }
        };

        template<class T,class P>
        class OGRRingIterator:public boost::iterator_facade<OGRRingIterator<T,P>,T,std::random_access_iterator_tag,T>{
        public:

            OGRRingIterator(const P * Poly ):parent_(Poly),id_(0),ncount_(Poly->getNumInteriorRings()+1){
            }
            OGRRingIterator(){

            }
            ptrdiff_t size()const{return ncount_;}
        private:
            friend class boost::iterator_core_access;
            ptrdiff_t id_=0;
            ptrdiff_t ncount_=0;
            const  P * parent_=nullptr;
            bool equal(const OGRRingIterator & other)const{
                return parent_ == other.parent_ && id_==other.id_;
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
            ptrdiff_t distance_to(const OGRRingIterator& other) const{return other.id_-id_;};
            OGRLinearRing dereference()const{
                OGRLinearRing * ring;
                if(id_ ==0){
                    //get exterior ring
                    if(parent_) {
                        ring = const_cast<OGRPolygon *>(parent_)->getExteriorRing();
                    }
                }else{
                    //get interiorring
                    if(parent_){
                        ring = const_cast<OGRPolygon*>(parent_)->getInteriorRing(id_-1);
                    }
                }
                return OGRLinearRing(ring);
            }
        };


    }
}
#endif //FROMMLE_OGRITERATORBASE_HPP
