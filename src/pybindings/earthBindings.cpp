#include <boost/python.hpp>
#include "earthmodels/EarthConstants.hpp"

namespace p =boost::python;
namespace fe=frommle::earthmodels;

void pyexport_earth(){

//Register Earth constants for use in python
    p::scope().attr("rho_water")=fe::RHO_WATER;
    p::scope().attr("rho_seawater")=fe::RHO_SEAWATER;
    p::scope().attr("rho_earth")=fe::RHO_EARTH;
    p::scope().attr("rho_ice")=fe::RHO_ICE;

//register WGS84 ellipsoid
    p::class_<fe::WGS84>("WGS84")
    .add_static_property("a",p::make_getter(&fe::WGS84::a))
    .add_static_property("finv",p::make_getter(&fe::WGS84::finv))
    .add_static_property("omegaRate",p::make_getter(&fe::WGS84::omegaRate))
    .add_static_property("GM",p::make_getter(&fe::WGS84::GM));



}
