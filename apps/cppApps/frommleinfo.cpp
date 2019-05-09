/*! \file
 \brief  Contains a main program to modify common frommle settings
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

#include "commandLineParsing.hpp"
#include "core/UserSettings.hpp"

namespace fcli=frommle::cli;

int main(int argc, char **argv) {

    //setup Usage info for the program
    fcli::FrommleOpts frOpts("Display and change userspace settings for frommle\n"
                           "Usage: frommleinfo [options]");

    //Add some default options (positional independent input filename)
    frOpts.addCommonOpts<fcli::ConfigOpt>();
    frOpts.addCommonOpts<fcli::ConfigwriteOpt>();
    //try command line argument parsing
    try {
        frOpts.parseCommandLine(argc, argv);
    } catch (po::error &parseError) {
        exit(1);
    }

    //possibly write yaml configuration
    if (frOpts.vm.count("config-write")){
        frommle::core::UserSettings::write(frOpts.vm["config-write"].as<std::string>());

    }

}