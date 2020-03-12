/*! \file frommle.hpp
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
#include "core/Exceptions.hpp"
#include "core/typehash.hpp"
#include <memory>
#ifndef FROMMLE_FROMMLE_HPP
#define FROMMLE_FROMMLE_HPP


namespace frommle{
    namespace io {
        //we only need to forward declare Group here
        class Group;
    }

    namespace core{
        ///@brief Polymorphic base class which wraps all important frommle classes
    class Frommle:public std::enable_shared_from_this<Frommle> {
    public:
        Frommle() {};

        Frommle(std::string name, typehash th = typehash("Frommle_t")) : name_(name), type_(th) {}

        std::string name() const { return name_; }

        void setName(std::string name) { name_ = name; }


        virtual std::vector<size_t> shape() const {
            THROWNOTIMPLEMENTED("This frommle object provides no shape information");
        }

        virtual void save(io::Group &grp) const;

        virtual void load(io::Group &group);

        //directly load from another frommle shared pointer
        virtual void load(const std::shared_ptr<Frommle> &frptr) {
            THROWNOTIMPLEMENTED("This frommle object cannot be loaded directly from a shared_ptr<Frommle> please implement the virtual function void load(const std::shared_ptr<Frommle>&)");
        }
        //add 1D index_range and index_gen types here?
            typehash hash() const { return type_; }
            typehash & hash(){ return type_; }
            std::string hashstr()const{return std::string(hash());}

            template<class T=const Frommle>
            std::shared_ptr<T> getPtr()const{
                return std::static_pointer_cast<T>(shared_from_this());

            }

            template<class T=Frommle>
            std::shared_ptr<T> getPtr(){
                return std::static_pointer_cast<T>(shared_from_this());

            }

            bool operator == (Frommle & other)const{return type_==other.type_;}

            bool operator != (Frommle & other)const{return type_!=other.type_;}

//        virtual void createFrom(core::typehash hash)=0;
//        virtual void loadFrom(const std::shared_ptr<Frommle> & frptr)=0;
    protected:
            ///@brief sets up the current object from just a typehash. Overload this function when this functionality is needed
            virtual void rehash(){};
            void sethash(typehash th){type_=th;}
        private:
            std::string name_="Frommle";
            typehash type_{"Frommle_t"};
        };




    template<class T>
    void loadFromFrommlePtr(std::shared_ptr<Frommle>  in, std::shared_ptr<Frommle> out){
        auto inptr=std::dynamic_pointer_cast<T>(in);
        auto outptr=std::dynamic_pointer_cast<T>(out);
        *outptr=*inptr;
    }

//    template<class T>
//    class FrommleExtend: public Frommle{
//    public:
//        FrommleExtend():Frommle(){}
//        FrommleExtend(std::string name,typehash th=typehash("Frommle_t")):Frommle(name,th){}
//
//        void createFrom(const std::shared_ptr<Frommle> &frptr) override{
//            auto inPtr=std::dynamic_pointer_cast<T>(frptr);
//            T* thisPtr=dynamic_cast<T*>(this);
//            *thisPtr=*inPtr;
//        }
//
//        void createFrom(core::typehash hash) override{
//            auto out=std::make_shared<T>(hash);
//            *this=*out;
//        }
//    };
//
//


    }
}

#endif //FROMMLE_FROMMLE_HPP
