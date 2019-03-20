#include <iostream>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "pybindings/coreBindings.hpp"
#include "core/GuideBase.hpp"

namespace p = boost::python;
namespace np = boost::python::numpy;

#include "sh/SHGuide.hpp"
#include "sh/Legendre_nm.hpp"
#include "../sh/SHGuide.hpp"
#include "../core/GuideBase.hpp"
#include "coreBindings.hpp"
#include "tupleconversion.hpp"

//namespace frommle {
//    namespace py {
//
//
////Wrapper class is needed to cope with pure virtual functions of SHGuideBase
//        struct SHGuideBaseWrapper : sh::SHGuideBase, p::wrapper<sh::SHGuideBase> {
//        public:
//            using sh::SHGuideBase::idx;
////            frommle::core::index   (SHGuideBaseWrapper::*idxfromEl)(const sh::SHGuideBase::Element)const = &sh::SHGuideBase::idx;
//            inline frommle::core::index   idxfromEl(const sh::SHGuideBase::Element El)const{ return this->sh::SHGuideBase::idx(El);}
//            inline frommle::core::index idxfromnmt(const int n,const int m, const sh::SHGuideBase::trig t)const{return this->idx(n,m,t);}
//
//            inline frommle::core::index idx(const int n, const int m, const sh::SHGuideBase::trig t)const {
//                return this->get_override("idx")(n,m,t);
//            }
//
//            Element operator[](const frommle::core::index idx) const {
//                return this->get_override("operator[]")(idx);
//            }
//        };
//    }
//}

using namespace frommle;

//Wrapper class is needed to cope with pure virtual functions of SHGuideBase
        struct SHGuideBaseWrapper : sh::SHGuideBase, p::wrapper<sh::SHGuideBase> {
        public:
            using sh::SHGuideBase::idx;
            using Element=sh::SHGuideBase::Element;
            using trig=sh::SHGuideBase::trig;
//            frommle::core::index   (SHGuideBaseWrapper::*idxfromEl)(const Element) const = &SHGuideBaseWrapper::idx;
//            frommle::core::index   (sh::SHGuideBase::*idxfromnmt)(const int,const int, const trig ) const = &sh::SHGuideBase::idx;
//            inline frommle::core::index   idxfromEl(const sh::SHGuideBase::Element El)const{ return this->idx(El);}
            inline frommle::core::index idxfromnmt(const int n,const int m, const sh::SHGuideBase::trig t)const{return this->idx(n,m,t);}

            inline frommle::core::index idx(const int n, const int m, const sh::SHGuideBase::trig t)const {
                return this->get_override("idx")(n,m,t);
            }

            inline frommle::core::index idx(const Element el)const {
                return this->SHGuideBase::idx(el);
            }

            Element operator[](const frommle::core::index idx) const {
                return this->get_override("operator[]")(idx);
            }
        };

//template<class T>
////Wrapper class is needed to cope with pure virtual functions of SHGuideBase
//struct SHGuideBaseWrapper : T, p::wrapper<T> {
//public:
//    //forward non-standard constructors to the base T
//    template<typename... Args>
//    SHGuideBaseWrapper(Args... args):p::wrapper<T>(),T(std::forward<Args...>(args)...){}
////    using p::wrapper<T>::p::wrapper;
//    using sh::SHGuideBase::idx;
//    using trig=sh::SHGuideBase::trig;
//    using Element=sh::SHGuideBase::Element;
//    using bpclass=p::class_<SHGuideBaseWrapper,p::bases<frommle::core::GuideBase> >;
////    inline frommle::core::index idxfromEl(const Element El)const{ return this->sh::SHGuideBase::idx(El);}
////    inline frommle::core::index idxfromnmt(const int n,const int m, const trig t)const{return this->idx(n,m,t);}
////
////    inline frommle::core::index idx(const int n, const int m, const trig t)const {
////        return this->get_override("idx")(n,m,t);
////    }
////
////    Element operator[](const frommle::core::index idx) const {
////        return this->get_override("operator[]")(idx);
////    }
//
//    /*
//     * does the boost python registration for this wrapper class
//     */
//    static bpclass & class_(char const * name){
//        return bpclass(name,p::init<int>()).def("nmax",p::pure_virtual(&sh::SHGuideBase::nmax))
//                .def("nmin",&T::nmin)
//                .def("idx",&T::idx)
////                .def("idx",&SHGuideBaseWrapper::idxfromEl)
//                .def("__getitem__",&T::operator[]);
//    }
//};

void pyexport_sh()
{


    p::enum_<sh::SHGuideBase::trig>("trig")
            .value("c",sh::SHGuideBase::trig::C)
            .value("s",sh::SHGuideBase::trig::S);

    py::register_tuple<sh::SHGuideBase::Element >();

    // p::to_python_converter<sh::SHGuideBase::Element, py::stdtuple_to_ptuple<sh::SHGuideBase::Element> >();
    // py::ptuple_to_stdtuple<sh::SHGuideBase::Element>();

    //the SHGuideBase
    p::class_<SHGuideBaseWrapper,p::bases<core::GuideBase>,boost::noncopyable>("SHGuideBase")
            .def("nmax",&sh::SHGuideBase::nmax)
            .def("nmin",&sh::SHGuideBase::nmin)
            .def("idx",&sh::SHGuideBase::idxfromEl)
            .def("idx",&SHGuideBaseWrapper::idxfromnmt)
            .def("__getitem__",p::pure_virtual(&sh::SHGuideBase::operator[]));

    p::class_<sh::SHtmnGuide,p::bases<sh::SHGuideBase> >("SHtmnGuide",p::init<int>())
            .def("idx",&sh::SHtmnGuide::idx)
            .def("idx",&sh::SHGuideBase::idxfromEl);
//            .def("idx",&SHGuideBaseWrapper::idxfromEl);
//            .def("i_from_mn",&sh::SHtmnGuide::i_from_mn).staticmethod("i_from_mn")
//            .def("mn_from_i",&sh::SHtmnGuide::mn_from_i).staticmethod("mn_from_i");


//    p::class_<SHGuideBaseWrapper,p::bases<frommle::core::GuideBase>,boost::noncopyable>("SHGuideBase")
//            .def("nmax",p::pure_virtual(&sh::SHGuideBase::nmax))
//            .def("nmin",p::pure_virtual(&sh::SHGuideBase::nmin))
//            .def("idx",&SHGuideBaseWrapper::idxfromnmt)
//            .def("idx",&SHGuideBaseWrapper::idxfromEl)
//            .def("__getitem__",p::pure_virtual(&sh::SHGuideBase::operator[]));

//        .def_readwrite("n",&sh::nmt::n)
//        .def_readwrite("m",&sh::nmt::m);
////        .def_readwrite('t',&sh::nmt::t);

//    p::def("i_from_mn",&sh::i_from_mn);
//    p::def("mn_from_i",&sh::mn_from_i);
    p::def("nmax_from_sz",&sh::nmax_from_sz);

//    SHGuideBaseWrapper<sh::SHtmnGuide>::class_("SHtmnGuide");
//            .def("i_from_mn",&sh::SHtmnGuide::i_from_mn).staticmethod("i_from_mn")
//            .def("mn_from_i",&sh::SHtmnGuide::mn_from_i).staticmethod("mn_from_i");
//    p::class_<sh::SHtmnGuide,p::bases<sh::SHGuideBase> >("StmnGuide",p::init<int>())
////            .def("idx",&SHtmnDim::idx)
////            .def("nmt",&sh::SHtmnDim::nmt)
//            .def("i_from_mn",&sh::SHtmnGuide::i_from_mn).staticmethod("i_from_mn")
//            .def("mn_from_i",&sh::SHtmnGuide::mn_from_i).staticmethod("mn_from_i");

    p::class_<sh::Legendre_nm_d>("Legendre_nm",p::init<int>())
            .def("__call__",&sh::Legendre_nm_d::operator())
            .def("indxnm", &sh::Legendre_nm_d::indxnm);

}