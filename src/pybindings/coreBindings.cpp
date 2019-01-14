#include "pybindings/coreBindings.hpp"
#include "core/GuideBase.hpp"
#include "core/IndexGuide.hpp"

using namespace frommle::py;
using namespace frommle::core;

//this wrapper class is needed in order for the virtual type function to work
//struct GuideBaseWrap : GuideBase, p::wrapper<GuideBase>
//{
//    std::string type()const
//    {
//        if (p::override type = this->get_override("type"))
//            return type(); // *note*
//        return GuideBase::type();
//    }
//
//    std::string default_type() { return this->GuideBase::type(); }
//};


void pyexport_core(){


    //Register c++ to python conversions

    //register vector to ndarray converter at runtime
    p::to_python_converter< std::vector<double> , vec_to_ndarray <double>> ();

    //register specific std:tuple to python tuple converters
    p::to_python_converter<std::tuple<int,int>, stdtuple_to_ptuple<std::tuple<int,int>>> ();



    ///registration of Guides defined in frommle::core

    //GuideBase
    p::class_<GuideBase>("GuideBase")
            .def("size",&GuideBase::size)
            .def("type",&GuideBase::type);


    //IndexGuide
    p::class_<IndexGuide,p::bases<GuideBase>>("IndexGuide").def(p::init<size_t>());


}