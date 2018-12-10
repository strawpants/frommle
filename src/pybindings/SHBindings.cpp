#include <iostream>
#include <boost/python.hpp>
using namespace boost::python;
#include "sh/SHDimension.hpp"
#include "sh/Legendre_nm.hpp"

using namespace frommle;

void pyexport_sh()
{
    enum_<sh::trig>("trig")
            .value("c",sh::trig::C)
            .value("s",sh::trig::S);

    class_<sh::nmt>("nmt")
        .def_readwrite("n",&sh::nmt::n)
        .def_readwrite("m",&sh::nmt::m);
//        .def_readwrite('t',&sh::nmt::t);


    class_<sh::Legendre_nm_d>("Legendre_nm",init<int>())
            .def("__call__",&sh::Legendre_nm_d::operator())
            .def("indxnm", &sh::Legendre_nm_d::indxnm);


}