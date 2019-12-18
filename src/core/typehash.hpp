/*! \file typehash.hpp
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

#include <string>
#include <ostream>
#include <vector>
#include <boost/algorithm/string.hpp>

#ifndef FROMMLE_TYPEHASH_HPP
#define FROMMLE_TYPEHASH_HPP
namespace frommle{
    namespace core{

    ///@brief a structure which holds a hash
    class typehash{
    public:
        typehash(){}
        typehash(const std::string base):hash_(base){}
        //append an integer to the hash
        typehash add(const int & id){
            return typehash(hash_+","+std::to_string(id));
        }
        operator std::string ()const{return hash_;}
        const std::string & name()const{return hash_;}
        bool operator == (const typehash &other)const{return hash_==other.hash_;}

        bool operator != (const typehash &other)const{return not this->operator==(other);}
        std::ostream& write(std::ostream& os)const{
            return os << hash_;
        }
        std::vector<std::string> split()const{
            std::vector<std::string> vec;
            boost::split(vec,hash_,boost::is_any_of(","));
            return vec;
        }

    private:
        std::string hash_{};
    };

////free function to allow streaming the typehas to a stream
std::ostream &operator<<(std::ostream &os, typehash const &m);

    }
}




#endif //FROMMLE_TYPEHASH_HPP
