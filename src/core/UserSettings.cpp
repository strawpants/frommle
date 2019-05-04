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
#include "UserSettings.hpp"

#include <chrono>
#include <ctime>
namespace frommle {
	namespace core {
			//@brief reads in the user settings from a yaml file
			UserSettings::UserSettings(){
				//set the default file to search for
				yamlfile_=std::string(std::getenv("HOME"))+"/.frommle.yaml";
				std::ifstream fin(yamlfile_);

				if (fin.good()){
					fin.close();
					//try to read the file into the root yaml:node
					readYaml(yamlfile_);
				}else{
					setDefaults();

				}

			}

		void UserSettings::setDefaults()  {//fill with default values
			config_["User"]= std::string(std::getenv("USER"));
			config_["Contact"]= config_["User"].as<std::string>() + "@unknown";

			config_["Authstore"]="gnome-keyring";

			config_["geoslurp"]["host"]= std::string("karpaten");
			config_["geoslurp"]["port"]= std::string("5432");
			config_["geoslurp"]["user"]= std::string("geoslurp");
		}

		void UserSettings::readYaml(const std::string yamlfile) {
			config_ = YAML::LoadFile(yamlfile);
		}

		//@brief write current yaml tree to a stream
		void UserSettings::write(std::ostream & fout) {
			auto tnow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			fout << "# Frommle yaml config file, written " <<std::ctime(&tnow) <<std::endl;
			fout << UserSettings::get().config_;
		}

		void UserSettings::write(const std::string filename) {
			std::ofstream fout(filename,std::ios::out|std::ios::trunc);
			UserSettings::write(fout);
			fout.close();
		}

		void UserSettings::write() {
			UserSettings::write(UserSettings::yamlfile());
		}

		void UserSettings::read(const std::string filename) {
			UserSettings::get().readYaml(filename);
		}

		void UserSettings::read() {
			UserSettings::read(UserSettings::yamlfile());
		}

		std::string UserSettings::yamlfile() {
			return UserSettings::get().yamlfile_;
		}

		YAML::Node UserSettings::getAuth(const  std::string alias) {
			YAML::Node authnode;
			std::string method=UserSettings::get().config_["Authstore"].as<std::string>();

			if (method == "gnome-keyring") {
				//try to retrieve the authentification credentials from the gnome-keyring
				authnode["password"]="Blah blah this is not a password";
			}

			return authnode;

		}
	}

}


