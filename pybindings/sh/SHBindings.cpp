#include <iostream>
#include <boost/python.hpp>
#include "sh/SHisoOperator.hpp"
#include "sh/Legendre_nm.hpp"
#include "sh/Ynm.hpp"
#include "../core/coreGuides.hpp"
#include "../core/tupleconversion.hpp"
#include <boost/python/return_value_policy.hpp>
#include <boost/python/copy_non_const_reference.hpp>


namespace p = boost::python;
namespace np = boost::python::numpy;

using namespace frommle::guides;

using namespace frommle::sh;
using namespace frommle;

template<class T>
struct register_shisooperator{
    register_shisooperator(std::string basename){
        p::class_<SHisoOperator<T>,p::bases<core::GOperatorDiag<T>>>(basename.c_str(),p::init<std::vector<T>,p::optional<std::string>>())
            .def("nmax",&SHisoOperator<T>::nmax);

        //.def(p::init<int>())
                //.def(p::init<std::vector<T>>());


    }
};


void pyexport_sh()
{


    p::enum_<trigenum>("trig")
            .value("c",trigenum::C)
            .value("s",trigenum::S);

    frommle::py::register_tuple<nmtEl>();
    //not needed as std:tuple<int,int> has laready been registered in tupleconversion.hpp
    //frommle::py::register_tuple<nmEl>();

    //export SHGuide
    pyIndexedGuide<SHGuide>::reg("SHGuide")
            .def(p::init<int,p::optional<std::string>>())
            .add_property("nmax",&SHGuide::nmax)
            .add_property("nmin",&SHGuide::nmin);

    pyNoIndexedGuide<SHnmGuide>::reg("SHnmGuide")
        .def(p::init<int>())
        .add_property("nmax",&SHnmGuide::nmax)
        .add_property("nmin",&SHnmGuide::nmin);

    pyNoIndexedGuide<SHtmnGuide>::reg("SHtmnGuide")
        .def(p::init<int>())
        .add_property("nmax",&SHtmnGuide::nmax)
        .add_property("nmin",&SHtmnGuide::nmin);
    
    pyNoIndexedGuide<SHnmtGuide>::reg("SHnmtGuide")
        .def(p::init<int>())
        .add_property("nmax",&SHnmtGuide::nmax)
        .add_property("nmin",&SHnmtGuide::nmin);


    p::register_ptr_to_python< std::shared_ptr<SHnmGuide> >();
    p::register_ptr_to_python< std::shared_ptr<SHGuide> >();
    p::register_ptr_to_python< std::shared_ptr<SHtmnGuide> >();
    p::register_ptr_to_python< std::shared_ptr<SHnmtGuide> >();

    //register SHisoperator
    register_shisooperator<double>("shisoperator");


    p::class_<Legendre_nm<double>,p::bases<core::GArrayDense<double,1>>>("Legendre_nm",p::init<int>())
            .def("set",&sh::Legendre_nm_d::set,p::return_value_policy<p::copy_non_const_reference>());
    
    //register Spherical harmonics operator

//    p::class_<Ynm<double>,p::bases<core::GOperatorDyn<double,1,1>>>("Ynm",p::init<int>())

}
