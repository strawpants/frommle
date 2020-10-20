/*! \file FrommleBindings.cpp
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
#include <boost/python.hpp>
#include <boost/core/noncopyable.hpp>
#include "core/frommle.hpp"
#include "io/Group.hpp"


namespace p = boost::python;
namespace frommle {
//    namespace io{
//        class Group;
//    }

    namespace core {

    class FrommleWrapper:public Frommle,public p::wrapper<Frommle>{
    public:
        FrommleWrapper(std::string name="frommle"):Frommle(name){}
        core::typehash hash()const override{
            return core::typehash("pyfrommle_t");
        }
        void save(io::Group & grp)const{
            if(auto saveop=this->get_override("save")){
                saveop(grp);

            }else{
                Frommle::save(grp);

            }

        }

        void save_default(io::Group & grp)const{
            Frommle::save(grp);

        }

        void load(io::Group & grp){
            if(auto loadop=this->get_override("load")){
                loadop(grp);

            }else{
                Frommle::load(grp);

            }

        }

        void load_default(io::Group & grp){
            Frommle::load(grp);

        }

    };


        void (Frommle::*loadf)(io::Group &g)=&Frommle::load;

        void registerFrommle() {

            // we need to register the typehash which is closely linked to the Frommle class
            p::class_<typehash>("typehash").def(p::init<std::string>())
                    .def("__str__",&typehash::operator std::string )
                    .def("add",&typehash::add);


            p::class_<FrommleWrapper,boost::noncopyable>("Frommle",p::init<p::optional<std::string>>())
                    .add_property("name", &Frommle::name, &Frommle::setName)
                    .def("save",&Frommle::save,&FrommleWrapper::save_default)
                    .def("load",loadf,&FrommleWrapper::load_default)
                    .add_property<typehash(Frommle::*)()const>("hash",&Frommle::hash)
                    .def("__str__",&Frommle::hashstr);

            p::register_ptr_to_python< std::shared_ptr<Frommle> >();
            p::register_ptr_to_python< std::shared_ptr<const Frommle> >();
//            p::class_<Frommle>("Frommle").def("name", &Frommle::name)
//                    .add_property("name", &Frommle::name, &Frommle::setName);

        }


    }
}
