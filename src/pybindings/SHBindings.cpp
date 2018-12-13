#include <iostream>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/tuple.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

#include "sh/SHDimension.hpp"
#include "sh/Legendre_nm.hpp"
#include "core/seqGenerator.hpp"

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




template<class T>
struct stdtuple_to_btuple{
  static PyObject* convert(T const & tin){
        return p::incref(getbtuple(tin,typename frommle::core::seqGenerator<std::tuple_size<T>::value>::type()).ptr());

  }
  template <int ...S>
  static p::tuple getbtuple(T const & tin, frommle::core::sequence<S...>){
        return p::make_tuple(std::get<S>(tin)...);
    }

  };




void pyexport_sh()
{
//    Py_Initialize();
    np::initialize();

    //register vector to ndarray converter at runtime
    p::to_python_converter< std::vector<double> , vec_to_ndarray <double>> ();

    //register specific std:tuple to python tuple converters
    p::to_python_converter<std::tuple<int,int>, stdtuple_to_btuple<std::tuple<int,int>>> ();


//    p::enum_<sh::trig>("trig")
//            .value("c",sh::trig::C)
//            .value("s",sh::trig::S);
//
//    p::class_<sh::nmt>("nmt")
//        .def_readwrite("n",&sh::nmt::n)
//        .def_readwrite("m",&sh::nmt::m);
////        .def_readwrite('t',&sh::nmt::t);

    p::def("i_from_mn",&sh::i_from_mn);
    p::def("mn_from_i",&sh::mn_from_i);
    p::def("nmax_from_sz",&sh::nmax_from_sz);

    p::class_<sh::Legendre_nm_d>("Legendre_nm",p::init<int>())
            .def("__call__",&sh::Legendre_nm_d::operator())
            .def("indxnm", &sh::Legendre_nm_d::indxnm);


}