//#include "pybindings/coreBindings.hpp"
#include "core/GuidePack.hpp"
#include "core/GOperatorBase.hpp"
#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include "pybindings/tupleconversion.hpp"
#include "pybindings/numpyConverting.hpp"
#include "pybindings/coreBindings.hpp"
#include <memory>
using namespace frommle::py;
using namespace frommle::guides;
using namespace frommle::core;


void pyexport_core(){


    //Register c++ to python conversions

    register_tuple_converters();
    
    register_numpy_converters();

    //register to/from date converters
    register_datetime();
    
    register_GuideBase();

    register_coreGuides();

    
    
    //register the GuidePack
    registerGuidePack();


    //register GArrays
    registerGArrays();

    
    registerGOperators();

    //export setting logging levels for C++
    p::def("setcppInfoLevel",&Logging::setInfoLevel);
    p::def("setcppDebugLevel",&Logging::setDebugLevel);
    p::def("setcppWarningLevel",&Logging::setWarningLevel);
    p::def("setcppErrorLevel",&Logging::setErrorLevel);
    //actually set the default logging level  for the library to errors only
    Logging::setErrorLevel();
    

}

