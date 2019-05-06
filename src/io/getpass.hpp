/*! \file
 \brief Query for a user password (code modified from https://www.gnu.org/software/libc/manual/html_node/getpass.html)
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

#ifndef FROMMLE_GETPASS_HPP
#define FROMMLE_GETPASS_HPP


#include <termios.h>
#include <stdio.h>
#include <iostream>
namespace frommle{
    namespace io{
        std::string getpass(const std::string prompt);
    }
}


#endif //FROMMLE_GETPASS_HPP
