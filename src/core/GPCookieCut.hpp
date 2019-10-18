/*! \file GPCookieCut.hpp
 \brief Contains tools to reshape Guidepacks (both at compile time and runtime)
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
#ifndef FROMMLE_GPCOOKIECUT_HPP
#define FROMMLE_GPCOOKIECUT_HPP

namespace frommle{
    namespace guides{

        template<int n>
        struct GPCookieCutDyn{

        };

        ///@brief general template declaration
        template<class ... T>
                struct GPCookieCut;

        ///@brief template specialization which allows splitting up the input guide from GuidePack
        template<class ... Guides>
        struct GPCookieCut<GuidePack<Guides...> > : public GPCookieCutDyn<sizeof ...(Guides)>{
//            using gpbase=GuidePack<Guides...>;

            template<class otherG>
            using gapp_t=GuidePack<Guides...,otherG>;

            template<class ... OtherGuides>
                    struct with{};
            template<class ... OtherGuides>
                    struct with<GuidePack<OtherGuides...>>{
                        using rmerge_t=GuidePack<Guides...,OtherGuides...>;
                    };

        };

        template<class LastG, class ... Guides>
        struct GPstrip{};

        template<class LastG, class ... Guides>
        struct GPstrip<GuidePack<Guides...,LastG>>{
            using gps=GuidePack<Guides...>;
            using gpin=GuidePack<Guides...,LastG>;
            static gps strip(const gpin & in){
                    GuidePack<Guides...> gpout();
                    auto frst=in.begin();
                    std::copy(frst,frst+gps::ndim,gpout.begin());

                    return gpout;
            }
        };



    }
}




#endif //FROMMLE_GPCOOKIECUT_HPP
