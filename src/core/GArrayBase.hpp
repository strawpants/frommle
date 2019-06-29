/*! \file
 \brief Holds the base for the templated guided array class
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

#ifndef SRC_CPP_GARRAYBASE_HPP_
#define SRC_CPP_GARRAYBASE_HPP_

#include <string>
#include <boost/multi_array.hpp>
#include <tuple>
#include "core/GuidePack.hpp"
#include <eigen3/Eigen/Core>
#include <type_traits>
#include "io/Group.hpp"
namespace frommle {
    namespace core {


/*!
 * \brief Abstract base class for storing data and the corresponding dimensions
 * This class support the storing of multidimensional data in memory, whose dimensions are linked to Dimension objects
 * Furthermore, a unitbase can be assigned to the array
 */
        class GArrayBase {
        public:
            virtual ~GArrayBase() {
            }
            std::string name()const{return name_;}
            void setName(const std::string & name){
                name_=name;
            }
        private:
            std::string name_="data";

        };

        /*!brief
         * This is essentially a wrapper around boost multiarray class with guides
         * @tparam T element type of the multiarray
         * @tparam Guides a variadic set of Guides
         */
        template<class T, class GPack>
        class Garray:public GArrayBase{
        public:
//            using GPack=GuidePack<Guides...>;
//            static const int ndim=sizeof...(Guides);
            static const int ndim=GPack::ndim;
            template<int n>
            using g_t=typename GPack::template g_t<n>;

            template<int n>
            inline g_t<n> & g(){return gp_.template g<n>();}

            template<int n>
            inline const g_t<n> & g()const{return gp_.template g<n>();}

            ///@brief polymorphic version
            inline const std::shared_ptr<GuideBase>  g(const int n)const{return gp_.g(n);}
            inline std::shared_ptr<GuideBase>  g(const int n){return gp_.g(n);}

            using arr=boost::multi_array_ref<T,ndim>;
            inline arr & mat(){return ar_;}
            Garray():gp_(),data_(std::shared_ptr<T[]>(new T[gp_.num_elements()])),ar_(data_.get(),gp_.getExtent()){}


            Garray(GPack && gp):gp_(std::move(gp)),data_(std::shared_ptr<T[]>(new T[gp_.num_elements()])),ar_(data_.get(),gp_.getExtent()){
            }

//            //return a single value of the internal multi_array_ref
            template<int i=0>
            typename std::enable_if< i+1 == ndim, T >::type & operator[](const typename g_t<0>::element_type::Element & indx){
                assert(1==ndim);
                return ar_[g<0>()->idx(indx)];
            }

            // return a subview of the current Garray (i.e. strip a dimension)
            using subGarray=typename arr::template array_view<ndim-1>::type;
            template<int i=0>
            typename std::enable_if< i+1 != ndim, subGarray >::type & operator[](const typename g_t<0>::element_type::Element & indx){
                assert(i<ndim-1);
                return ar_[g<0>()->idx(indx)];
            }
            Garray & operator=(const T scalar){
                std::fill(ar_.data(),ar_.data()+ar_.num_elements(),scalar);
            }

            //Extract eigen matrix (but only for 2D arrays)
            using eigm=typename Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> ,Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>>;
            template<int dim=ndim>
            typename std::enable_if<dim==2,eigm>::type  eig(){
                auto marrStrd=Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(ar_.strides()[0],ar_.strides()[1]);
                return eigm(ar_.data(),ar_.shape()[0],ar_.shape()[1],marrStrd);
            }

            template<int n>
            typename core::Garray<T,typename GPack::template maskpack<n>::type> maskdim(){
                typename core::Garray<T,typename GPack::template maskpack<n>::type> mask{};
                mask.gp_=gp_;
                mask.data_=data_;
                mask.ar_=ar_;
                return mask;
            }


        protected:
        private:
            friend class io::serialize;
            template<class Y,class Gp>
            friend class Garray;

            GPack gp_{};
            std::shared_ptr<T[]> data_{};
            arr ar_;
            template<class Archive>
            void load(Archive & Ar){
                //first load the matrix variable
                auto & mvar=Ar.template getVariable<T>(name());

                //set the appropriate names for the guidepacks
               gp_.setNames(mvar.template getAttribute<std::string>("coordinates"));
                //then load the appropriate guides

                Ar >> gp_;


                //also load the matrix data
                core::Hyperslab<T> hslab{};
                mvar.getValue(hslab);
                //

            }
            template<class Archive>
            void save(Archive & Ar)const{
                //save the Guidepack
                Ar << gp_;


                //save the multiarray data
                auto & mvar=Ar.template getVariable<T>(name());

                //link the coordinate variables in the attributes
                std::string coord=g(0).name();
                for (auto i=1; i< ndim;++i){
                    coord+=" "+g(i).name();
                }
                mvar.setAttribute("coordinates",coord);
                mvar.setValue(core::Hyperslab<T>(ar_));

            }

        
        };


        ///@brief a guided array which also screens off invalid elements
        template<class T, class GPack>
        class MaskedGarray:public Garray<T,GPack>{
            public:
            using Garray<T,GPack>::ndim;
            using mask_ar=boost::multi_array<bool,ndim>;
            private:
            mask_ar mask_{};
            
        };

        /*!brief
         * Factory function to quickly create garrays from initialized Guides
         * @tparam T
         * @tparam Guides
         * @param inpack
         * @return
         */
        template<class T=double,class... Guides>
        Garray<T,GuidePack<Guides...>> make_garray(Guides && ... inpack){
            return Garray<T,GuidePack<Guides...>>(GuidePack<Guides...>(std::forward<Guides>(std::move(inpack))...));
        }





        }
    }
#endif /* SRC_CPP_GARRAYBASE_HPP_*/
