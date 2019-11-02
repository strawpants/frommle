/*! \file
 \brief converts boost time classes to and from python (inspired by http://code.activestate.com/recipes/576395-convert-datetimedatetime-objects-tofrom-boostpytho/)
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


#ifndef FROMMLE_BDATETIMECONVERSION_HPP
#define FROMMLE_BDATETIMECONVERSION_HPP

#include <boost/python.hpp>
#include "core/seqGenerator.hpp"
#include <boost/date_time.hpp>
#include <datetime.h>
namespace p=boost::python;

namespace frommle {
    namespace py {
        using gregdate=boost::gregorian::date;
        using ptime=boost::posix_time::ptime;
        using duration=boost::posix_time::time_duration;

        static long get_usecs(boost::posix_time::time_duration const& d)
        {
            static long resolution
                    = duration::ticks_per_second();
            long fracsecs = d.fractional_seconds();
            if (resolution > 1000000)
                return fracsecs / (resolution / 1000000);
            else
                return fracsecs * (1000000 / resolution);
        }


        struct bgregdate_to_pdatetime {
            static PyObject *convert(gregdate const &tin) {
                return PyDateTime_FromDateAndTime(tin.year(),tin.month(),tin.day(),0,0,0,0);

            }
        };

        struct bptime_to_pdatetime {
            static PyObject *convert(ptime const &tin) {
                auto date=tin.date();
                auto tod=tin.time_of_day();
                return PyDateTime_FromDateAndTime(date.year(),date.month(),date.day(),
                        tod.hours(),tod.minutes(),tod.seconds(),get_usecs(tod));

            }
        };

        struct pdatetime_to_bgregdate {
        public:
            pdatetime_to_bgregdate(){
                p::converter::registry::push_back(&convertible, &construct,p::type_id<gregdate>());
            }

            static void* convertible(PyObject * py_obj){
                if ( ! PyDateTime_Check(py_obj)){
                         return 0;
                }
                return py_obj;
            }

            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {
                
                //setup memory for C++ class
                typedef p::converter::rvalue_from_python_storage<gregdate> storage_t;
                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
                void *memory_chunk = the_storage->storage.bytes;
                
                PyDateTime_DateTime const* pdatetime = reinterpret_cast<PyDateTime_DateTime*>(py_obj);
                new(memory_chunk) gregdate(PyDateTime_GET_YEAR(pdatetime),
                        PyDateTime_GET_MONTH(pdatetime),
                        PyDateTime_GET_DAY(pdatetime));

                data->convertible = memory_chunk;
            }
        };

        struct pdatetime_to_bptime {
        public:
            pdatetime_to_bptime(){
                p::converter::registry::push_back(&convertible, &construct,p::type_id<ptime>());
            }

            static void* convertible(PyObject * py_obj){
                if ( ! PyDateTime_Check(py_obj)){
                    return 0;
                }
                return py_obj;
            }

            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {

                //setup memory for C++ class
                typedef p::converter::rvalue_from_python_storage<ptime> storage_t;
                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
                void *memory_chunk = the_storage->storage.bytes;

                PyDateTime_DateTime const* pdatetime = reinterpret_cast<PyDateTime_DateTime*>(py_obj);
                gregdate date(PyDateTime_GET_YEAR(pdatetime),
                                                PyDateTime_GET_MONTH(pdatetime),
                                                PyDateTime_GET_DAY(pdatetime));
                duration tod(PyDateTime_DATE_GET_HOUR(pdatetime),
                                  PyDateTime_DATE_GET_MINUTE(pdatetime),
                                  PyDateTime_DATE_GET_SECOND(pdatetime),
                                  0);
                // Set the usecs value
                tod += boost::posix_time::microseconds(PyDateTime_DATE_GET_MICROSECOND(pdatetime));


                new(memory_chunk) ptime(date,tod);
                data->convertible = memory_chunk;
            }
        };
        
        void register_datetime() {
            PyDateTime_IMPORT;
            p::to_python_converter<gregdate, bgregdate_to_pdatetime>();
            pdatetime_to_bgregdate();

            p::to_python_converter<ptime, bptime_to_pdatetime>();
            pdatetime_to_bptime();
        }
    }
}

#endif
