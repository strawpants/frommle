/*! \file
 \brief contains routines for sorting and comparing Guides
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


#ifndef SRC_CORE_GUIDE_SORTING_HPP_
#define SRC_CORE_GUIDE_SORTING_HPP_

namespace frommle{

    namespace guides{
        ///@brief find the matching part of two guides and return two masked versions. This version Guides derived frim IndexedGuides
        template<class Gd>
        std::array<Gd,2> Union(const Gd& left,const Gd & right){
            //we first create copies of the input
            std::array<Gd,2> GuidesOut={left,right};
            //then we find matching results and mask the entries which do not have a common entry
            //Initialize iterators which are sorted by element
            auto itl=GuidesOut[0].elindx().begin();
            auto endl=GuidesOut[0].elindx().end();
            auto itr=GuidesOut[1].elindx().begin();
            auto endr=GuidesOut[1].elindx().end();

            //because we have sorted the iterators we can do this in a single loop (the nested loops are subloops)
            do while (itl != endl or itr != endr){

                //increment and mask the left iterator until it reaches the second iterator
                do while (itl != endl & (itr == endr or itl->el < itr->el ) ){
                    //mask left iterator entry
                    ++itl;

                }

                //increment and mask the right iterator until it reaches the first iterator
                do while (itr != endr & (itl == endl or itr->el < itl->el ) ){
                    //mask right iterator entry
                    ++itr;

                }

                //don't mask just increment both iterators (they should be the same or at the end  when we arrive here)
                if( itl != endl) ++itl;

                if( itr != endr) ++itr;

            }
            return GuidesOut;

        }

    }
}






#endif /* #SRC_CORE_GUIDE_SORTING_HPP_ */
