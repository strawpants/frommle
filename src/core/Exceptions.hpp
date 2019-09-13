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

#include <stdexcept>
#include "core/MacroMagic.hpp"

#ifndef FROMMLE_EXCEPTIONS_HPP
#define FROMMLE_EXCEPTIONS_HPP

namespace frommle{
    namespace core{
    ///@brief dedicated exception thrown for Guide matching problems
        class GuideMatchingException:public std::runtime_error{
        public:
            GuideMatchingException(const std::string message):std::runtime_error(message){}
        };

        ///@brief dedicated exception thrown on wrong (user) input values
        class InputException:public std::runtime_error{
        public:
            InputException(const std::string message):std::runtime_error(message){}
        };


        ///@brief Exception for indexing errors (out of bound, stride mismath etc)
        class IndexingException:public std::runtime_error{
        public:
            IndexingException(const std::string message):std::runtime_error(message){}
        };

        ///@brief Exception for indexing errors (out of bound, stride mismath etc)
        class IOException:public std::runtime_error{
        public:
            IOException(const std::string message):std::runtime_error(message){}
        };

        ///@brief Exception thrown when class methods are requested which don't exist for this runtime instance
        class MethodException:public std::runtime_error{
        public:
            MethodException(const std::string message):std::runtime_error(message){}
        };

        ///@brief Exception thrown when class methods are requested which don't exist for this runtime instance
        class NotImplementedException:public std::runtime_error{
        public:
            NotImplementedException(const std::string message):std::runtime_error(message){}
        };
    }
}

#define THROWIOEXCEPTION(x) throw frommle::core::IOException(HERE + x)
#define THROWINPUTEXCEPTION(x) throw frommle::core::InputException(HERE + x)
#define THROWMETHODEXCEPTION(x) throw frommle::core::MethodException(HERE + x)
#define THROWNOTIMPLEMENTED(x) throw frommle::core::NotImplementedException(HERE + x)

#endif //FROMMLE_EXCEPTIONS_HPP
