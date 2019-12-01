/*! \file Attributes.hpp
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
#include <boost/any.hpp>
#include <map>


#ifndef FROMMLE_ATTRIBUTES_HPP
#define FROMMLE_ATTRIBUTES_HPP

namespace frommle{
    namespace core{

        ///@brief a wrapper class which stores arbitrary objects in an internal std::map
        class Attributes{
        public:
            using atmap=std::map<std::string,boost::any>;
            Attributes(){}
            Attributes(atmap attr):attrib_(attr){}

            template<class Value>
            void set(const std::string &name, const Value &val){
                attrib_[name]=boost::any(val);
            }

            size_t getCount(const std::string &name)const{return attrib_.count(name);}
            size_t size()const{return attrib_.size();}
            atmap & map(){return attrib_;}
            const atmap & map()const{return attrib_;}


            ///@brief standard way to retrieve an attribute
            template<class Value>
            typename std::enable_if<!std::is_same<Value,std::string>::value,Value>::type get(const std::string &name){
                if(! contains(name)){
                    THROWINPUTEXCEPTION("key not found in Attributes");
                }
                return boost::any_cast<Value>(attrib_[name]);
            }

            ///@brief special treatment of std::string attribute retrieval (accepts both char * and string )
            template<class Value>
            typename std::enable_if<std::is_same<Value,std::string>::value,Value>::type get(const std::string &name){
                if(! contains(name)){
                    THROWINPUTEXCEPTION("key not found in Attributes");
                }
                
                try{
                    return boost::any_cast<std::string>(attrib_[name]);
                }catch(boost::bad_any_cast & excep){
                    return std::string(boost::any_cast<char const * >(attrib_[name]));
                }
            }

            bool contains(const std::string & name)const{
                return getCount(name) != 0;
            }




        private:
            std::map<std::string,boost::any> attrib_{};

        };


    }
}


#endif //FROMMLE_ATTRIBUTES_HPP
