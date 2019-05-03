/*! \file
 \brief Implementation of reading in user settings from a yaml file 
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
#include <cstdlib>
#include <fstream>
#include "core/Logging.hpp"
namespace frommle {
	namespace core {

//@brief reads in the user settings from a yaml file
	UserSettings::UserSettings(){
				std::string yamlfile(std::string(std::getenv("HOME"))+"/.frommle.yaml");
				std::ifstream fin(yamlfile);

				if (fin.good()){
					fin.close();
					//try to read the file into a yaml:node 
					config_ = YAML::LoadFile(yamlfile);
				}

			}
	}

}


