/*! \file
 \brief 
 \copyright Roelof Rietbroek 2018
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

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/python/tuple.hpp>
#include <boost/multi_array.hpp>
#include "core/seqGenerator.hpp"
#include "object.h"
#include "core/seqGenerator.hpp"

namespace p = boost::python;
namespace np = boost::python::numpy;
#include "pybindings/tupleconversion.hpp"

#ifndef FROMMLE_COREBINDINGS_HPP
#define FROMMLE_COREBINDINGS_HPP
namespace frommle{
    namespace py{

        ///@brief convert an element to a numpy dtype (specialize for each valid Element)
        template<class Element>
        struct to_np_dtype{};



        template<class T>
        struct vec_to_ndarray{
            static PyObject* convert( std::vector<T> const & invec){
                p::tuple shape = p::make_tuple(invec.size());
                np::dtype dtype = np::dtype::get_builtin<T>();
//        np::ndarray py_array = np::from_data(invec.data(), dtype, p::make_tuple(invec.size()),p::make_tuple(sizeof(ftype)),p::object());
                np::ndarray py_array = np::empty(shape, dtype);
                T * dptr = reinterpret_cast<T*> (py_array.get_data());
                //copy values (not very efficient for large vectors)
                for (auto & el: invec){
                    *dptr=el;
                    dptr++;
                }
                return p::incref(py_array.ptr());
            }
        };




        template<int ndim>
        struct arr_to_ptuple{
            template<class Arr>
            static PyObject* convert(const Arr &in){
                return p::incref(make(in).ptr());
            }

            template<class Arr>
            static p::tuple make(const Arr &in){
                return ptuple_from_array(in,typename frommle::core::seqGenerator<ndim>::type());
            }
            template<class Arr, int ...S>
            static p::tuple ptuple_from_array(const Arr & in, frommle::core::sequence<S...>){
                return p::make_tuple(in[S]...);
            }
        };


        template<typename T, size_t ndim>
        struct marray_to_ndarray{
            static PyObject* convert ( boost::multi_array<T,ndim> & marr){
                return p::incref(get(marr).ptr());
            }
            static np::ndarray get(boost::multi_array<T,ndim> & marr){
                p::tuple shape = arr_to_ptuple<ndim>::make(marr.shape());
                p::tuple strides = arr_to_ptuple<ndim>::make(marr.strides());
                np::dtype dtype = np::dtype::get_builtin<T>();
                return np::from_data(marr.data(),dtype,shape,strides,p::object());
            }
        };


    }
}

#endif //FROMMLE_COREBINDINGS_HPP
