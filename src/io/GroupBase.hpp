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
#include <string>
#include <memory>

#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP
namespace frommle{
    namespace io{
        class InputArchiveBase;
        class Groupiterator;

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class GroupBase{
        public:
            GroupBase(){}
            //construct when only the groupname is known
            GroupBase(const std::string & name ):gname_(name){}
            //construct when both the name and open Archive are known
            GroupBase(const std::string & name, const InputArchiveBase * const Arptr ):gname_(name),Arptr_(Arptr){}
            //only the Archive is known at this stage
            GroupBase(const InputArchiveBase * const Arptr):Arptr_(Arptr){}
            void setName(const std::string gname){gname_=gname;}
            std::string getName()const{return gname_;}
            virtual GroupBase & operator ++(){++gid_;}


        protected:
            friend Groupiterator;
            std::string gname_="";
            int gid_=-1;
            //Note: this class does not own the memory of the Archive pointer
            const InputArchiveBase * Arptr_=nullptr;
            //possibly add attributes and variables names here
        };

        using GroupRef=std::shared_ptr<GroupBase>;

        //@brief an iterator which iterates through the groups within an archive
        class Groupiterator{
        public:
            //iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = GroupBase;
            using difference_type = std::ptrdiff_t;
            using pointer = GroupBase*;
            using reference = GroupBase&;
            Groupiterator& operator++(){++*grpPtr_;};
            bool operator==(const Groupiterator & other) const {return grpPtr_->gid_ == other.grpPtr_->gid_;}
            bool operator!=(const Groupiterator & other) const {return !(*this == other);}
            GroupBase * operator*() {return grpPtr_.get();}
            virtual ~Groupiterator(){}
            Groupiterator(const Groupiterator & in){
                grpPtr_=in.grpPtr_;
            }
            Groupiterator():grpPtr_(new GroupBase()){}
            Groupiterator(const InputArchiveBase * const inAr);
        private:
            GroupRef grpPtr_{};
        };

    }

}



#endif //FROMMLE_GROUPBASE_HPP
