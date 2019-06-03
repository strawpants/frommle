/*! \file
 \brief Provides an abstract base to work with groups and variables which can be sued to navigate in Archives
 \copyright Roelof Rietbroek 2019
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

#include "ArItem.hpp"
#include "io/Group.hpp"
#include "io/VarItem.hpp"
#include "io/ValueItem.hpp"
#include "io/ArchiveBase.hpp"
namespace frommle{
    namespace io{


        template<class T, class P>
        ArItemiterator<T, P>::ArItemiterator(const P *const parentPtr) {
            aritemPtr_=parentPtr->operator[](0);
        }

        template<class T, class P>
        ArItemiterator<T,P>::ArItemiterator():aritemPtr_(new T()){}



        //explicitly initiate templates for a Group item. This is needed to prevent circular header file includes
        // and since the amount of Aritems is expected to be limited (Groups versus variables, this is not such a problem)
        template
        class ArItemiterator<Group, ArchiveBase>;
        //also instantiate a varitem iterator
        template
        class ArItemiterator<VarItem,Group>;

        template
        class ArItemiterator<ValueItem,VarItem>;

    }


}

