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
            inline g_t<n> & g();

            template<int n>
            inline const g_t<n> & g()const;

            ///@brief polymorphic version
            std::shared_ptr<const GuideBase>  g(const int n)const;
            std::shared_ptr<GuideBase>  g(const int n);

            using arr=boost::multi_array_ref<T,ndim>;
            inline arr & mat(){return ar_;}
            ///structors
            Garray():gp_(),data_(std::shared_ptr<T[]>(new T[gp_.num_elements()])),ar_(data_.get(),gp_.getExtent()){

            }


            Garray(GPack && gp):gp_(std::move(gp)),data_(std::shared_ptr<T[]>(new T[gp_.num_elements()])),ar_(data_.get(),gp_.getExtent()){
            }

            //indexing
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


            ///@brief fill the matrix with a scalar value
            Garray & operator=(const T scalar);

            //Extract eigen matrix (but only for 2D arrays)
            using eigm=typename Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> ,Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>>;
            template<int dim=ndim>
            typename std::enable_if<dim==2,eigm>::type  eig(){
                auto marrStrd=Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(ar_.strides()[0],ar_.strides()[1]);
                return eigm(ar_.data(),ar_.shape()[0],ar_.shape()[1],marrStrd);
            }

            
//            template<int ... n>
//            typename GarrayView<typename GPack::template maskpack<n>::type> maskdims();
            //{
                //using garview=typename GarrayView<Garray<T,typename GPack::template maskpack<n>::type>>;
                //garview mask{*this};
                //mask.gp_=gp_;
                //mask.data_=data_;
                //using range=arr::index_range;
                ////create a multi_array_view referencing the same memory
                //mask.arv_=ar_[boost::indices[range()][range()];
                //return mask;
            //}

//            template<class subGp>
//                class GarrayView{
//                    public:
//                        using ndim=subGp::ndim;
//                        //type describing a view on the underlying multi_array_ref (possibly with reduced dimensions)
//                        using arrv=Garray::arr::array_view<ndim>;
//                        template<class Gp>
//                        GarrayView(const Garray & garin, Gp & gpin):garr_(garin){
//                        //assign (possibly squeeze out degenerate dimensions
//                        gp_=gpin.squeeze();
//                        arv_=garin.ar_[gpin.indices()];
//
//                        }
//                    private:
//                        subGp=gp_{};
//                        arrv arv_={};
//                        //holds a pointer to the underlying Garray
//                        Garray * garr_=nullptr;
//                };

        protected:
        private:
            friend class io::serialize;
            template<class Y,class Gp>
            friend class Garray;

            GPack gp_{};
            std::shared_ptr<T[]> data_{};
            arr ar_;

            static std::array<size_t,ndim> getExtent(arr & in){
                std::array<size_t,ndim> out{};
                std::copy(in.shape(), in.shape()+out.size(), out.begin());
                return out;

            }

            template<class Archive>
            void load(Archive & Ar);
            template<class Archive>
            void save(Archive & Ar)const;

        
        };


//        ///@brief a wrapper class which provides 'view access' (i.e. subarray with fewer dimensions, permuted or masked) to another Garray
//        template<class GA>
//        class GarrayView:public GArrayBase{
//        public:
//            using ndim=GA::ndim;
//            using arrv=typename GA::arr::template array_view<ndim>::type;
//            GarrayView(const GA & gar){
//            }
//        private:
//            arrv arv_{};
//            GA * garptr_=nullptr;
//
//        };

        ///@brief a guided array which also screens off invalid elements
//        template<class T, class GPack>
//        class MaskedGarray:public Garray<T,GPack>{
//            public:
//            using Garray<T,GPack>::ndim;
//            using mask_ar=boost::multi_array<bool,ndim>;
//            private:
//            mask_ar mask_{};
//
//        };

//            template<class T,class ... Guides>
//            template<int i>
//            typename Garray<T,Guides>::GarrayView<typename Garray<T,Guides>>::GPack::template maskpack<n>::type> maskdims()
//            {
//                using garview=typename Garray<T,Guides>::GarrayView<typename GPack::template maskpack<n>::type>;
//                return garview(*this,this->gp_.mask<i>());
//            }


        //templatei function implementations for Garray


        template<class T, class GPack>
        template<int n>
        const typename Garray<T, GPack>::template g_t <n> &core::Garray<T, GPack>::g() const {return gp_.template g<n>();}

        template<class T, class GPack>
        template<int n>
        typename Garray<T,GPack>::template g_t <n> &Garray<T, GPack>::g() {return gp_.template g<n>();}

        template<class T, class GPack>
        std::shared_ptr<const GuideBase> Garray<T, GPack>::g(const int n) const {return gp_.g(n);}

        template<class T, class GPack>
        std::shared_ptr<GuideBase> Garray<T, GPack>::g(const int n) {return gp_.g(n);}

//        template<class T, class GPack>
//        template<int... n>
//        Garray::GarrayView<typename GPack::template maskpack<n>::type> Garray<T, GPack>::maskdims() {}

        template<class T, class GPack>
        Garray<T,GPack> &Garray<T, GPack>::operator=(const T scalar) {
            std::fill(ar_.data(),ar_.data()+ar_.num_elements(),scalar);
            return *this;
        }

        template<class T, class GPack>
        template<class Archive>
        void Garray<T, GPack>::save(Archive &Ar) const {
            //save the Guidepack
            Ar << gp_;


            //save the multiarray data
            auto & mvar=Ar.template getVariable<T>(name());

            //link the coordinate variables in the attributes
            std::string coord=g(0)->name();
            for (auto i=1; i< ndim;++i){
                coord+=" "+g(i)->name();
            }
            mvar.setAttribute("coordinates",coord);
            mvar.setValue(core::Hyperslab<T>(ar_));

        }

        template<class T, class GPack>
        template<class Archive>
        void Garray<T, GPack>::load(Archive &Ar) {
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
