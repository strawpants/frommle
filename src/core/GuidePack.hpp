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

#ifndef CORE_GUIDEPACK_HPP_
#define CORE_GUIDEPACK_HPP_
namespace frommle{

    namespace core{

//        template <class LGuide, class ... RGuides> struct stripfirst{
//            using type=std::tuple<RGuides...>;
//        };
/*!brief
 * Wraps several guides into a tuple and provide access functions
 * @tparam Guides: a variadic list of guides which spans the dimensions
 */
        template< class ... Guides >
        class GuidePack{
        public:
            static const int ndim=sizeof...(Guides);
            using guides_t=std::tuple<Guides...>;
            template<int n>
            using g_t=typename std::tuple_element<n,guides_t>::type;
//            using subguides=typename stripfirst<Guides...>::type;
            GuidePack(){}
            GuidePack(Guides&& ... Args){
                extent_={Args.size()...};
                guides_ = std::make_tuple(std::forward<Guides>(Args)...);

            }
            std::array<size_t,ndim> getExtent()const{
                return extent_;
            };

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
            typename std::enable_if< n+1 != ndim,const GuideBase>::type & g(const int i){
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
            typename std::enable_if< n+1 == ndim,const GuideBase>::type & g(const int i){
                assert(i+1==ndim);
                return g<n>();
            }
        private:
            guides_t guides_{};
            std::array<size_t,ndim> extent_={};
        };


    }
}

#endif /* _CORE_GUIDEPACK_HPP_ */
