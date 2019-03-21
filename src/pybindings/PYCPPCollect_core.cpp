/*! \file
 \brief This is the only file which is compiled through the python setup.py file and therefore does not belong to any c++ target
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



#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

void pyexport_core();

BOOST_PYTHON_MODULE(_core)
        {
        Py_Initialize();
        boost::python::numpy::initialize();
                pyexport_core();
        }
