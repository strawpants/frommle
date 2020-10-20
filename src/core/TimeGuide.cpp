/*! \file
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
 */
#include "core/TimeGuide.hpp"

namespace frommle{
	namespace guides{
        DateGuide monthlyRange(const int yearstart, const int monthstart,const int yearend,const int monthend){
            gregdate mstart(yearstart,monthstart,1);   
            gregdate tmpdate=mstart+(mstart.end_of_month()-mstart)/2.0;
            gregdate dend=gregdate(yearend,monthend,1).end_of_month();   
            DateGuide outguide{};
            while ( tmpdate < dend){
                outguide.push_back(tmpdate);
                
                if ( mstart.month() ==12){
                    mstart=gregdate(mstart.year()+1,1,1);
                }else{
                
                    mstart=gregdate(mstart.year(),mstart.month()+1,1);
                }
                     
                tmpdate=mstart+(mstart.end_of_month()-mstart)/2.0;
            }
            
            return outguide;
        }
	}
}
