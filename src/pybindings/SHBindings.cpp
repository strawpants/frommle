#include <iostream>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

#include "sh/SHDimension.hpp"
#include "sh/Legendre_nm.hpp"

using namespace frommle;

template<class ftype>
struct vec_to_ndarray{
    static PyObject* convert( std::vector<ftype> const & invec){
        p::tuple shape = p::make_tuple(invec.size());
        np::dtype dtype = np::dtype::get_builtin<ftype>();
//        np::ndarray py_array = np::from_data(invec.data(), dtype, p::make_tuple(invec.size()),p::make_tuple(sizeof(ftype)),p::object());
        np::ndarray py_array = np::empty(shape, dtype);
        ftype * dptr = reinterpret_cast<ftype*> (py_array.get_data());
        //copy values (not very efficient for large vectors)
        for (auto & el: invec){
            *dptr=el;
            dptr++;
        }
        return p::incref(py_array.ptr());
    }
};


void pyexport_sh()
{
    np::initialize();

    //register vector to ndarray converter at runtime
    p::to_python_converter< std::vector<double> , vec_to_ndarray <double>> ();

    p::enum_<sh::trig>("trig")
            .value("c",sh::trig::C)
            .value("s",sh::trig::S);

    p::class_<sh::nmt>("nmt")
        .def_readwrite("n",&sh::nmt::n)
        .def_readwrite("m",&sh::nmt::m);
//        .def_readwrite('t',&sh::nmt::t);


    p::class_<sh::Legendre_nm_d>("Legendre_nm",p::init<int>())
            .def("__call__",&sh::Legendre_nm_d::operator())
            .def("indxnm", &sh::Legendre_nm_d::indxnm);


}