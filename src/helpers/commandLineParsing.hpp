/*! \file
 \brief Contains some classes which facilitate writing command line programs (e.g. registration of common command line options)
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

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include "core/Exceptions.hpp"
#include "core/Logging.hpp"
#include <iostream>
#include "core/UserSettings.hpp"
#include "io/getpass.hpp"
#ifndef FROMMLE_COMMANDLINEPARSING_HPP
#define FROMMLE_COMMANDLINEPARSING_HPP

namespace po = boost::program_options;

namespace frommle{

namespace helpers {


//@brief abstract struct to setup a common options
    struct CommonOpt {
        virtual void addOpts(po::options_description &OptsDescr) const=0;

        //@brief possibly register option as positional (there is no need to implement this function if the option is not positional)
        virtual void registerPositional(po::positional_options_description &PosOpts) const {}
    };


    class FrommleOpts {
    public:
        FrommleOpts(const std::string usageString);

        FrommleOpts();//empty constructor is declared but not implemented, to force users to submit a usage string

        //@brief forward add_options call to CustomOpts instance
        inline po::options_description_easy_init add_options() { return CustomOpts_.add_options(); }

        //@brief adds a common option
        template<class T>
        void addCommonOpts();

        void parseCommandLine(int argc, char **argv);

        void printHelp() const;

        //make the variables map publically accessible (currently I don't see the need to wrap this in a getter function ..)
        po::variables_map vm;
    private:
        po::options_description CustomOpts_;
        po::options_description ComOpts_;
        po::options_description AllOptions_;
        po::positional_options_description PosOpts_;
        std::string Usage;
    };

    template<class T>
    void FrommleOpts::addCommonOpts() {
        std::unique_ptr<T>Copt(new T);
        Copt->addOpts(ComOpts_);
        Copt->registerPositional(PosOpts_);
    }
//Set up common options


//@brief common option to define an input file
        struct InputOpt : public CommonOpt {
            void addOpts(po::options_description &OptsDescr) const {

                OptsDescr.add_options()("input", po::value<std::string>(),
                                        "Explicitly specify input source (default takes the "
                                        "last non-optional command line argument as input source. "
                                        "Standard input is used when - is specified as a source\n");

            }

            void registerPositional(po::positional_options_description &PosOpts) const {
                //add this option to the positional arguments
                PosOpts.add("input", 1);
            }

        };

    struct OutputOpts:public CommonOpt {
        void addOpts(po::options_description & OptsDescr)const{
            OptsDescr.add_options()("output", po::value<std::string>()->default_value("-"),
                                    "Specify output source ('-' denotes standard output)");
        }
    };


        struct ConfigwriteOpt:public CommonOpt {
            void addOpts(po::options_description & OptsDescr)const{
                OptsDescr.add_options()("config-write", po::value<std::string>()->implicit_value(core::UserSettings::yamlfile())->multitoken()->zero_tokens(),
                                        "Where to write the configuration to ('-' denotes standard output/input)");
            }
        };

        //@brief display the frommle version number
        struct VersionOpt:public CommonOpt{
            void addOpts(po::options_description &OptsDescr) const {
                OptsDescr.add_options()("version,v",po::bool_switch()->notifier(VersionOpt::printVersion),"Show frommle version");
            }
            static void printVersion(const bool printv){
                if(printv){
                    std::cout << FRVERSION << " libbuild: "<< __DATE__ << " " << __TIME__<< std::endl;
                }
            }
        };

        //@brief display or change user settings
        struct ConfigOpt:public CommonOpt{
            void addOpts(po::options_description &OptsDescr) const {

                OptsDescr.add_options()("config", po::value<std::string>()->notifier(ConfigOpt::processConfig),
                                        "Display or set configuration settings. Options can be:\n"
                                        "User:USERNAME (set user name)\n"
                                        "Contact:EMAIL (set contact)\n"
                                        "Secrets:ALIAS (store password associated with the ALIAS key (prompts for password)");
            }
            static void processConfig(std::string opt){
                std::vector<std::string> splitOpt;
                boost::split(splitOpt, opt, boost::is_any_of(":"));
//                std::cout <<"option string"<< splitOpt[0] <<std::endl;
                if(splitOpt[0] == "User" || splitOpt[0] == "Contact") {
                    core::UserSettings::at(splitOpt[0]) = splitOpt[1];
                }else if (splitOpt[0]=="Secrets"){
                    auto passw=io::getpass(std::string("Enter secret token (e.g. password) for alias: ")+splitOpt[1]);
                    core::UserSettings::setAuth(splitOpt[1],passw);
                }else{
                    throw frommle::core::InputException("Cannot parse config option");
                }


            }

        };


    }
}

#endif //FROMMLE_COMMANDLINEPARSING_HPP
