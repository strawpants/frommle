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
#ifndef FROMMLE_TREENODE_HPP
#define FROMMLE_TREENODE_HPP

namespace frommle{
    namespace core{
        class TreeNodebase{
        public:
            TreeNodebase(const std::string & name):name(name){}
            TreeNodebase(const std::string && name):name(std::move(name)){}
            std::string getName()const{return name_;}
            void setName(const std::string name){name_=name;}
        private:
            std::string name_="TreeNode";
        };

        template<class T>
        class TreeNodeRef;

        template<class T,class P>
        class TreeNodeIterator;

        template<class T, class K, class P>
        class TreeNode: public TreeNodebase{
        public:
            using ref=TreeNodeRef<T>;
            //refers to the iterator containing elements of T itself
            using iterator=TreeNodeIterator<T,P>;
            TreeNode(const std::string name):TreeNodebase(name){}
            TreeNode(const std::string && name):TreeNodebase(std::move(name)){}
            TreeNode(const std::string name,const P * parent ):TreeNodebase(name),parent_(parent){}

            K::iterator begin()const{return K::iterator(this);}
            iterator end()const{return iterator();}

            virtual K::ref operator[](const std::string & name)=0;
            virtual K::ref operator[](const size_t & indx)=0;

            const P* parent()const{return parent_;}
        private:
            P* parent_=nullptr;
            size_t id_=-1;
        };

    }
}
#endif //FROMMLE_TREENODE_HPP
