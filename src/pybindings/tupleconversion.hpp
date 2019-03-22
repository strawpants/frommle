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


#ifndef FROMMLE_TUPLECONVERSION_HPP
#define FROMMLE_TUPLECONVERSION_HPP

#include <boost/python.hpp>
#include "core/seqGenerator.hpp"

namespace p=boost::python;

namespace frommle {
    namespace py {

        template<class T>
        struct stdtuple_to_ptuple {
            static PyObject *convert(T const &tin) {
                return p::incref(
                        getptuple(tin, typename frommle::core::seqGenerator<std::tuple_size<T>::value>::type()).ptr());

            }

            template<int ...S>
            static p::tuple getptuple(T const &tin, frommle::core::sequence<S...>) {
                return p::make_tuple(std::get<S>(tin)...);
            }

        };


        template<class T>
        struct ptuple_to_stdtuple {
        public:
            using seqtype=typename frommle::core::seqGenerator<std::tuple_size<T>::value>::type;

            static void *convertible(PyObject *py_obj) {
                if (!PySequence_Check(py_obj)) {
                    return 0;
                }

                if (!PyObject_HasAttrString(py_obj, "__len__")) {
                    return 0;
                }

                p::object ptuple(p::handle<>(p::borrowed(py_obj)));

                if (!isconvertible_impl(ptuple, seqtype())) {
                    return 0;
                }

                //if we land here we're good to go witht he conversion
                return py_obj;
            }

            template<int ...S>
            static bool isconvertible_impl(p::object &pin, frommle::core::sequence<S...>) {

                std::array<bool, std::tuple_size<T>::value> chk = {
                        p::extract<typename std::tuple_element<S, T>::type>(pin[S]).check()...};
                return std::all_of(chk.begin(), chk.end(), [](bool i) { return i; });
            }

            static void construct(PyObject *py_obj, p::converter::rvalue_from_python_stage1_data *data) {
                typedef p::converter::rvalue_from_python_storage<T> storage_t;
                storage_t *the_storage = reinterpret_cast<storage_t *>( data );
                void *memory_chunk = the_storage->storage.bytes;

                p::object ptuple(p::handle<>(p::borrowed(py_obj)));

                T *c_tuple = new(memory_chunk) T(construct_impl(ptuple, seqtype()));
                data->convertible = memory_chunk;
            }

            template<int ...S>
            static T construct_impl(p::object const &pin, frommle::core::sequence<S...>) {
                T tpl{p::extract<typename std::tuple_element<S, T>::type>(pin[S])...};
                return tpl;
            }


        };

/* @brief register tuple conversions from and to python
 *
 */
        template<class T>
        void register_tuple() {

            p::to_python_converter<T, stdtuple_to_ptuple<T>>();
            p::converter::registry::push_back(&ptuple_to_stdtuple<T>::convertible, &ptuple_to_stdtuple<T>::construct,
                                              p::type_id<T>());
        }
    }
}

#endif //FROMMLE_TUPLECONVERSION_HPP
