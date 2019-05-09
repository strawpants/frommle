/*! \file
 \brief Some tools to work with the geoslurp database. Note that this should have no dependencies on geoslurp code !!
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

#include "core/UserSettings.hpp"
#ifndef FROMMLE_GEOSLURP_HPP
#define FROMMLE_GEOSLURP_HPP

namespace frommle {
    namespace io {
        namespace geoslurp {

            std::string GDALSourceName() {
                using us=core::UserSettings;
                std::string source=std::string("dbname='")+us::at("geoslurp")["db"].as<std::string>()+
                        "' host='"+ us::at["geoslurp"]["host"].as<std::string>()+
                        "' port='"+ us::at("geoslurp")["port"].as<std::string>()+
                        "' user='"+ us::at("geoslurp")["user"].as<std::string>()+
                        "' password='"+ us::getAuth("geoslurp").as<std::string>()+"'";
                return source;
            }


        }
    }
}
#endif //FROMMLE_GEOSLURP_HPP
