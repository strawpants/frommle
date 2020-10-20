/*! \file
 \brief 
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

#include "core/GuidePacktemplated.hpp"
#include "core/GArrayBase.hpp"


#ifndef FROMMLE_GARRAYDENSE_HPP
#define FROMMLE_GARRAYDENSE_HPP

namespace frommle {
    namespace core {
        ///@brief Sub array of a dense array
        template<class T, int n, class Parent>
        class GArrayDenseSub {
        public:
            using subtype=GArrayDenseSub<T, n - 1, Parent>;
            using arr=typename Parent::arr::template sub_array<n>;

            GArrayDenseSub(const Parent *par, arr ar) : parent_(par), sar_(ar) {

            }

            template<class Element>
            subtype operator[](const Element &el) {
                return subtype(parent_, sar_[parent_->gpp()->template idx<Parent::ndim - n - 1>(el)]);
            };

            arr &mat() { return sar_; }

            const arr &mat() const { return sar_; }

        private:
            const Parent *parent_ = nullptr;
            arr sar_{};
        };

        ///@brief specialization needed to return a scaler when indexed
        template<class T, class Parent>
        class GArrayDenseSub<T, 1, Parent> {
        public:
            using arr=typename Parent::arr::template subarray<1>::type;

            GArrayDenseSub(const Parent *par, arr ar) : parent_(par), sar_(ar) {

            }

            template<class Element>
            T &operator[](const Element &el) {
                return sar_[parent_->gpp()->template idx<Parent::ndim - 1>(el)];
            };

            inline arr &mat() { return sar_; }

            inline const arr &mat() const { return sar_; }

        private:
            const Parent *parent_ = nullptr;
            arr sar_{};
        };


        ///@brief holds a dense matrix
        template<class T, int n>
        class GArrayDense : public GArrayBase<T, n> {
        public:
            using GABase=GArrayBase<T, n>;
            using GABase::gpp;
            using GABase::gp_;
            using GABase::name;
            using typename GABase::gp_ptr_t;
            using tvec=T[];
            using arr=boost::multi_array_ref<T, n>;
            using eigm=typename Eigen::Map<Eigen::Matrix <
                                           T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Aligned, Eigen::Stride <Eigen::Dynamic, Eigen::Dynamic>>;
            using const_eigm=typename Eigen::Map<const Eigen::Matrix <T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Aligned,
                    Eigen::Stride < Eigen::Dynamic, Eigen::Dynamic>>;
            using subarr=GArrayDenseSub<T, n - 1, GArrayDense>;

            ///structors

            ///@brief the type traits remove this constructor when the input is not a guidepack
            template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<n>, GP>::value, int>::type = 0>
            GArrayDense(GP guidepack) : GABase(guidepack),
                data_(std::shared_ptr<tvec>(new T[gpp()->num_elements()])),
                ar_(data_.get(), gp_->extent()) {}

            template<class GP, typename std::enable_if<std::is_base_of<guides::GuidePackDyn<n>, GP>::value, int>::type = 0>
            GArrayDense(GP guidepack, std::string name) : GABase(guidepack,name),
                    data_(std::shared_ptr<tvec>(new T[gpp()->num_elements()])),
                            ar_(data_.get(), gp_->extent()) {}


            ///@brief Specialized constructor which casts a generic GuidePackPtr to an appropritate GuidePackDyn
            GArrayDense(const guides::GuidePackPtr &guidepack) : GABase(guidepack),
                data_(std::shared_ptr<tvec>(new T[gpp()->num_elements()])),
                        ar_(data_.get(), gp_->extent()) {}

            ///@brief construct from shared guidepack (i.e. internal guidepack will be co-owned)
            GArrayDense(gp_ptr_t guidepackptr): GABase(guidepackptr),
                data_(std::shared_ptr<tvec>(new T[gpp()->num_elements()])),
                        ar_(data_.get(), gp_->extent()) {}

            //note although empty, we always need to construct the multi_array_ref using a non-default constructor
            GArrayDense():GABase(),
                data_(std::shared_ptr<tvec>(new T[gpp()->num_elements()])),
                        ar_(data_.get(), gp_->extent()){}

            inline arr &mat() { return ar_; }

            inline const arr &mat() const { return ar_; }

            //Extract eigen matrix (but only for 2D arrays)
            template<int dim = n>
            typename std::enable_if<dim == 2, eigm>::type eig() {
                auto marrStrd = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(ar_.strides()[1], ar_.strides()[0]);
                return eigm(ar_.data(), ar_.shape()[0], ar_.shape()[1], marrStrd);
            }

            template<int dim = n>
            typename std::enable_if<dim == 2, const_eigm>::type eig() const {
                auto marrStrd = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(ar_.strides()[1], ar_.strides()[0]);
                return const_eigm(ar_.data(), ar_.shape()[0], ar_.shape()[1], marrStrd);
            }

            template<class GP>
            GArrayDense<T, GP::ndim> reshape(const GP &gpin) const;

            template<class Element, int nd = n, typename std::enable_if<nd == 1, int>::type = 0>
            inline T &operator[](const Element &el) {
                return ar_[gpp()->idx(0, el)];

            }


            template<class Element, int nd = n, typename std::enable_if<nd != 1, int>::type = 0>
            inline subarr operator[](const Element &el) {
                return subarr(this, ar_[gpp()->idx(0, el)]);
            }

            GABase &operator=(const T scalar) override {
                std::fill(ar_.data(), ar_.data() + ar_.num_elements(), scalar);
                return *this;
            }
            void save(io::Group &ar) const override {

                //save guidepacks
                GABase::save(ar);

                //save matrix data
                auto mar = ar.getVariable(name());
                mar->setValue(HyperSlabConstRef<T>(mat()));
            }

            void load(io::Group &ar) override {
                //load guidepack
                GABase::load(ar);
                //allocate new memory so it fits the new guidepack
                realloc();
                auto mar = ar.getVariable(name());
                HyperSlab <T> hslab(mat(), data_);
                mar->getValue(hslab);
                //we may need to reassign the internal multi_array_ref so it uses the correct data
                reassign();

            }
        protected:

            std::shared_ptr <tvec> data_{};
            arr ar_{{}};
        private:
            ///@brief resize data and internal array to a possibly update guidepack
            void reset(const std::shared_ptr <tvec> &data) {
                data_ = data;
                reassign();
            }

            void realloc() {
                data_.reset(new T[gpp()->num_elements()]);
                //also make sure the internal multi_arrary_ref uses this newly allocated data
                reassign();
            }

            ///@brief reassign internal multi_array_ref
            void reassign() {
                ar_.~arr();
                new(&ar_) arr(data_.get(), gpp()->extent());
            }

        };

        ///@brief keep the same data but reshape according to new guidepack
        template<class T, int n>
        template<class GP>
        GArrayDense<T, GP::ndim> GArrayDense<T, n>::reshape(const GP &gpin) const {
            if (gpp()->num_elements() != gpin.num_elements()) {
                THROWINPUTEXCEPTION("reshape failed: guidepacks don't contain the same amount of elements");
            }
            //default constructor doesn't allocate (which is want we want)
            GArrayDense<T, GP::ndim> gaout;
            //assign guides
            gaout.gp() = gpin;
            //assign data pointer
            gaout.data_ = data_;
            //create a renewed multi_array_ref (ugly hack with placement new)
            using arrnew=typename GArrayDense<T, GP::ndim>::arr;
            gaout.ar_.~arrnew();
            new(&(gaout.ar_)) arrnew(gaout.data_.data(), gaout.gpp()->extent());

            return gaout;

        }

        template<class T, class ... Guides>
        class GArrayDense_spec;

        template<class T, class ... Guides>
        class GArrayDense_spec<T, guides::GuidePack < Guides...>>

        :
        public GArrayDense<T, sizeof ...(Guides)> {
        public:
        using GPack=guides::GuidePack<Guides...>;
        using GAdense=GArrayDense<T, sizeof ... (Guides)>;
        static const int n =sizeof ... (Guides);
        using GAdense::name;
        using GAdense::ar_;
        using GAdense::operator=;
        using subarr=GSubArray<T, n - 1, GArrayDense_spec>;

//note this shadows
        inline const GPack &gp() const { return *(std::static_pointer_cast<GPack>(GAdense::gpp())); }

        inline const std::shared_ptr <GPack> gpp() const { return std::static_pointer_cast<GPack>(GAdense::gpp()); }
///allows the extraction of the guide types during compile time
        template<int n>
        using g_t=typename GPack::template g_t<n>;

        template<int n>
        using gptr_t=typename GPack::template gptr_t<n>;

        ///allow compile time access and resolution to the underlying guides
        template<int nd>
        inline gptr_t<nd> g() {
            return gp().template g<nd>();
        }

        template<int nd>
        inline const gptr_t<nd> g() const {
            return gp().template g<nd>();
        }

///structors
        GArrayDense_spec() : GAdense() {

        }

        GArrayDense_spec(GPack
        gp):
        GAdense(std::move(gp)
        ) {
    }


    GArrayDense_spec(Guides
    ... guides) :

    GAdense (GPack(std::move(guides)...)) {}

    template<int nd = n, typename std::enable_if<nd == 1, int>::type = 0>
    T &operator[](const typename g_t<0>::Element &el) {
        return ar_[g<0>()->idx(el)];
    }

    template<int nd = n, typename std::enable_if<nd != 1, int>::type = 0>
    subarr operator[](const typename g_t<0>::Element &el) {
        return subarr(this, ar_[g<0>()->idx(el)]);
    }

private:

};




/// A selection of tools to create dense Garrays

template<class T>
struct createDenseGAr {

    template<int n>
    static GArrayDense <T, n> ones(guides::GuidePackDyn <n> gpin) {
        return fromGuidePack<n>::fill(1, gpin);
    }

    template<class ... Guides>
    static GArrayDense_spec <T, guides::GuidePack<Guides...>> ones(Guides ... guides) {
        return fromGuides<Guides...>::fill(1, std::forward<Guides>(guides)...);
    }

    template<int n>
    static GArrayDense <T, n> zeros(guides::GuidePackDyn <n> gpin) {
        return fromGuidePack<n>::fill(0, gpin);
    }

    template<class ... Guides>
    static GArrayDense_spec <T, guides::GuidePack<Guides...>> zeros(Guides ... guides) {
        return fromGuides<Guides...>::fill(0, std::forward<Guides>(guides)...);
    }


    template<int n>
    static GArrayDense<T, n * 2> eye(guides::GuidePackDyn <n> gpin) {
        auto gout = fromGuidePack<n * 2>::fill(0, *gpin.append(gpin));
        //fill diagonal using eigen
        gout.eig().diagonal().array() = static_cast<T>(1);
        return gout;
    }


    template<class ... Guides>
    struct fromGuides {

        static GArrayDense_spec <T, guides::GuidePack<Guides...>> fill(const T &fillvalue, Guides ... guides) {
            GArrayDense_spec <T, guides::GuidePack<Guides...>> gout(std::move(guides)...);
            gout = fillvalue;
            return gout;

        }

    };


    template<int n>
    struct fromGuidePack {

        static GArrayDense <T, n> fill(const T &fillvalue, guides::GuidePackDyn <n> gpin) {

            GArrayDense <T, n> gout(std::move(gpin));
            gout = fillvalue;
            return gout;
        }


    };

};


}
}
#endif //FROMMLE_GARRAYDENSE_HPP
