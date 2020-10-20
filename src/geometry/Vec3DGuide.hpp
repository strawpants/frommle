/*! \file
 \brief contains a guide which holds a single 3 D vector
 \copyright Roelof Rietbroek 2020
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

#include "core/Exceptions.hpp"
#include "core/GuideBase.hpp"
#include <array>
#include <io/Group.hpp>
#include "io/Variable.hpp"
#include "core/Logging.hpp"

#ifndef FROMMLE_VEC3DGUIDE_HPP
#define FROMMLE_VEC3DGUIDE_HPP

namespace frommle{
    namespace guides{

        enum xyz{
        x=0,y=1,z=2
        };

        class Vec3DGuide:public GuideBase{
        public:
            using Element=xyz;
            using ElementContainer=std::array<xyz,3>;
            //structors
            Vec3DGuide():GuideBase("XYZGuide"){}
            Vec3DGuide(const std::string & name):GuideBase(name){}
            Vec3DGuide(const Vec3DGuide & in)=default;
            typehash hash()const override{return typehash("Vec3d_t");}

            using const_iterator=ElementContainer::const_iterator;
            using iterator=typename ElementContainer::iterator;
            
            const_iterator begin() const { return comp_.cbegin(); }
            const_iterator end() const { return comp_.cend(); }

            //iterator begin() { return comp_.begin(); }
            //iterator end() { return comp_.end(); }

            //Element & operator[](const size_t idx){return comp_[idx];}
            const Element & operator[](const size_t idx)const{return comp_[idx];}
            size_t idx(const Element & cart)const{
                
                if (cart == comp_[0]){
                    return 0;
                }else if (cart == comp_[1]){
                    return 1;
                }else if (cart == comp_[2]){
                    return 2;
                }
                THROWINPUTEXCEPTION("Cannot find the cartesian element in Vec3DGuide, something is messed up");
            }
            //std::vector<std::string> descriptor()const override{
                //return std::vector<std::string>{"X","Y","Z"}; 
            //}
            //void load(io::Group & Ar);
            //void save(io::Group & Ar)const;
            size_t size()const override{return 3;}
        private:
            std::array<xyz,3> comp_={xyz::x,xyz::y,xyz::z};
        };

    }
}


#endif //FROMMLE_VEC3DGUIDE_HPP
