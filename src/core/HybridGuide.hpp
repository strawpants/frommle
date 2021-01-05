/*! \file
 \brief  A Guide which groups and wraps other guides
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
#include <boost/any.hpp>

#ifndef SRC_CORE_HYBRID_GUIDE_HPP_
#define SRC_CORE_HYVRID_GUIDE_HPP_
namespace frommle{
    namespace guides{

    class HybridGuide: public GuideBase{
        public:
            using Element=boost::any;
            struct range{
                size_t start=0;
                size_t end=0;
            }
            const std::vector<range> & range()const {return ranges_;}
        

        protected:

        private:
               std::vector<range> ranges_{};
               std::vector<std::shared_ptr<GuideBase>> guides_{};
    };
}

#endif /* #SRC_CORE_HYBRID_GUIDE_HPP_ */
