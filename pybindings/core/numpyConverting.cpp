/*! \file
 \brief 
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

#include "numpyConverting.hpp"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace frommle{

    namespace py{

        core::slice convPyslice(p::slice pslice){

            ptrdiff_t start=0;
            ptrdiff_t stop=-1;
            ptrdiff_t step=1;

            if(pslice.start() != p::slice_nil()){
                start=p::extract<ptrdiff_t>(pslice.start());
            }

            if(pslice.stop() != p::slice_nil()) {
                stop = p::extract<ptrdiff_t>(pslice.stop());
            }

            if(pslice.step() != p::slice_nil()) {
                step = p::extract<ptrdiff_t>(pslice.step());
            }

            return {{start,stop,step}};
        }


    template<class T>
    struct pyvec{

        static void reg(const char * name){
        p::class_<std::vector<T> >(name).def(p::vector_indexing_suite<std::vector<T> >())
            .def("reserve",&pyvec::reserve);
    
        }
        static void reserve(std::vector<T> & vin,size_t n){
            //LOGINFO << "reserving " << n <<std::endl;
            vin.reserve(n);
        }
    };


    void register_numpy_converters(){
        
        //register vector <-> ndarray converters
        pyvec<double>::reg("V_float64");
        pyvec<size_t>::reg("V_size_t");

        register_mar<double,6>();
        register_mar<size_t,6>();

        register_hslab<double>();

    }        
    }
}
