/*! \file
 \brief Holds the base for the inputArchive class
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

#ifndef SRC_CPP_INARCHIVEBASE_HPP_
#define SRC_CPP_INARCHIVEBASE_HPP_

#include<string>
#include <map>
#include <boost/any.hpp>
#include <cassert>
#include <memory>
#include <boost/serialization/split_free.hpp>
#include "geometry/OGRiteratorBase.hpp"
#include "io/GroupBase.hpp"

namespace frommle {
	namespace io {
		using ArchiveOpts=std::map<std::string,boost::any>;
/*!
 * \brief Abstract base class for an input archive
 */
		class InputArchiveBase {
		public:
			virtual ~InputArchiveBase() = default;

			//@brief this template calls serialization calls of  Y object
			template<class Y>
			InputArchiveBase & operator >> (Y & out){boost::serialization::serialize(*this,out,file_version()); return *this;}


			Groupiterator begin()const{return Groupiterator(this);}
			Groupiterator end()const{return Groupiterator();}


			//Functions which allow to navigate through the groups of an Archive
			GroupRef operator[](const std::string & Groupname)const{return this->at(Groupname);}
			GroupRef operator[](const int & nGroup)const{return this->at(nGroup);}
		protected:
			virtual unsigned int file_version(){return 0;};

			virtual GroupRef  at(const std::string & groupname)const=0;
			virtual GroupRef  at(const int nGroup)const=0;

		private:
			friend boost::serialization::access;
			typedef boost::mpl::bool_<false> is_saving;
			typedef boost::mpl::bool_<true> is_loading;
			//needed to be compatible with the boost serialization library(don't ask me why)
			void load_binary(void * address,std::size_t count){assert(0);};

			template <class T>
			InputArchiveBase & operator & ( T & t){
				return *this >> t;
			}
//			virtual GroupBase nextGroup()=0;
		};


	}
}
#endif /* SRC_CPP_INARCHIVEBASE_HPP_*/
