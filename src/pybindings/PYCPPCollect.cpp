/*
 * This is the only file which is compiled as an extension of the python setup.py
 * The other files are compiled and included in the library
Created by roelof on 28-11-18.
*/

#include <boost/python.hpp>
using namespace boost::python;

void pyexport_core();
void pyexport_sh();

BOOST_PYTHON_MODULE(_cpp)
        {
                pyexport_core();
                pyexport_sh();
        }
