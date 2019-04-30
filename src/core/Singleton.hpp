/*! \file
 \brief Wrap a class in this singleton template to create a singleton version from it
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

#include<boost/core/noncopyable.hpp>
#include<assert.h>

#ifndef SRC_CORE_SINGLETON_HPP_
#define SRC_CORE_SINGLETON_HPP_


namespace frommle {
	namespace core {
		template<class T>
		class Singleton : public boost::noncopyable {
		public:
			static T &get() {
				//this may not work on all compilers (they say), but seems fine up till now
				static T Instance; //C++11 says that this shall be called only once and is safe when called concurrently!! (Instance will be deleted after the main function returns control to the OS)
				return Instance;
			};
		protected:
			~Singleton() {};
			//Make usre the contructor is private/protected so it cannot be called from the public scope
			Singleton() {};
		};

	}

}


#endif /* SRC_CORE_SINGLETON_HPP_ */
