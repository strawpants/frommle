/*! \file
 \brief provide boost python bindings for the frommle geometry module
 \copyright Roelof Rietbroek 2019
 \license
 This file is part of Frommle.
 frommle is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 Frommle is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with Frommle; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_by_value.hpp>
#include "../core/coreBindings.hpp"
#include <ogr_geometry.h>
#include "geometry/GuideMakerTools.hpp"

namespace p = boost::python;



namespace frommle{
    namespace geometry {

///@brief convert this class into a python shapely class
        template<class T>
        PyObject* toShapely(const T & geom ){
            p::object shapely=p::import("shapely.wkb");

            //load wkb string in data
            int sz=geom.WkbSize();
            unsigned char * wkb =new  unsigned char [sz];

            p::object memview(p::handle<>(PyMemoryView_FromMemory((char*)wkb,sz, PyBUF_READ)));
            geom.exportToWkb(wkbNDR,wkb,wkbVariantIso);

            p::object pygeom=shapely.attr("loads")(memview.attr("tobytes")());
            delete wkb;
            return p::incref(pygeom.ptr());

        }
//
//        //@brief class which allows to and from shapely conversion for an OGRPoint
//        struct PyOGRPoint{
//            PyOGRPoint(){
//                p::converter::registry::push_back(&convertible, &construct,p::type_id<OGRPoint>());
//                p::to_python_converter<OGRPoint, PyOGRPoint>();
//                p::register_ptr_to_python< std::shared_ptr<OGRPoint> >();
//            }
//
//            //from python conversion
//            static void* convertible(PyObject * py_obj){
//                auto shapelyPnt=p::import("shapely.geometry.point").attr("Point")();
//                if ( !PyObject_IsInstance(py_obj,shapelyPnt.ptr())){
//                    return 0;
//                }
//                return py_obj;
//            }
//
//
//            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {
//
//                //setup memory for C++ class
//                typedef p::converter::rvalue_from_python_storage<OGRPoint> storage_t;
//                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
//                void *memory_chunk = the_storage->storage.bytes;
//
//                auto pnt =p::object(p::handle<>(p::borrowed(py_obj))) ;
//                double X=p::extract<double>(pnt.attr("x"));
//                double Y=p::extract<double>(pnt.attr("y"));
//                new(memory_chunk) OGRPoint(X,Y);
//
//                data->convertible = memory_chunk;
//            }
//
//
//
//            ///@brief to python conversion
//            static PyObject *convert(OGRPoint const &tin) {
//                return toShapely<OGRPoint>(tin);
//            }
//
//        };
        //template<class T>
        //struct OGR_to_pyOGR{
            //static PyObject* convert( T const & OGRgeom ){
                ////p::object main=p::import("__main__");
                //p::object ogr=p::import("osgeo.ogr");

                ////load wkb string in data
                //int sz=OGRgeom.WkbSize();
                //unsigned char * wkb =new  unsigned char [sz];
                
                //p::object memview(p::handle<>(PyMemoryView_FromMemory((char*)wkb,sz, PyBUF_READ)));
                //OGRgeom.exportToWkb(wkbNDR,wkb,wkbVariantIso);

                //p::object pygeom=ogr.attr("CreateGeometryFromWkb")(memview);
                //delete wkb;
                //return p::incref(pygeom.ptr());
            //}
        //};

    
        //template<class T>
            //struct OGRshrd_ptr_to_pyOGR{
                //static PyObject* convert (std::shared_ptr<T> const & OGRgeomPtr){
                    //return OGR_to_pyOGR<T>::convert(*OGRgeomPtr);
                //}
            //};




    }
}



///@brief convert OGRGeometry to python shapely

//struct OGRPointWrapper:OGRPoint, p::wrapper<OGRPoint>{
   //public:
      /////@brief convert this class into a python shapely class 
      //PyObject* toShapely()const{
                //p::object shapely=p::import("shapely.wkb");

                ////load wkb string in data
                //int sz=WkbSize();
                //unsigned char * wkb =new  unsigned char [sz];
                
                //p::object memview(p::handle<>(PyMemoryView_FromMemory((char*)wkb,sz, PyBUF_READ)));
                //exportToWkb(wkbNDR,wkb,wkbVariantIso);

                //p::object pygeom=shapely.attr("loads")(memview);
                //delete wkb;
                //return p::incref(pygeom.ptr());

   //}
      
   //};





using namespace frommle::guides;
using namespace frommle::core;
using namespace frommle::geometry;
using OGRPointGuide=OGRGuide<OGRPoint>;

void (OGRPointGuide::*pb1)(const OGRPoint &)=&OGRPointGuide::push_back;
void (OGRPointGuide::*pb2)(const std::string &)=&OGRPointGuide::push_back;
const std::shared_ptr<OGRPoint> & (OGRPointGuide::*iget)(const size_t)const=&OGRPointGuide::operator[];

void pyexport_geometry()
{
    //register to and from pyhton conversion for OGRPoint
//    PyOGRPoint();
       p::class_<OGRPoint>("FrOGRPoint",p::init<double,double>())
      .def("toShapely",&toShapely<OGRPoint>);

    p::register_ptr_to_python< std::shared_ptr<OGRPoint> >();

    p::class_<OGRPointGuide,p::bases<GuideBase>>("PointGuide",p::init<p::optional<std::string>>())
        .def("append",pb1)
        .def("append",pb2)
        //.def("idx",&OGRPointGuide::idx)
        .def("__getitem__",iget,p::return_value_policy<p::copy_const_reference>())
        .def("__iter__",p::iterator<const OGRPointGuide>());
   
    p::def("makeFibonacciGrid",&makeFibonacciGrid);
}
