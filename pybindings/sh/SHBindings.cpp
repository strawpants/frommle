#include <iostream>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "../core/coreBindings.hpp"
#include "core/GuideBase.hpp"
#include "../core/numpyConverting.hpp"
#include "sh/SHisoOperator.hpp"
namespace p = boost::python;
namespace np = boost::python::numpy;

#include "sh/SHGuide.hpp"
#include "sh/Legendre_nm.hpp"
#include "sh/SHGuide.hpp"
#include "core/GuideBase.hpp"
#include "../core/coreBindings.hpp"
#include "../core/tupleconversion.hpp"
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

namespace frommle{
    namespace guides{

//Wrapper class is needed to cope with pure virtual functions of SHGuideBase
        struct SHGuideBaseWrapper : SHGuideBase, p::wrapper<SHGuideBase> {
        public:
            using SHGuideBase::idx;
            using Element=SHGuideBase::Element;
            using trig=SHGuideBase::trig;

//            frommle::core::index   (SHGuideBaseWrapper::*idxfromEl)(const Element) const = &SHGuideBaseWrapper::idx;
//            frommle::core::index   (sh::SHGuideBase::*idxfromnmt)(const int,const int, const trig ) const = &sh::SHGuideBase::idx;
//            inline frommle::core::index   idxfromEl(const sh::SHGuideBase::Element El)const{ return this->idx(El);}
            inline size_t idxfromnmt(const int n, const int m, const SHGuideBase::trig t) const { return this->idx(n, m, t); }

            inline size_t idx(const int n, const int m, const SHGuideBase::trig t) const {
                return this->get_override("idx")(n, m, t);
            }

            inline size_t idx(const Element el) const {
                return this->SHGuideBase::idx(el);
            }

            Element operator[](const size_t idx) const {
                return this->get_override("operator[]")(idx);
            }
        };
    }
}


using namespace frommle::guides;

using namespace frommle::sh;
using namespace frommle;

template<class T>
struct register_shisooperator{
    register_shisooperator(std::string basename){
        p::class_<SHisoOperator<T>,p::bases<core::GOperatorDiag<T>>>(basename.c_str())
                .def(p::init<int>())
                .def(p::init<std::vector<T>>());


    }
};


void pyexport_sh()
{


    p::enum_<SHGuideBase::trig>("trig")
            .value("c",SHGuideBase::trig::C)
            .value("s",SHGuideBase::trig::S);

    frommle::py::register_tuple<SHGuideBase::Element >();

    //export SHHalfguide
    size_t (SHnmHalfGuide::*idxfromel)(const SHnmHalfGuide::Element &)const=&SHnmHalfGuide::idx;
    size_t (SHnmHalfGuide::*idxfromnm)(const int, const int)const=&SHnmHalfGuide::idx;
    p::class_<SHnmHalfGuide,p::bases<GuideBase>>("SHnmHalfGuide",p::init<int>())
    .add_property("nmax",&SHnmHalfGuide::nmax)
    .add_property("nmin",&SHnmHalfGuide::nmin)
    .def("idx",idxfromel)
    .def("idx",idxfromnm)
    .def("__getitem__",&SHnmHalfGuide::operator[])
    ;


    //the SHGuideBase
    p::class_<SHGuideBaseWrapper,p::bases<GuideBase>,boost::noncopyable>("SHGuideBase")
            .add_property("nmax",&SHGuideBase::nmax)
            .add_property("nmin",&SHGuideBase::nmin)
            .def("idx",&SHGuideBase::idxfromEl)
            .def("__getitem__",p::pure_virtual(&SHGuideBase::operator[]))
//            .def("__iter__",p::range<p::return_value_policy<p::copy_const_reference>>(&SHGuideBase::begin,&SHGuideBase::end))
            .def("__iter__",p::iterator<const SHGuideBase>())
//            .def("__iter__",p::iterator<const SHGuideBase>())
            .def("__array__",&py::guide_to_ndarray<SHGuideBase>);

    //export subclass SHtmnGuide
    p::class_<SHtmnGuide,p::bases<SHGuideBase> >("SHtmnGuide").def(p::init<int,p::optional<std::string>>());
            //.def("i_from_mn",&sh::SHtmnGuide::i_from_mn).staticmethod("i_from_mn")
            //.def("mn_from_i",&sh::SHtmnGuide::mn_from_i).staticmethod("mn_from_i");

    //export subclass SHnmtGuide
    p::class_<SHnmtGuide,p::bases<SHGuideBase> >("SHnmtGuide").def(p::init<int,p::optional<std::string>>())
            .def("i_from_nmt",&SHnmtGuide::i_from_nmt).staticmethod("i_from_nmt");



    p::def("nmax_from_sz",&nmax_from_sz);

    p::class_<Legendre_nm<double>,p::bases<core::GArrayDyn<double,1>>>("Legendre_nm",p::init<int>())
            .def("set",&sh::Legendre_nm_d::set,p::return_value_policy<p::copy_non_const_reference>());
//            .def("indxnm", &sh::Legendre_nm_d::indxnm);

    //register SHisoperator
    register_shisooperator<double>("shisoperator_float64");



}
