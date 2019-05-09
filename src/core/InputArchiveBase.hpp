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

			virtual void changeGroup(const std::string & GroupName){
				currentgroup_=GroupName;
				//reset current variable name
				currentvar_="";
			}
			virtual void changeVar(const std::string & VarName){
				currentvar_=VarName;
			}

			//@brief this template calls serialization calls of  Y object
			template<class Y>
			InputArchiveBase & operator >> (Y & out){boost::serialization::serialize(*this,out,file_version()); return *this;}

			typedef boost::mpl::bool_<false> is_saving;
			typedef boost::mpl::bool_<true> is_loading;
			//needed to be compatible with the boost serialization library(don't ask me why)
			void load_binary(void * address,std::size_t count){assert(0);};

			template <class T>
			InputArchiveBase & operator & ( T & t){
				return *this >> t;
			}

			Groupiterator begin()const;
			Groupiterator end()const;



			using ogriter=geometry::OGRiteratorBase;
			virtual std::shared_ptr<ogriter> ogrbegin(){return std::shared_ptr<ogriter>(new ogriter());}
			virtual std::shared_ptr<ogriter> ogrend()const{return std::shared_ptr<ogriter>(new ogriter());}

			//Functions which allow to navigate through the groups of an Archive
			GroupBase & operator[](const std::string & Groupname){return this->at(Groupname);}
			GroupBase & operator[](const int & nGroup){return this->at(nGroup);}
		protected:
			virtual unsigned int file_version(){return 0;};
			std::string currentgroup_{};
			std::string currentvar_{};

			virtual GroupBase & at(const std::string & groupname)=0;
			virtual GroupBase & at(const int & nGroup)=0;

		private:
//			virtual GroupBase nextGroup()=0;
		};


	}
}
#endif /* SRC_CPP_INARCHIVEBASE_HPP_*/
