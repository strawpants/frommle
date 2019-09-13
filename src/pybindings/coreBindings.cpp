//#include "pybindings/coreBindings.hpp"
#include "core/GuideBase.hpp"
#include "core/GuidePack.hpp"
#include "core/GOperatorBase.hpp"
#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include "pybindings/tupleconversion.hpp"
#include "pybindings/datetimeconversion.hpp"
#include "pybindings/numpyConverting.hpp"
#include "pybindings/coreBindings.hpp"
using namespace frommle::py;
using namespace frommle::core;


void pyexport_core(){


    //Register c++ to python conversions


    register_tuple_converters();
    
    register_numpy_converters();
    //register to/from date converters
    register_datetime();



    //p::to_python_converter<std::tuple<int,int>, stdtuple_to_ptuple<std::tuple<int,int>>> ();



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

    p::register_ptr_to_python< constGuideBasePtr >();
    p::register_ptr_to_python< GuideBasePtr >();

    //IndexGuide
    p::class_<IndexGuide,p::bases<GuideBase>>("IndexGuide").def(p::init<size_t>()).
        def("__array__",&guide_to_ndarray<IndexGuide>);

    //TimeGuides
    void (DateGuide::*pbdg)(const gregdate )=&DateGuide::push_back;
    const gregdate & (DateGuide::*igetdt)(const size_t)const=&DateGuide::operator[];
    p::class_<DateGuide,p::bases<GuideBase>>("DateGuide")
    .def("append",pbdg)
    .def("__getitem__",igetdt,p::return_value_policy<p::copy_const_reference>())
    .def("__iter__",p::iterator<const DateGuide>());

    void (PTimeGuide::*pbpt)(const ptime )=&PTimeGuide::push_back;
    const ptime & (PTimeGuide::*igetpt)(const size_t)const=&PTimeGuide::operator[];
    p::class_<PTimeGuide,p::bases<GuideBase>>("PTimeGuide")
    .def("append",pbpt)
    .def("__getitem__",igetpt,p::return_value_policy<p::copy_const_reference>())
    .def("__iter__",p::iterator<const PTimeGuide>());


    //register the GuidePack
    registerGuidePack(); 
    //GuidePackPtr (*mkgp1) (GuideBase &) =&makeGuidePack;
    //p::def("makeGuidePack",mkgp1);
    ////BOOST_PYTHON_FUNCTION_OVERLOADS(makegp_overloads, &makeGuidePack, 1, 2);
    //GuidePackPtr (*mkgp2) (GuideBase &,GuideBase &) =&makeGuidePack;
    //p::def("makeGuidePack",mkgp2);
    
    ///Register the operator base as defined in frommle
    p::class_<GOperatorBase>("GOperatorBase").def(p::init<p::optional<std::string>>())
            .add_static_property("ndim",p::make_getter(&GOperatorBase::ndim))
            .add_property("name",&GOperatorBase::name,&GOperatorBase::setName);

    //export setting logging levels for C++
    p::def("setcppInfoLevel",&Logging::setInfoLevel);
    p::def("setcppDebugLevel",&Logging::setDebugLevel);
    p::def("setcppWarningLevel",&Logging::setWarningLevel);
    p::def("setcppErrorLevel",&Logging::setErrorLevel);
    //actually set the default logging level  for the library to errors only
    Logging::setErrorLevel();
    

}

