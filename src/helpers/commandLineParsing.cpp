/*! \file
 \brief Implements copmmand lin parsing helper for frommle programs
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

#include "helpers/commandLineParsing.hpp"
namespace frommle{
    namespace helpers {

        FrommleOpts::FrommleOpts(const std::string usageString):CustomOpts_("Program options"),vm(),
            ComOpts_("Common options"),AllOptions_(""),PosOpts_(),Usage(usageString) {
            //always add the help option
            ComOpts_.add_options()("help,h","Display this help message");
            addCommonOpts<VersionOpt>();

        }


        void frommle::helpers::FrommleOpts::printHelp() const {
            std::cout << Usage << std::endl;
            std::cout << CustomOpts_ << std::endl;
            std::cout << ComOpts_ << std::endl;
        }


        void frommle::helpers::FrommleOpts::parseCommandLine(int argc, char **argv) {
            //merge all possible option description groups
            AllOptions_.add(ComOpts_).add(CustomOpts_);
            try{
                po::store(po::command_line_parser(argc,argv).options(AllOptions_).positional(PosOpts_).run(),vm);

                //check if help has been requested
                if(vm.count("help")){
                    this->printHelp();
                    exit(0);//directly exit program without error
                }
                po::notify(vm);


            }catch(po::error & parseError){
                LOGERROR << "processing command line arguments " << argv[0]<< std::endl;
                LOGERROR << parseError.what() << std::endl;
                LOGERROR << "Try --help for usage info" << std::endl;
                throw(parseError);
            }

        }



    }
}
