/*! \file
 \brief read in settings from a yaml file and possibly passwords from a gnome-keyring
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


#ifndef SRC_CORE_USER_SETTINGS_HPP_
#define SRC_CORE_USER_SETTINGS_HPP_

#include "core/Singleton.hpp"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <ostream>

namespace frommle {
	namespace core {

	class UserSettings: public Singleton<UserSettings>{
		public:
			template<class T>
			static YAML::Node at(const T & key){return UserSettings::get().config_[key];}
			static void write();
			static void write(const std::string filename);

			static void write(std::ostream & fout);

			static void read(const std::string filename);
			static void read();

			static std::string yamlfile();

			static YAML::Node getAuth(const std::string alias);
			void setDefaults();
		private:
			friend Singleton<UserSettings>;

			//@brief reads in the user settings from a yaml file
			UserSettings();
			void readYaml(const std::string yamlfile);
			YAML::Node config_{};
			std::string yamlfile_="";

	};
	}
}


#endif /* #SRC_CORE_USER_SETTINGS_HPP_ */
