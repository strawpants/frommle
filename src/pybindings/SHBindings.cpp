#include <iostream>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "pybindings/coreBindings.hpp"
#include "core/GuideBase.hpp"

namespace p = boost::python;
namespace np = boost::python::numpy;

#include "sh/SHGuide.hpp"
#include "sh/Legendre_nm.hpp"


namespace frommle {
    namespace py {
//Wrapper class is needed to cope with pure virtual functions of SHGuideBase
        struct SHGuideBaseWrapper : sh::SHGuideBase, p::wrapper<sh::SHGuideBase> {
        public:
            index idx(const int n, const int m, const trig t)const {
                return this->get_override("idx")(n,m,t);
            }

            Element nmt(const index idx) const {
                return this->get_override("nmt")(idx);
            }
        };
    }
}

using namespace frommle;

void pyexport_sh()
{


    p::enum_<sh::SHGuideBase::trig>("trig")
            .value("c",sh::SHGuideBase::trig::C)
            .value("s",sh::SHGuideBase::trig::S);


    p::to_python_converter<sh::SHGuideBase::Element, py::stdtuple_to_ptuple<sh::SHGuideBase::Element> >();

    p::class_<py::SHGuideBaseWrapper,boost::noncopyable>("SHGuideBase")
            .def("nmax",p::pure_virtual(&sh::SHGuideBase::nmax))
            .def("nmin",p::pure_virtual(&sh::SHGuideBase::nmin))
            .def("idx",p::pure_virtual(&sh::SHGuideBase::idx))
            .def("nmt",p::pure_virtual(&sh::SHGuideBase::nmt));

//        .def_readwrite("n",&sh::nmt::n)
//        .def_readwrite("m",&sh::nmt::m);
////        .def_readwrite('t',&sh::nmt::t);

//    p::def("i_from_mn",&sh::i_from_mn);
//    p::def("mn_from_i",&sh::mn_from_i);
    p::def("nmax_from_sz",&sh::nmax_from_sz);

    p::class_<sh::SHtmnDim,p::bases<sh::SHGuideBase> >("SHtmnDim",p::init<int>())
//            .def("idx",&sh::SHtmnDim::idx)
//            .def("nmt",&sh::SHtmnDim::nmt)
            .def("i_from_mn",&sh::SHtmnDim::i_from_mn).staticmethod("i_from_mn")
            .def("mn_from_i",&sh::SHtmnDim::mn_from_i).staticmethod("mn_from_i");

    p::class_<sh::Legendre_nm_d>("Legendre_nm",p::init<int>())
            .def("__call__",&sh::Legendre_nm_d::operator())
            .def("indxnm", &sh::Legendre_nm_d::indxnm);


}