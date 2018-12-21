/*@file MacroMagic.hpp
@brief Encapsulate ugly compiler preprocesor checks for (un)supported features
@copyright Roelof Rietbroek 2016
@license
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

#ifndef SRC_CORE_MACROMAGIC_HPP_
#define SRC_CORE_MACROMAGIC_HPP_


//some ugly preproccessor magic to insert source file and linenumbers as strings in the code
#define Q(x) #x
#define QUOTE(x) Q(x)
//#define HERE __FILE__ QUOTE(:__LINE__)

#define HERE std::string(__FILE__ ":" QUOTE(__LINE__) "\n\tin ")+ std::string(__PRETTY_FUNCTION__)+" "

//first assume all compilers support get_time
#define WITH_STD_GET_TIME

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)

//alas gcc compilers below version 5 do not support get_time
#if GCC_VERSION < 50000
#undef WITH_STD_GET_TIME
#endif

#endif


#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef M_PIl
#define M_PIl (3.14159265358979323846264338327950288)
#endif

#define D2R (M_PI/180)

#define D2Rl (M_PIl/180)


#endif /* SRC_CORE_MACROMAGIC_HPP */
