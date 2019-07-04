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

#include <tuple>
#include "core/GuideBase.hpp"
#include "io/Group.hpp"
#include "seqGenerator.hpp"
#include <boost/algorithm/string.hpp>

#ifndef CORE_GUIDEPACK_HPP_
#define CORE_GUIDEPACK_HPP_
namespace frommle{

    namespace core{

/*!brief
 * Wraps several guides into a tuple and provide access functions
 * @tparam Guides: a variadic list of guides which spans the dimensions
 */
        template< class ... Guides >
        class GuidePack{
        public:
            static const int ndim=sizeof...(Guides);
            using guides_t=std::tuple<std::shared_ptr<Guides>...>;
            template<int n>
            using g_t=typename std::tuple_element<n,guides_t>::type;
//            using subguides=typename stripfirst<Guides...>::type;
            GuidePack(){}
            GuidePack(Guides&& ... Args){
                extent_={Args.size()...};
//                guides_ = std::make_tuple(std::forward<(std::make_shared<Guides>(std::forward<Guides>(Args)...)...);
                guides_ = std::make_tuple(std::make_shared<Guides>(std::move(Args))...);

            }


            template<class Gp>
            GuidePack & operator=(Gp & gpin){
                assert(ndim == gpin.ndim);
                assignGuides<0, Gp>(gpin);
                return *this;
            }


            std::array<size_t,ndim> getExtent()const{
                return extent_;
            };

            long long unsigned int num_elements()const{
                return std::accumulate(extent_.cbegin(),extent_.cend(),0);
            }

            void setNames(const std::string & coordinatenames){
                std::vector<std::string> splits;
                boost::split(splits,coordinatenames,boost::is_any_of(" "));
                for(int i=0;i<splits.size();++i){
                    g(i)->setName(splits[i]);
                }

            }
            size_t size()const{return std::accumulate(extent_.cbegin(),extent_.cend(),0);}

            /*!brief
             * Get the nth guide of the array
             * @tparam n
             * @return a reference to the Guide instance belonging to the Garray
             */
            template<int n>
            typename std::tuple_element<n,guides_t>::type & g() {
                assert(n<=ndim);
                return std::get<n>(guides_);
            }
            template<int n>
            const typename std::tuple_element<n,guides_t>::type & g()const{
                assert(n<=ndim);
                return std::get<n>(guides_);
            }

            /*!brief
             * Polymorphic version of the g() function
             * Recursively calls itself until the correct dimension is founc
             * @tparam n test for this dimension
             * @param i: dimension to retrieve
             * @return a GuideBase
             */
            template<int n=0>
            typename std::enable_if< n+1 != ndim,std::shared_ptr<const GuideBase>>::type  g(const int i)const{
                if (i == n) {
                    return g<n>();
                }else{
                    //call this function recursively
                    return g<n+1>(i);
                }
            }
            /*! brief the function below stops the recursion
             *
             * @tparam n
             */
            template<int n=0>
            typename std::enable_if< n+1 == ndim,std::shared_ptr<const GuideBase>>::type  g(const int i)const{
                assert(i+1==ndim);
                return g<n>();
            }

            //non-const versions

            template<int n=0>
            typename std::enable_if< n+1 != ndim, std::shared_ptr<GuideBase>>::type  g(const int i){
                if (i == n) {
                    return g<n>();
                }else{
                    //call this function recursively
                    return g<n+1>(i);
                }
            }
            /*! brief the function below stops the recursion
             *
             * @tparam n
             */
            template<int n=0>
            typename std::enable_if< n+1 == ndim,std::shared_ptr<GuideBase>>::type  g(const int i){
                assert(i+1==ndim);
                return g<n>();

            }

            template<int i>
            struct maskpack {
                    template<class Gother>
                    using mask_g=typename std::conditional<std::is_same<Gother,typename g_t<i>::element_type>::value,core::MaskedGuide<Gother>,Gother>::type;
                    using type=GuidePack< mask_g<Guides>...>;
            };

            ///@returns a guidepack which has one dimension masked
            template<int i>
            typename maskpack<i>::type mask(){
                typename maskpack<i>::type maskp{};
                //assign guides
                maskp.guides_=guides_;
                return maskp;
            }

        private:
            template<class ...Gs>
            friend class GuidePack;

            friend class io::serialize;
            template<class Archive>
            void load(Archive & Ar){
                loadGuides<Archive,0>(Ar);
            }

            template<class Archive>
            void save(Archive & Ar)const{
                saveGuides<Archive,0>(Ar);            
            
            }
            template<class Archive,int n>
            typename std::enable_if< n+1 != ndim>::type saveGuides(Archive & Ar)const{
                Ar << *(std::get<n>(guides_));
                //also recursively save the remaining guides
                saveGuides<Archive,n+1>(Ar);
            };
            
            template<class Archive,int n>
            typename std::enable_if< n+1 == ndim>::type saveGuides(Archive & Ar)const{
                Ar << *(std::get<n>(guides_));
            };

            template<class Archive,int n>
            typename std::enable_if< n+1 != ndim>::type loadGuides(Archive & Ar){
                Ar >> *(std::get<n>(guides_));
                //also recursively save the remaining guides
                loadGuides<Archive,n+1>(Ar);
            };

            template<class Archive,int n>
            typename std::enable_if< n+1 == ndim>::type loadGuides(Archive & Ar){
                Ar >> *(std::get<n>(guides_));
            };

            template<class G,class Gin>
                static typename std::enable_if<std::is_same< typename G::element_type,typename Gin::element_type>::value>::type assign(G & out, Gin & in){
                    //we don't convert but just add an owner to the shared_ptr
                    out=in;
                }
            
            template<class G,class Gin>
                static typename std::enable_if<!std::is_same<typename G::element_type,typename Gin::element_type>::value>::type assign(G & out, Gin & in){
                    //we convert the value here but just add an owner to the shared_ptr
                    //convert

                    out=std::shared_ptr<typename G::element_type>(new typename G::element_type(*in));
                }

            template<int n,class Gpackother>
            typename std::enable_if< n+1 != ndim >::type assignGuides(Gpackother &gpother){
                    assign(g<n>(),gpother.template g<n>());
                    //also recursively assign the other guides
                assignGuides<n + 1, Gpackother>(gpother);
            }

            template<int n,class Gpackother>
            typename std::enable_if< n+1 == ndim>::type assignGuides(Gpackother &gpother){
                assign(g<n>(),gpother.template g<n>());

            };

            guides_t guides_{};
            std::array<size_t,ndim> extent_={0};
        };


    }
}

#endif /* _CORE_GUIDEPACK_HPP_ */
