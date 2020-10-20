/*! \file
 \brief contains how c++ exceptions are translated to python exceptions
 \copyright Roelof Rietbroek 2020
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

#include "core/Exceptions.hpp"
#include <boost/python/exception_translator.hpp>

namespace p=boost::python;

namespace frommle{


    namespace core{
    
    void translateIndexExc(IndexingException const& e)
    {
        PyErr_SetString(PyExc_IndexError, e.what());
    }

    void registerExceptions(){
        p::register_exception_translator<IndexingException>(&translateIndexExc);
    }
    
    }
}



