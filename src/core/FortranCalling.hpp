/*!@file F77calling.hpp
@brief File Description
@copyright Roelof Rietbroek 2014
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

#ifndef FROMMLE_FORTRAN_HPP
#define FROMMLE_FORTRAN_HPP

typedef int F77Int;
typedef float F77Float;
typedef double F77Double;
typedef unsigned long long int F77int64;

//For portability reasons in the future, this macro may choose to use either upper or lower case routine names to call fortran
#define F77CALL(ucname,lcname) lcname ## _



#endif /* FROMMLE_FORTAN_HPP */
