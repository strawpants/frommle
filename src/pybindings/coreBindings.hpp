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

//#include <boost/python.hpp>
////#include <boost/python/numpy.hpp>
//#include <boost/core/noncopyable.hpp>
//#include <boost/python/tuple.hpp>
//#include "core/seqGenerator.hpp"
//#include "object.h"
//#include "core/seqGenerator.hpp"
//namespace p = boost::python;
////namespace np = boost::python::numpy;
//#include "core/IndexGuide.hpp"
////#include "core/GuideAppender.hpp"

#ifndef FROMMLE_COREBINDINGS_HPP
#define FROMMLE_COREBINDINGS_HPP
namespace frommle{
    namespace guides {


        //implementation cna be found in GuidePackBindings.cpp
        void registerGuidePack();
    }

    namespace core{
        void registerGArrays();
        void registerGOperators();

    }
}

#endif //FROMMLE_COREBINDINGS_HPP
