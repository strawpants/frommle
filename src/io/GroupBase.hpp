/*! \file
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

#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP

namespace frommle{
    //forward declare
    namespace core {
           class InputArchiveBase;
    }

    namespace io{
        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class GroupBase{
        public:
            GroupBase(){}
            //construct when only the groupname is known
            GroupBase(const std::string & name ):gname_(name){}
            //construct when both the name and open Archive are known
            GroupBase(const std::string & name, core::InputArchiveBase * const Arptr ):gname_(name),Arptr_(Arptr){}
            //the name is unknown bu the arhvie is
            GroupBase(core::InputArchiveBase * const Arptr):gname_(),Arptr_(Arptr){}
        private:
            std::string gname_="";
            //Note: this class does not own the memory of the Archive pointer
            core::InputArchiveBase * Arptr_=nullptr;
            //possibly add attributes and variables names here
        };


        //@brief an iterator which iterates through the groups within an archive
        class Groupiterator{
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = GroupBase;
            using difference_type = std::ptrdiff_t;
            using pointer = GroupBase*;
            using reference = GroupBase&;
            virtual Groupiterator& operator++(){ return *this;};
            bool operator==(Groupiterator & other) const {return grpPtr_ == other.grpPtr_;}
            bool operator!=(Groupiterator & other) const {return !(*this == other);}
            GroupBase * operator*() {return grpPtr_;}
            virtual ~Groupiterator(){}
            Groupiterator(){}
        private:
            GroupBase *grpPtr_=nullptr;
        };

    }

}



#endif //FROMMLE_GROUPBASE_HPP
