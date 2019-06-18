/*! \file CFConventions.hpp
 \brief 
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

 Created by Roelof Rietbroek,  2019

 */


#include "core/Exceptions.hpp"
#include "core/UserSettings.hpp"
#include <boost/any.hpp>

#ifndef FROMMLE_CFCONVENTIONS_HPP
#define FROMMLE_CFCONVENTIONS_HPP
namespace frommle{
    namespace io{

        class CFConventions{
        public:
            template<class Ar>
            static void SetGlobalAttr(Ar & ar);

            template<class Ar>
            static void SetGroupAttr(Ar & ar);

            template<class Ar>
            static void SetDataAttr(Ar & ar);

        };


        template<class Ar>
        void CFConventions::SetGlobalAttr(Ar &ar) {
            ar.setAttr("Conventions","CF-1.7");
            ar.setAttr("history","created: "+core::currentTimetag());
            try {
                ar.setAttr("institution", core::UserSettings::at("Institution").as<std::string>());
            }catch(core::InputException & excep) {
            //ok, this is just an optional feature
            }

            try {
                ar.setAttr("contact", core::UserSettings::at("Contact").as<std::string>());
            }catch(core::InputException & excep) {
                //ok, this is just an optional feature
            }

            for(auto & att:ar.getAttribMap()){
                //check for string(text) attributes
                if(att.first == "title" or att.first == "comment"){
                    ar.setAttr(att.first,ar.template getAttribute<std::string>(att.first));
                }
            }

        }

        template<class Ar>
        void CFConventions::SetGroupAttr(Ar &ar) {
            ar.setAttr("history","created: "+core::currentTimetag());
            for(auto & att:ar.getAttribMap()){
                //check for string(text) attributes
                if(att.first == "title" or att.first == "comment"){
                    ar.setAttr(att.first,ar.template getAttribute<std::string>(att.first));
                }
            }

        }


        template<class Ar>
        void CFConventions::SetDataAttr(Ar &ar) {
            for(auto & att:ar.getAttribMap()){
                //check for string(text) attributes
                if(att.first == "long_name" or att.first == "units"
                or   att.first == "standard_name"
                or att.first == "coordinates"){
                    ar.setAttr(att.first,ar.template getAttribute<std::string>(att.first));
                }
            }

        }


    }


}
#endif //FROMMLE_CFCONVENTIONS_HPP
