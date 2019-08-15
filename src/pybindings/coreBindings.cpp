#include "pybindings/coreBindings.hpp"
#include "core/GuideBase.hpp"
#include "core/GOperatorBase.hpp"
#include "core/IndexGuide.hpp"

using namespace frommle::py;
using namespace frommle::core;


void pyexport_core(){


    //Register c++ to python conversions

    //register vector to ndarray converter at runtime
    p::to_python_converter< std::vector<double> , vec_to_ndarray <double>> ();

    //register specific std:tuple to python tuple converters
    p::to_python_converter<std::tuple<int,int>, stdtuple_to_ptuple<std::tuple<int,int>>> ();



    ///registration of Guides defined in frommle::core

    //typehash
    p::class_<typehash>("typehash").def(p::init<std::string>())
         .def("__str__",&typehash::operator std::string )
         .def("add",&typehash::add);

    //GuideBase
    p::class_<GuideBase>("GuideBase")
            .add_property("name",&GuideBase::name,&GuideBase::setName)
            .def("size",&GuideBase::size)
            .def("hash",&GuideBase::hash)
            .add_static_property("ndim",p::make_getter(&GuideBase::ndim))
            .def("__str__",&GuideBase::printhash);
//            .def_readonly("ndim",make_getter(&GuideBase::ndim, p::return_value_policy<p::reference_existing_object>()));


    //IndexGuide
    p::class_<IndexGuide,p::bases<GuideBase>>("IndexGuide").def(p::init<size_t>());

    ///Register the operator base as defined in frommle
    p::class_<GOperatorBase>("GOperatorBase").def(p::init<p::optional<std::string>>())
            .add_static_property("ndim",p::make_getter(&GOperatorBase::ndim))
            .add_property("name",&GOperatorBase::name,&GOperatorBase::setName);


}
