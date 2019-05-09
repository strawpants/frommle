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
#include "core/Exceptions.hpp"
#include <chrono>
#include <ctime>
#include <libsecret/secret.h>

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
				auto username= std::string(std::getenv("USER"));
				config_["User"]=username;
				config_["Contact"]= username + "@unknown";

				config_["Authstore"]="libsecret";
				config_["Defaultdb"]="geoslurp";
				config_["geoslurp"]["db"]= std::string("geoslurp");
				config_["geoslurp"]["host"]= std::string("hostname");
				config_["geoslurp"]["port"]= std::string("5432");
				config_["geoslurp"]["user"]= username;
		}

		void UserSettings::readYaml(const std::string yamlfile) {
			config_ = YAML::LoadFile(yamlfile);
		}

		//@brief write current yaml tree to a stream
		void UserSettings::write(std::ostream & fout) {
			auto tnow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			fout << "# Frommle yaml config file, written " <<std::ctime(&tnow);
			fout << "# Frommle version: " << FRVERSION << std::endl;
			std::string method=UserSettings::at("Authstore").as<std::string>();
			if (method == "unsecure"){
				fout << "# WARNING passwords are stored unencrypted (consider using Authstore: libsecret)" << std::endl;
			}
			fout << UserSettings::get().config_ <<std::endl;
		}

		void UserSettings::write(const std::string filename) {
				if (filename == "-"){
					UserSettings::write(std::cout);
				}else {
					std::ofstream fout(filename, std::ios::out | std::ios::trunc);
					UserSettings::write(fout);
					fout.close();
				}
		}

		void UserSettings::write() {
				if (!UserSettings::yamlfile().empty()){
					UserSettings::write(UserSettings::yamlfile());
				}
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

		std::string UserSettings::getAuth(const  std::string alias) {
			std::string method=UserSettings::get().config_["Authstore"].as<std::string>();

			if (method == "libsecret") {
				//try to retrieve the authentification credentials from the gnome-keyring
				return UserSettings::get().getAuthlibsecret(alias);
			}else if (method == "unsecure"){
				return UserSettings::get().getAuthUnsecure(alias);
			}else{
				throw InputException("Unknown method to retrieve authentification secrets: "+ method);
			}

		}

		//@brief stores a secret
		void UserSettings::setAuth(const std::string alias, const std::string secret) {
			std::string method=UserSettings::at("Authstore").as<std::string>();
			if (method == "libsecret") {
				//try to retrieve the authentification credentials from the gnome-keyring
				UserSettings::get().setAuthlibsecret(alias, secret);
			}else if (method == "unsecure"){
				UserSettings::get().setAuthUnsecure(alias, secret);
			}else{
				throw InputException("Unknown method to store authentification secrets: "+ method);
			}

		}

		//@brief store secret passwords in an unsecure way (plain text in config_ tree)
		void UserSettings::setAuthUnsecure(const std::string alias, const std::string secret) {
			config_["Secrets"][alias]=secret;
		}

		//@brief retrieves secrets stored in an unsecure way from config_ tree
		std::string UserSettings::getAuthUnsecure(const std::string alias) const {
			return config_["Secrets"][alias].as<std::string>();
		}

		const SecretSchema *
		frommle_get_schema (void)
		{
			static const SecretSchema the_schema = {
					"org.frommle.Password", SECRET_SCHEMA_NONE,
					{
							{  "alias", SECRET_SCHEMA_ATTRIBUTE_STRING },
							{  "NULL", SECRET_SCHEMA_ATTRIBUTE_STRING },
					}
			};
			return &the_schema;
		}

		//@brief lookup a password or secret token through the libsecret method
		std::string UserSettings::getAuthlibsecret(const std::string alias) const {
			GError *error = NULL;
			gchar *password = secret_password_lookup_sync (frommle_get_schema(), NULL, &error,
					"alias",alias.c_str(),
					NULL);

			if (error != NULL) {
				/* ... handle the failure here */
				g_error_free (error);

			}

			return std::string(password);
		}

		//@brief stores a secret with libsecret
		void UserSettings::setAuthlibsecret(const std::string alias, const std::string secret) const {
			GError *error = NULL;

			secret_password_store_sync (frommle_get_schema(), SECRET_COLLECTION_DEFAULT,
										"Frommle", secret.c_str(), NULL, &error,
										"alias", alias.c_str(),
										NULL);

			if (error != NULL) {
				/* ... handle the failure here */
				g_error_free (error);
			} else {
				/* ... do something now that the password has been stored */
			}

		}

	}

}


