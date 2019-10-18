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
#include "GuidePack.hpp"

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
//            virtual guides::GuidePackBasePtr gp()const=0;
        private:
            std::string name_="data";

        };



        template<class T,int n>
        class GArrayDyn:public GArrayBase{
            public:
            using gp_t=guides::GuidePackDyn<n>;
            using gp_tptr=std::shared_ptr<guides::GuidePackDyn<n>>;
            //template<class ... Guides>
            //GArrayDyn(Guides && ... Args):gp_(std::make_shared<gp_t>(std::move(Args)...)),
                //data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
                //ar_(data_.get(),gp_->extent()){}

             ///@brief only allow this constructor when we  are considering complete guidepacks with the correct dimensions as input arguments
            template<class GP, typename std::enable_if< std::is_base_of<guides::GuidePackDyn<n>,GP>::value_type,int>::type =0 >
            GArrayDyn(GP && guidepack):gp_(std::make_shared<GP>(std::move(guidepack))),
                data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
                ar_(data_.get(),gp_->extent()){}

            GArrayDyn(const guides::GuidePackDyn<n> & gp):gp_(std::make_shared<guides::GuidePackDyn<n>>(gp)),
                    data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
                    ar_(data_.get(),gp_->extent()){

            }
            //note although empty, we always need to construct the multi_array_ref using a non-default constructor
            GArrayDyn():gp_(std::make_shared<gp_t>()),ar_(data_.get(),gp_->extent()){}
            static const int ndim=n;
            using GArrayBase::name;
            using GArrayBase::setName;
            using arr=boost::multi_array_ref<T,ndim>;
            using eigm=typename Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> ,Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>>;
            inline arr & mat(){return ar_;}
            inline const arr & mat()const{return ar_;}
            const gp_tptr & gp()const {return gp_;}
            gp_tptr & gp(){return gp_;}
            //Extract eigen matrix (but only for 2D arrays)
            template<int dim=ndim>
            typename std::enable_if<dim==2,eigm>::type  eig(){
                auto marrStrd=Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(ar_.strides()[0],ar_.strides()[1]);
                return eigm(ar_.data(),ar_.shape()[0],ar_.shape()[1],marrStrd);
            }

        GArrayDyn & operator=(const T scalar) {
            std::fill(ar_.data(),ar_.data()+ar_.num_elements(),scalar);
            return *this;
        }
            private:
            gp_tptr gp_{};
            std::shared_ptr<T[]> data_{};
            protected:
            arr ar_{{}};
        };



        ///general template declaration (the specialization below allows us to separate out the Guides using template magic)
        template<class T, class ... Guides>
    class GArray{};


        /*!brief
         * This is essentially a wrapper around boost multiarray class with guides
         * @tparam T element type of the multiarray
         * @tparam Guides a variadic set of Guides
         */
    template<class T, class ... Guides>
    class GArray<T,guides::GuidePack<Guides...>>:public GArrayDyn<T,sizeof ...(Guides)>{
        public:
            using GPack=guides::GuidePack<Guides...>;
            using GAdyn=GArrayDyn<T,sizeof ... (Guides)>;
            using GAdyn::ndim;
            using GAdyn::name;
            using GAdyn::arr;
            using GAdyn::ar_;
            //note this shadows
            inline const std::shared_ptr<GPack> gp()const{return std::static_pointer_cast<GPack>(GAdyn::gp());}

            ///allows the extraction of the guide types during compile time
            template<int n>
            using g_t=typename GPack::template g_t<n>;

            template<int n>
            using gptr_t=typename GPack::template gptr_t<n>;

            template<int n>
            inline gptr_t<n>  g(){
                return gp()->template g<n>();
            }

            template<int n>
            inline const gptr_t<n> g()const{
                return gp()->template g<n>();
            }
//
//            ///@brief polymorphic version
//            std::shared_ptr<const guides::GuideBase>  g(const int n)const;
//            std::shared_ptr<guides::GuideBase>  g(const int n);

            ///structors
            GArray():GAdyn(){

            }


            GArray(GPack && gp):GAdyn(std::move(gp)){
            }

            GArray(Guides && ... guides):GAdyn(GPack(std::move(guides)...)){}

            //indexing
            template<int i=0>
            typename std::enable_if< i+1 == ndim, T >::type & operator[](const typename g_t<i>::Element & indx){
                assert(1==ndim);
                return ar_[g<0>()->idx(indx)];
            }
//
//            // return a subview of the current GArray (i.e. strip a dimension)
//            using subGArray=typename arr::template array_view<ndim-1>::type;
//            template<int i=0>
//            typename std::enable_if< i+1 != ndim, subGArray >::type & operator[](const typename g_t<0>::element_type::Element & indx){
//                assert(i<ndim-1);
//                return ar_[g<0>()->idx(indx)];
//            }
//
//
//            ///@brief fill the matrix with a scalar value
//            GArray & operator=(const T scalar);


            
//            template<int ... n>
//            typename GArrayView<typename GPack::template maskpack<n>::type> maskdims();
            //{
                //using garview=typename GArrayView<GArray<T,typename GPack::template maskpack<n>::type>>;
                //garview mask{*this};
                //mask.gp_=gp_;
                //mask.data_=data_;
                //using range=arr::index_range;
                ////create a multi_array_view referencing the same memory
                //mask.arv_=ar_[boost::indices[range()][range()];
                //return mask;
            //}

//            template<class subGp>
//                class GArrayView{
//                    public:
//                        using ndim=subGp::ndim;
//                        //type describing a view on the underlying multi_array_ref (possibly with reduced dimensions)
//                        using arrv=GArray::arr::array_view<ndim>;
//                        template<class Gp>
//                        GArrayView(const GArray & garin, Gp & gpin):garr_(garin){
//                        //assign (possibly squeeze out degenerate dimensions
//                        gp_=gpin.squeeze();
//                        arv_=garin.ar_[gpin.indices()];
//
//                        }
//                    private:
//                        subGp=gp_{};
//                        arrv arv_={};
//                        //holds a pointer to the underlying GArray
//                        GArray * garr_=nullptr;
//                };

        protected:
        private:
            friend class io::serialize;
//            template<class Y,class Gp>
//            friend class GArray;

            template<class Archive>
            void load(Archive & Ar);
            template<class Archive>
            void save(Archive & Ar)const;

        
        };


        template<class T,int n>
        GArrayDyn<T,n> GA_ones(const guides::GuidePackDyn<n> & gpin){
            GArrayDyn<T,n> gout(gpin);
            gout=1;
            return gout;

        }


//        ///@brief a wrapper class which provides 'view access' (i.e. subarray with fewer dimensions, permuted or masked) to another GArray
//        template<class GA>
//        class GArrayView:public GArrayBase{
//        public:
//            using ndim=GA::ndim;
//            using arrv=typename GA::arr::template array_view<ndim>::type;
//            GArrayView(const GA & gar){
//            }
//        private:
//            arrv arv_{};
//            GA * garptr_=nullptr;
//
//        };

        ///@brief a guided array which also screens off invalid elements
//        template<class T, class GPack>
//        class MaskedGArray:public GArray<T,GPack>{
//            public:
//            using GArray<T,GPack>::ndim;
//            using mask_ar=boost::multi_array<bool,ndim>;
//            private:
//            mask_ar mask_{};
//
//        };

//            template<class T,class ... Guides>
//            template<int i>
//            typename GArray<T,Guides>::GArrayView<typename GArray<T,Guides>>::GPack::template maskpack<n>::type> maskdims()
//            {
//                using garview=typename GArray<T,Guides>::GArrayView<typename GPack::template maskpack<n>::type>;
//                return garview(*this,this->gp_.mask<i>());
//            }


        //templatei function implementations for GArray


//        template<class T, class ... Guides>
//        template<int n>
//        const typename GArray<T, Guides...>::template g_t <n> &GArray<T, Guides ...>::g() const {return Ggp_.template g<n>();}
//
//        template<class T, class GPack>
//        template<int n>
//        typename GArray<T,GPack>::template g_t <n> &GArray<T, GPack>::g() {return gp_.template g<n>();}
//
//        template<class T, class GPack>
//        std::shared_ptr<const guides::GuideBase> GArray<T, GPack>::g(const int n) const {return gp_.g(n);}
//
//        template<class T, class GPack>
//        std::shared_ptr<guides::GuideBase> GArray<T, GPack>::g(const int n) {return gp_.g(n);}
//
////        template<class T, class GPack>
////        template<int... n>
////        GArray::GArrayView<typename GPack::template maskpack<n>::type> GArray<T, GPack>::maskdims() {}
//
//        template<class T, class GPack>
//        GArray<T,GPack> &GArray<T, GPack>::operator=(const T scalar) {
//            std::fill(ar_.data(),ar_.data()+ar_.num_elements(),scalar);
//            return *this;
//        }
//
//        template<class T, class GPack>
//        template<class Archive>
//        void GArray<T, GPack>::save(Archive &Ar) const {
//            //save the Guidepack
//            Ar << gp_;
//
//
//            //save the multiarray data
//            auto & mvar=Ar.template getVariable<T>(name());
//
//            //link the coordinate variables in the attributes
//            std::string coord=g(0)->name();
//            for (auto i=1; i< ndim;++i){
//                coord+=" "+g(i)->name();
//            }
//            mvar.setAttribute("coordinates",coord);
//            mvar.setValue(core::Hyperslab<T>(ar_));
//
//        }
//
//        template<class T, class GPack>
//        template<class Archive>
//        void GArray<T, GPack>::load(Archive &Ar) {
//            //first load the matrix variable
//            auto & mvar=Ar.template getVariable<T>(name());
//
//            //set the appropriate names for the guidepacks
//            gp_.setNames(mvar.template getAttribute<std::string>("coordinates"));
//            //then load the appropriate guides
//
//            Ar >> gp_;
//
//
//            //also load the matrix data
//            core::Hyperslab<T> hslab{};
//            mvar.getValue(hslab);
//            //
//
//        }
//
//
//        /*!brief
//         * Factory function to quickly create garrays from initialized Guides
//         * @tparam T
//         * @tparam Guides
//         * @param inpack
//         * @return
//         */

    //template<class T,class... Guides>
    //std::shared_ptr<GArrayDyn<T,sizeof...(Guides)>> make_garray(guides::GuidePackDyn<sizeof ...(Guides)> & inpack){
            //return GArray<T,Guides...>(guides::GuidePack<Guides...>(std::forward<Guides>(std::move(inpack))...));
    //}




        }
    }
#endif /* SRC_CPP_GARRAYBASE_HPP_*/
