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
#include <cassert>
#include "io/ArIO.hpp"
#include <boost/serialization/split_free.hpp>

#ifndef FROMMLE_GROUPBASE_HPP
#define FROMMLE_GROUPBASE_HPP
namespace frommle{
    namespace io{

        //@brief Holds information about a group in an archive (e.g. name, attributes, Archive pointer). This can also be used as an index to refer to navigate to a certain group within an Archive
        class Group:public ArItemBase  {
        public:
            Group(){}
            //construct when only the groupname is known
            Group(const std::string & name ):ArItemBase(name){}
            //construct when both the name and open Archive are known
            Group(const std::string & name, const ArchiveBase * const Arptr ):ArItemBase(name),Arptr_(Arptr){}
            //only the Archive is known at this stage
            Group(const ArchiveBase * const Arptr):ArItemBase(),Arptr_(Arptr){}

            VarIterator begin()const{return VarIterator(this);}
            VarIterator end()const{return VarIterator();}


            //Functions which allow to navigate through the variables of a Group
            inline VarRef operator[](const std::string & VarName)const{return this->at(VarName);}
            inline VarRef operator[](const int  nVar)const{return this->at(nVar);}

            //@brief convenience function to get the first OGR 'geom' variable in a group (defautl takes the one called 'geom')
            virtual VarRef geoVar(){return at("geom");};

            //serialization goodies (objects can be read/written to groups by implmenting a serialization function
            template<class Y>
            Group & operator >> (Y & out){boost::serialization::serialize(*this,out,file_version()); return *this;}

            const InputArchiveBase* parent()const{return Arptr_;}
            typedef boost::mpl::bool_<true> is_saving;
            typedef boost::mpl::bool_<true> is_loading;
        protected:
            friend boost::serialization::access;

            //needed to be compatible with the boost serialization library(don't ask me why)
            void load_binary(void * address,std::size_t count){assert(0);};
            template <class T>
            VarItem & operator & ( T & t){
                return *this >> t;
            }

            virtual unsigned int file_version(){return 0;};


            virtual VarRef at(const std::string & VarName)const{return VarRef();}
            virtual VarRef at(const int nVar)const{return VarRef();}
            //Note: this class does not own the memory of the Archive pointer
            const ArchiveBase * Arptr_=nullptr;
            //possibly add attributes and variables names here
        };

    }

}



#endif //FROMMLE_GROUPBASE_HPP
