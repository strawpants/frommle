/*! \file
 \brief Holds the base for the Dimension class
 \copyright Roelof Rietbroek 2018
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

#ifndef SRC_CPP_DIMENSIONBASE_HPP_
#define SRC_CPP_DIMENSIONBASE_HPP_

#include <string>
#include <vector>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <memory>
namespace frommle {
namespace core {

/*!
 * \brief Abstract base class for describing  guided dimensions
 * The general idea behind the Guide classes is that they are used to add value to the dimension of an array/vector
 * Derived classes are expected to provide the following functionality:
 * -# Encapsulate functions (notably index functions) which are specific to the dimension
 * -# Allow serialization to read from/write to archives and other processes
 * -# Keep track of permutations so that an underlying linked array may be permuted but the indexing stays the same
 * -# Provide ways to compare and permute with other Dimensions so that they can be sorted relative to each other
 * -# Can be used as template arguments in order to select the correct functions at compile time (e.g. an observation equation)
 * Please see the class \ref TimeGuide or \ref IndexGuide for an example of a derived class
 * This abstract base class makes sure that the pure virtual functions are implemented and that
 * containers can store (smart) pointers to DimensionBase
 */
    class GuideBase {
    public:
        using index=size_t;

        GuideBase() {};

        GuideBase(const std::string &type, const index &sz) : type_(type), size_(sz) {}

        GuideBase(const std::string &type) : type_(type) {}

        virtual ~GuideBase() {
        }


        //add 1D index_range and index_gen types here?
        virtual std::string type() const { return type_; }

        index size() const { return size_; }

        virtual bool operator==(const GuideBase &in) const {
            return type_ == in.type_;
        }

    private:
        bool isPermuted_ = false;
        std::vector<index> permute_ = {};
    protected:
        std::string type_ = "FROMMLE";
        index size_ = 0;
    };

    //!@brief templated constant forward iterator class for use in Guides
    template<class Element>
    class Gconst_iterator: public boost::iterator_facade<Gconst_iterator<Element>,Element const,boost::forward_traversal_tag>{
    public:
        ~Gconst_iterator(){}
        Gconst_iterator(){};
    protected:
        friend class boost::iterator_core_access;
        //note that the default constructed Element corresponds to the 'end' of the iteration
        std::unique_ptr<Element> currentEl_={};
        virtual void increment()=0;
        const Element & dereference()const{return *currentEl_;}
        bool equal(const Gconst_iterator & comp)const{return *currentEl_ == *(comp.currentEl_);}

    };

    template <class Element>
    class GuideGen:public GuideBase{
    public:
        virtual Element operator[](const index idx)const =0;
        virtual Element & operator[](const index idx)=0;
        virtual index idx(const Element & in)const=0;
        Gconst_iterator<Element> begin()const=0;
        Gconst_iterator<Element> end()const=0;
    protected:
    private:

    };

}
}

#endif /* SRC_CPP_DIMENSIONBASE_HPP_*/