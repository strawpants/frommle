/*! \file contains some template tools to generate sequences
 \brief 
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
#ifndef FROMMLE_SEQUENCE_HPP
#define FROMMLE_SEQUENCE_HPP

namespace frommle {
    namespace core {

/*!brief Empty struct which has a specialization with a list of int arguments (to be used for compile time sequence generation)
*/
        template<int...>
        struct sequence {
        };

/*!brief recursively generate a sequence with integer template elements
*/
        template<int N, int ...S>
        struct seqGenerator : seqGenerator<N - 1, N - 1, S...> {
        };

/*! brief this stops the above recursion and sets the type member to the generated sequence<0,1,2, ...>
*/
        template<int... S>
        struct seqGenerator<0, S...> {
            using type = sequence<S...>;
        };


    }
}

#endif //FROMMLE_SEQUENCE_HPP
