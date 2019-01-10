/*! \file
 \brief Holds the base for the templated guided array class
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

#ifndef SRC_CPP_GARRAYBASE_HPP_
#define SRC_CPP_GARRAYBASE_HPP_

#include <string>
#include <boost/multi_array.hpp>
#include <tuple>
#include "core/GuidePack.hpp"
#include "core/MarrayWrapper.hpp"

#include <type_traits>

namespace frommle {
    namespace core {


/*!
 * \brief Abstract base class for storing data and the corresponding dimensions
 * This class support the storing of multidimensional data in memory, whose dimensions are linked to Dimension objects
 * Furthermore, a unitbase can be assigned to the array
 */
        class GArrayBase {
        public:
            virtual ~GArrayBase() {
            }
            std::string name()const{return name_;}
            void setName(const std::string & name){
                name_=name;
            }
        private:
        protected:
            std::string name_="data";

        };

        /*!brief
         * This is essentially a wrapper around boost multiarray class with guides
         * @tparam T element type of the multiarray
         * @tparam Guides a variadic set of Guides
         */
        template<class T, class... Guides>
        class Garray:public GArrayBase, GuidePack < Guides... >, boost::multi_array<T,sizeof...(Guides)>{
        public:
            using GPack=GuidePack<Guides...>;
            using GPack::ndim;
            using GPack::g;
            using arr_t=boost::multi_array<T,ndim>;
            using arr_t::operator[];
            Garray(){};
            /*!brief
             * Construct a Garray based on given set of Guide instances
             * @param Args
             */
            Garray(Guides&& ... Args):GPack(std::forward<Guides>(Args)...){
                //allocate matrix
                this->resize(this->getExtent());
            }


        protected:
        private:
        };

        /*!brief
         * Factory function to quickly create garrays from initialized Guides
         * @tparam T
         * @tparam Guides
         * @param inpack
         * @return
         */
        template<class T=double,class... Guides>
        Garray<T,Guides...> make_garray(Guides && ... inpack){
            return Garray<T,Guides...>(std::forward<Guides>(inpack)...);
        }


        }
    }
#endif /* SRC_CPP_GARRAYBASE_HPP_*/
