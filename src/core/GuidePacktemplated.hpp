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
#include "core/GuidePack.hpp"

#ifndef SRC_CORE_GUIDEPACKTEMPLATED_HPP_
#define SRC_CORE_GUIDEPACKTEMPLATED_HPP_
namespace frommle{
    namespace guides{

/*!brief
 * Wraps several guides into a tuple and provide access functions
 * @tparam Guides: a variadic list of guides which spans the dimensions
 */
        template< class ... Guides >
        class GuidePack: public GuidePackDyn<sizeof ...(Guides)> {
        public:
            using GPdyn=GuidePackDyn<sizeof ... (Guides)>;
            using GPdyn::ndim;
            using GPdyn::gpar_;
            using GPdyn::extent;
            using GPdyn::num_elements;

            ///@brief alias to store the types of the input
            using guides_t=std::tuple<Guides...>;

            ///@brief this allows the compile time extraction of the Guide type
            template<int n>
            using g_t=typename std::tuple_element<n,guides_t>::type;
            
            template<int n>
            using gptr_t=typename std::shared_ptr<typename std::tuple_element<n,guides_t>::type>;

            GuidePack(){}
            explicit GuidePack(Guides && ... Args):GPdyn(std::move(Args)...){
                //extent_={Args.size()...};
//                guides_ = std::make_tuple(std::forward<(std::make_shared<Guides>(std::forward<Guides>(Args)...)...);
                //guides_ = std::make_tuple(std::make_shared<Guides>(std::move(Args))...);

            }

            explicit GuidePack(const Guides & ... Args):GPdyn(Args...){}

            /*!brief
             * Get the nth guide of the array
             * @tparam n
             * @return a reference to the Guide instance belonging to the Garray
             */
            template<int n>
            gptr_t<n>  g() {
                assert(n<=ndim);
                return boost::get<gptr_t<n>>(gpar_[n]);
//                return std::static_pointer_cast<g_t<n>>(gpar_[n]);
            }
//
            template<int n>
            const gptr_t<n> g()const{
                assert(n<=ndim);
                return boost::get<gptr_t<n>>(gpar_[n]);
            }

            template<int i>
            size_t idx(const typename g_t<i>::Element & el)const{
                return  g<i>()->idx(el);
            }

        private:
        };
    }

}


#endif /* #SRC_CORE_GUIDEPACKTEMPLATED_HPP_ */
