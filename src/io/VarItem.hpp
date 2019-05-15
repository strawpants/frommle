/*! \file
 \brief
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
#include <string>
#include <memory>
#include "io/ArIO.hpp"
#include "core/Exceptions.hpp"

#ifndef FROMMLE_VARITEM_HPP
#define FROMMLE_VARITEM_HPP
namespace frommle{
    namespace io{

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class VarItem:public ArItemBase  {
        public:
            VarItem(){}
            //construct when only the groupname is known
            VarItem(const std::string & name ):ArItemBase(name){}
            //only the Archive is known at this stage
            VarItem(const Group * const Grpptr):ArItemBase(),grpParentPtr_(Grpptr){}
            VarItem(const std::string & name,const Group * const Grpptr):ArItemBase(name),grpParentPtr_(Grpptr){}

            //retrieve as 1D-vector container
            virtual VarItem & operator >>( std::vector<OGRGeometry*> & geovec ){throw core::IOException("vectorized geometry is not supported");}

            ValueIterator begin()const{return ValueIterator(this);}
            ValueIterator end()const{return ValueIterator();}
            inline ValueRef operator[](const size_t n)const{return at(n);}
            const Group* parent()const{return grpParentPtr_;}
            template<class T>
            const T getAttribute(const std::string & name)const{return boost::any_cast<const T >(attribs_.at(name));}
            template<class T>
            void setAttribute(const std::string & name, T & in){ attribs_[name]=in;}
        protected:
            //keep track of the Archive group to which this variable belongs to
            const Group * grpParentPtr_=nullptr;
            Attribs attribs_{};
        private:
            virtual ValueRef at(const size_t nVar)const{return ValueRef();}
            virtual void loadAttribs(){};
        };


    }

}




#endif //FROMMLE_VARITEM_HPP
