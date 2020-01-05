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

#include <string>
#include <boost/multi_array.hpp>
#include <tuple>
#include "core/GuidePack.hpp"
#include <eigen3/Eigen/Core>
#include <type_traits>
#include "io/Group.hpp"
#include "core/frommle.hpp"
#include "core/Hyperslab.hpp"

#ifndef SRC_CPP_GARRAYBASE_HPP_
#define SRC_CPP_GARRAYBASE_HPP_

namespace frommle {
    namespace core {


        template<class T, int n, class Parent>
        class GSubArray {
        public:
        using subtype=GSubArray<T, n-1, Parent>;
        using arr=typename Parent::arr::template sub_array<n>;

        GSubArray(const Parent *par, arr ar) : parent_(par), sar_(ar) {

        }

        template<class Element>
        subtype operator[](const Element & el) {
            return subtype(parent_, sar_[parent_->gpp()->template idx<Parent::ndim - n -1>(el)]);
        };

        arr &mat() { return sar_; }

        const arr &mat() const { return sar_; }

        private:
        const Parent *parent_ = nullptr;
        arr sar_{};
    };

    //specialization needed to return s scaler when indexed
    template<class T, class Parent>
    class GSubArray<T, 1, Parent> {
    public:
    using arr=typename Parent::arr::template subarray<1>::type;

    GSubArray(const Parent *par, arr ar) : parent_(par), sar_(ar) {

    }

    template<class Element>
    T &operator[](const Element & el) {
        return sar_[parent_->gpp()->template idx<Parent::ndim-1>(el)];
    };

    inline arr &mat() { return sar_; }

    inline const arr &mat() const { return sar_; }

    private:
    const Parent *parent_ = nullptr;
    arr sar_{};
};

/*!
 * \brief Abstract base class for storing data and the corresponding dimensions
 * This class support the storing of multidimensional data in memory, whose dimensions are linked to Dimension objects
 * Furthermore, a unitbase can be assigned to the array
 */
//class GArrayBase:public Frommle {
//public:
//    virtual ~GArrayBase() {
//    }
//
//    std::string name() const { return name_; }
//
//    void setName(const std::string &name) {
//        name_ = name;
//    }
////            virtual guides::GuidePackBasePtr gp()const=0;
//private:
//    std::string name_ = "data";
//
//};


template<class T, int n>
class GArrayDyn : public Frommle {
public:
    using gp_t=guides::GuidePackDyn<n>;
    using gp_tptr=std::shared_ptr<guides::GuidePackDyn < n>>;
    using tvec=T[];
    //template<class ... Guides>
    //GArrayDyn(Guides && ... Args):gp_(std::make_shared<gp_t>(std::move(Args)...)),
    //data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
    //ar_(data_.get(),gp_->extent()){}

    ///@brief only allow this constructor when we  are considering complete guidepacks with the correct dimensions as input arguments
    template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<n>, GP>::value, int> ::type = 0>
    GArrayDyn(GP guidepack) : Frommle("GArray"),gp_(std::make_shared<GP>(std::move(guidepack))),
                              data_(std::shared_ptr<tvec>(new T[gp_->num_elements()])),
                              ar_(data_.get(), gp_->extent()) {}

    template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<n>, GP>::value, int> ::type = 0>
    GArrayDyn(GP guidepack,std::string name) : Frommle(name),gp_(std::make_shared<GP>(std::move(guidepack))),
                              data_(std::shared_ptr<tvec>(new T[gp_->num_elements()])),
                              ar_(data_.get(), gp_->extent()) {}
                              
    //specialized constructor which casts a generic GuidePackPtr to an appropritate GuidePackDyn
    GArrayDyn(const guides::GuidePackPtr & guidepack) :Frommle("GArray"),gp_(std::dynamic_pointer_cast<guides::GuidePackDyn<n>>(guidepack)),
                                  data_(std::shared_ptr<tvec>(new T[gp_->num_elements()])),
                                  ar_(data_.get(), gp_->extent()) {}

    ///@brief only allow this constructor when we  are considering complete guidepacks with the correct dimensions as input arguments
//            template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<n>, GP>::value_type, int>::type = 0>
//            GArrayDyn(const GP &guidepack):gp_(std::make_shared<GP>(guidepack)),
//                                     data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
//                                     ar_(data_.get(), gp_->extent()) {}

//            GArrayDyn(const guides::GuidePackDyn<n> &gp) : gp_(std::make_shared<guides::GuidePackDyn<n>>(gp)),
//                                                           data_(std::shared_ptr<T[]>(new T[gp_->num_elements()])),
//                                                           ar_(data_.get(), gp_->extent()) {
//
//            }

    //note although empty, we always need to construct the multi_array_ref using a non-default constructor
    GArrayDyn() :Frommle("GArray"),gp_(std::make_shared<gp_t>()), ar_(data_.get(), gp_->extent()) {}

    static const int ndim = n;
    using arr=boost::multi_array_ref<T, ndim>;
    static typename arr::index_gen mindices(){
        return typename arr::index_gen();
     }
    //perfect forwarding to the boost index_range
    template<typename ... I>
    static typename boost::multi_array_types::index_range mrange( I ... Args){
        return boost::multi_array_types::index_range(std::forward(Args)...);
    }

    using eigm=typename Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Aligned, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;

    using const_eigm=typename Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Aligned, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;

    inline arr &mat() { return ar_; }

    inline const arr &mat() const { return ar_; }

    const gp_t &gp() const { return *gp_; }

    gp_t &gp() { return *gp_; }

    const gp_tptr &gpp() const { return gp_; }

    gp_tptr &gpp() { return gp_; }

    //Extract eigen matrix (but only for 2D arrays)
    template<int dim = ndim>
    typename std::enable_if<dim == 2, eigm>::type eig() {
        auto marrStrd = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(ar_.strides()[1], ar_.strides()[0]);
        return eigm(ar_.data(), ar_.shape()[0], ar_.shape()[1], marrStrd);
    }

    template<int dim = ndim>
    typename std::enable_if<dim == 2, const_eigm>::type eig()const {
        auto marrStrd = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(ar_.strides()[1], ar_.strides()[0]);
        return const_eigm(ar_.data(), ar_.shape()[0], ar_.shape()[1], marrStrd);
    }

    GArrayDyn &operator=(const T scalar) {
        std::fill(ar_.data(), ar_.data() + ar_.num_elements(), scalar);
        return *this;
    }

    template<class GP>
    GArrayDyn<T, GP::ndim> reshape(const GP &gpin) const;

//            template<class Element>
//            size_t idx(const Element &)const{
//
//            }

    template<class Element, int nd = n, typename std::enable_if<nd == 1, int>::type = 0>
    T &operator[](const Element & el) {
//               LOGINFO << boost::apply_visitor(guides::gvar_idx<Element>(el),gp_->gv(0)) << std::endl;
        return ar_[gp_->idx(0, el)];

    }

        using subarr=GSubArray<T,n-1,GArrayDyn>;
           template<class Element,int nd=n,typename std::enable_if< nd != 1, int>::type =0>
           subarr operator[](const Element & el){
               return subarr(this,ar_[gp_->idx(0,el)]);
           }

//            template<class Element>
//            typename std::enable_if< n == 1, T>::type & operator[](const & Element el){
//
//
//            }

    void save(io::Group &ar)const{
        //save guidepack
        gp_->save(ar);

        //save matrix data
        auto mar=ar.getVariable(name());
        mar->setValue(HyperSlabConstRef<T>(mat()));
    }

    void load(io::Group &ar){
        //load guidepack
        gp_->load(ar);
        //allocate new memory so it fits the new guidepack
        realloc();
        auto mar = ar.getVariable(name());
        HyperSlab<T> hslab(mat(),data_);
        mar->getValue(hslab);
        //we may need to reassign the internal multi_array_ref so it uses the correct data
        reassign();

    }
private:
    template<class TO, int no>
    friend
    class GArrayDyn;
        ///@brief resize data and internal array to a possibly update guidepack
        void reset(const std::shared_ptr<tvec> & data){
            data_=data;
            reassign();
        }
        void realloc(){
            data_.reset(new T[gp_->num_elements()]);
            //also make sure the internal multi_arrary_ref uses this newly allocated data
            reassign();
        }
        ///@brief reassign internal multi_array_ref
        void reassign(){
            ar_.~arr();
            new(&ar_) arr(data_.get(), gp_->extent());
        }
    gp_tptr gp_{};
    std::shared_ptr<tvec> data_{};
protected:
    arr ar_{{}};
};


///@brief keep the same data but reshape according to new guidepack
template<class T, int n>
template<class GP>
GArrayDyn<T, GP::ndim> GArrayDyn<T, n>::reshape(const GP &gpin) const {
    if (gp_->num_elements() != gpin.num_elements()) {
        THROWINPUTEXCEPTION("reshape failed: guidepacks don't contain the same amount of elements");
    }
    //default constructor doesn't allocate (which is want we want)
    GArrayDyn<T, GP::ndim> gaout;
    //assign guides
    gaout.gp() = gpin;
    //assign data pointer
    gaout.data_ = data_;
    //create a renewed multi_array_ref (ugly hack with placement new)
    using arrnew=typename GArrayDyn<T, GP::ndim>::arr;
    gaout.ar_.~arrnew();
    new(&(gaout.ar_)) arrnew(gaout.data_.data(), gaout.gpp()->extent());

    return gaout;

}



//        //specialization where n==1
//        template<class T>
//        GSubArray<T,0>{
//        public:
//        using GarMom=GArrayDyn<T,norig>;
//
//        template<class Element>
//        T& operator[](Element el){
//
//        };
//    private:
//};



///general template declaration (the specialization below allows us to separate out the Guides using template magic)
template<class T, class ... Guides>
class GArray {
};


/*!brief
 * This is essentially a wrapper around boost multiarray class with guides
 * @tparam T element type of the multiarray
 * @tparam Guides a variadic set of Guides
 */
template<class T, class ... Guides>
class GArray<T, guides::GuidePack < Guides...>>

:
public GArrayDyn<T, sizeof ...(Guides)> {
public:
using GPack=guides::GuidePack<Guides...>;
using GAdyn=GArrayDyn<T, sizeof ... (Guides)>;
using GAdyn::ndim;
using GAdyn::name;
using GAdyn::arr;
using GAdyn::ar_;
using GAdyn::operator=;

//note this shadows
inline const GPack &gp() const { return *(std::static_pointer_cast<GPack>(GAdyn::gpp())); }

inline const std::shared_ptr<GPack> gpp() const { return std::static_pointer_cast<GPack>(GAdyn::gpp()); }
///allows the extraction of the guide types during compile time
template<int n>
using g_t=typename GPack::template g_t<n>;

template<int n>
using gptr_t=typename GPack::template gptr_t<n>;

template<int n>
inline gptr_t<n> g() {
    return gp().template g<n>();
}

template<int n>
inline const gptr_t<n> g() const {
    return gp().template g<n>();
}
//
//            ///@brief polymorphic version
//            std::shared_ptr<const guides::GuideBase>  g(const int n)const;
//            std::shared_ptr<guides::GuideBase>  g(const int n);

///structors
GArray() : GAdyn() {

}


//            GArray(const GPack &gp) : GAdyn(gp) {
//            }

GArray(GPack
gp) :
GAdyn(std::move(gp)
) {
}


//            GArray(Guides &&... guides) : GAdyn(GPack(std::forward<Guides>(guides)...)) {}
GArray(Guides
... guides) :
GAdyn(GPack(std::move(guides)...)
) {
}
            template<int nd = ndim, typename std::enable_if<nd == 1, int>::type = 0>
            T &operator[](const typename g_t<0>::Element & el) {
                return ar_[g<0>()->idx(el)];

            }
//
            using subarr=GSubArray<T,ndim-1,GArray>;
            template<int nd=ndim,typename std::enable_if< nd != 1, int>::type =0>
            subarr operator[](const typename g_t<0>::Element & el){
                return subarr(this,ar_[g<0>()->idx(el)]);
            }

//            GArray(const Guides &... guides) : GAdyn(GPack(guides...)) {}
//indexing
//template<int i = 0>
//typename std::enable_if<i + 1 == ndim, T>::type &operator[](const typename g_t<i>::Element &indx) {
//    assert(1 == ndim);
//    return ar_[g<0>()->idx(indx)];
//}
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
void load(Archive &Ar);

template<class Archive>
void save(Archive &Ar) const;


};

        template<class T>
        struct GAconstruct{
            
            template<int n>
            static GArrayDyn<T,n> ones(guides::GuidePackDyn<n> gpin){
                return fromGuidePack<n>::fill(1,gpin);
            }

            template<class ... Guides>
            static GArray<T,guides::GuidePack<Guides...>> ones(Guides ... guides){
                return fromGuides<Guides...>::fill(1,std::forward<Guides>(guides)...);
            }
            
            template<int n>
            static GArrayDyn<T,n> zeros(guides::GuidePackDyn<n> gpin){
                return fromGuidePack<n>::fill(0,gpin);
            }

            template<class ... Guides>
            static GArray<T,guides::GuidePack<Guides...>> zeros(Guides ... guides){
                return fromGuides<Guides...>::fill(0,std::forward<Guides>(guides)...);
            }


            template<int n>
            static GArrayDyn<T,n*2> eye(guides::GuidePackDyn<n> gpin){
                auto gout=fromGuidePack<n*2>::fill(0,*gpin.append(gpin));
                //fill diagonal using eigen
                gout.eig().diagonal().array()=static_cast<T>(1);
                return gout;
            }


            template<class ... Guides>
            struct fromGuides{

            static GArray<T, guides::GuidePack<Guides...>> fill(const T & fillvalue, Guides ... guides){

                GArray<T, guides::GuidePack<Guides...>> gout(std::move(guides)...);
                 gout = fillvalue;
            //LOGINFO << "matrix value" << gout.mat()[0][0] << std::endl;
                return gout;

            }
        
            };


            template<int n>
            struct fromGuidePack{

            static GArrayDyn<T,n> fill(const T & fillvalue, guides::GuidePackDyn<n> gpin){

                GArrayDyn<T, n> gout(std::move(gpin));
                 gout = fillvalue;
            //LOGINFO << "matrix value" << gout.mat()[0][0] << std::endl;
                return gout;

            }
        
            
            };

        };





        template<class ... Guides>
        GArray<double, guides::GuidePack<Guides...>>
        GA_ones_d(Guides
                  ... gpin) {
            GArray<double, guides::GuidePack<Guides...>>

                    gout(std::move(gpin)

                                 ...);
            gout = 1.0;
            return
                    gout;

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
