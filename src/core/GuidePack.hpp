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

#include <tuple>
#include "core/GuideBase.hpp"
#include "io/Group.hpp"
#include "seqGenerator.hpp"
#include <boost/algorithm/string.hpp>
#include "core/Exceptions.hpp"
#include "core/GuideAppender.hpp"
#include "core/Logging.hpp"

#ifndef CORE_GUIDEPACK_HPP_
#define CORE_GUIDEPACK_HPP_

namespace frommle{

    namespace guides{




///@brief the GuidePackBase provides runtime access to an underlying (templated) Guidepack 
class GuidePackBase: public virtual GauxPureVirt{
    public:
        GuidePackBase(){};
        ~GuidePackBase(){};
        virtual int nDim()const=0;
        using Gvar=GuideRegistry::Gvar;
        using GauxPureVirt::append;
//        using iterator=Gvar*;
//        using const_iterator=const Gvar*;

        using const_iterator=GVIterator;
//        using iterator=GuideBasePtr*;
//        using const_iterator=const GuideBasePtr*;
//        virtual iterator begin()=0;
//        virtual iterator end()=0;
        virtual const_iterator begin()const=0;
        virtual const_iterator end()const=0;
        //dynamic access to the underlying guides should be implemented
        virtual size_t num_elements()const=0;
        //virtual Gvar & operator[](const int i)=0;
        
        virtual GuideBasePtr operator[](const int i)=0;
        virtual const GuideBasePtr operator[](const int i)const=0;
    
        virtual Gvar & gv(const int i)=0;
        virtual const Gvar & gv(const int i)const=0;

    private:

};

using GuidePackBasePtr=std::shared_ptr<GuidePackBase>;

///@brief dynamic GuidePack (holds all possible types,dimension is fixed at compile time)
template<int n>
class GuidePackDyn: public virtual GuidePackBase,public GauxVirtImpl<n>{
    public:
        GuidePackDyn():GauxVirtImpl<n>(this){
            //fill up the internal array with empty GuideBases
//            std::for_each(gpar_.begin(),gpar_.end(),[](GuideBasePtr & gb){
//                gb=std::make_shared<GuideBase>();
//            });
            std::for_each(gpar_.begin(),gpar_.end(),[](Gvar & gb){
                gb=std::make_shared<GuideBase>();
            });
        }
    template<class GP, typename std::enable_if< std::is_base_of<GuidePackDyn,GP>::value_type,int>::type =0 >
    GuidePackDyn(GP && gpin):GauxVirtImpl<n>(this){
        gpar_=std::move(gpin.gpar_);
    }

    template<class GP,typename std::enable_if< std::is_base_of<GuidePackDyn,GP>::value_type,int>::type =0 >
    GuidePackDyn(const GP & gpin, int istart):GauxVirtImpl<n>(this){
            //enforce that the input has enough guides to tap from
            assert((GP::ndim-istart) >= n);
        for(int i=0;i<n;++i){
            gpar_[i]=gpin.gpar_[i+istart];
        }
    }

    ///@brief admittedly, this is a pretty ackward contructor, but it is needed to apply SFINAE in order to distinguish whether we have GUides or a guidepack as arguments
        template<class G1,class ... Guides,typename std::enable_if<!std::is_base_of<GuidePackDyn, G1>::value,int>::type =0>
        explicit GuidePackDyn(G1 G1arg,Guides ... Args):GauxVirtImpl<n>(this){
            gpar_={{std::make_shared<G1>(std::move(G1arg)),std::make_shared<Guides>(std::move(Args))...}};
        }


        GuidePackDyn(const GuidePackDyn & in):GauxVirtImpl<n>(this){
            gpar_=in.gpar_;
        }


        //using Gvar=GuideRegistry::Gvar;
        using GauxVirtImpl<n>::append;
        //also allow appending a shared_ptr
        //std::shared_ptr<GuidePackDyn<n+1>> append(const GuideBasePtr & gin)const;

        //and allow appending an entire  guidepack
        template<int nadd>
        std::shared_ptr<GuidePackDyn<n+nadd>> append(const GuidePackDyn<nadd> & gpin)const;

        //allow apppending a boost variant

        std::shared_ptr<GuidePackDyn<n+1>> append(Gvar gvar)const;

        ///@brief strip guides from the guidepack
        template<int m=n>
       typename std::enable_if< m != 0,std::shared_ptr<GuidePackDyn<n-1>>>::type strip(bool right=true)const;

        int nDim()const override {return n;}
        static const int ndim=n;

        //@brief return the index corresponding to a certain Guide Element
        template<class Element>
        size_t idx(int i,const Element & el)const{
            return  boost::apply_visitor(guides::gvar_idx<Element>(el),gpar_[i]);
        }


        template<int i,class Element>
        size_t idx(const Element & el)const{
            return  boost::apply_visitor(guides::gvar_idx<Element>(el),std::get<i>(gpar_));
        }
//        GuideBasePtr & operator[](const int i)override{return gpar_[i];}
//        const  GuideBasePtr operator[](const int i)const override {return gpar_[i];}

        GuideBasePtr operator[](const int i)override{
            return boost::apply_visitor(gvar_baseptr(),gpar_[i]);
        }

        const  GuideBasePtr operator[](const int i)const override {
            return boost::apply_visitor(gvar_baseptr(),gpar_[i]);
        }

        //aliases for cleaner code when using pointers
        inline GuideBasePtr at(const int i){return this->operator[](i);}

        inline const GuideBasePtr at(const int i)const{return this->operator[](i);}


        //direct access to the underlying boost variants
        Gvar & gv(const int i)override{return gpar_[i];}
        const Gvar & gv(const int i)const override{return gpar_[i];}

//        extract a specific type of guide from the guidepack
        template<class T>
        inline const std::shared_ptr<T> as(const int i)const{return std::static_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}
        template<class T>
        inline std::shared_ptr<T> as(const int i){return std::static_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}

        size_t num_elements()const override;
        std::array<size_t,n> extent()const;


        const_iterator begin()const override{ return GVIterator(gpar_.begin());}
        const_iterator end()const override{ return GVIterator(gpar_.end());}

        void save(io::Group &ar)const{
            for(auto const & gd:gpar_){
                boost::apply_visitor(guides::gvar_save(&ar),gd);
            }
        }

            void load(io::Group &ar){
                for(auto & gd:gpar_){
                    boost::apply_visitor(guides::gvar_load(&ar),gd);
                }
            }
    protected:
        std::array<Gvar,n> gpar_{};

};

        template<int n>
        size_t GuidePackDyn<n>::num_elements() const {
            //extract the elements from the guides
            //quick return when dimension is 0
            if(n ==0){
                return 0;
            }

            auto ext=extent();
            return std::accumulate(ext.cbegin(),ext.cend(),1,std::multiplies<size_t>());
        }

        template<int n>
        std::array<size_t,n> GuidePackDyn<n>::extent()const{
            std::array<size_t,n> ext;
            std::transform(gpar_.cbegin(),gpar_.cend(),ext.begin(),[](const Gvar & gb){
                return boost::apply_visitor(gvar_size(),gb);
//                return gb->size();
            });
            return ext;
        };


        template<int n>
        template<int nadd>
        std::shared_ptr<GuidePackDyn<n+nadd>> GuidePackDyn<n>::append(const GuidePackDyn<nadd> & gpin)const{
            auto gpout=std::make_shared<GuidePackDyn<n+nadd>>();
            for(int i=0; i<n;++i){
                gpout->gv(i)=this->gv(i);
            }

            //append others guides
            //
            for(int i=0; i<nadd;++i){
                gpout->gv(i+n)=gpin.gv(i);
            }
            return gpout;

        }

        template<int n>
        std::shared_ptr<GuidePackDyn<n+1>> GuidePackDyn<n>::append(Gvar gvar)const{

            auto gpout=std::make_shared<GuidePackDyn<n+1>>();
            for(int i=0; i<n;++i){
                gpout->gv(i)=this->gv(i);
            }

            //append new guide directly as boost:variant
            gpout->gv(n)=gvar;
            return gpout;

        }

        template<int n>
        template<int m>
        typename std::enable_if< m != 0,std::shared_ptr<GuidePackDyn<n-1>>>::type GuidePackDyn<n>::strip(bool right)const{

            auto gpout=std::make_shared<GuidePackDyn<n-1>>();
            int istart=right?0:1;
            int iend=right?n-1:n;
            for(int i=istart; i<iend;++i){
                gpout->gv(i)=this->gv(i);
            }

        //LOGWARNING << "assign guide" <<(*gpout)[0]->name()<<std::endl;
            return gpout;
        }

/*!brief
 * Wraps several guides into a tuple and provide access functions
 * @tparam Guides: a variadic list of guides which spans the dimensions
 */
        template< class ... Guides >
        class GuidePack: public GuidePackDyn<sizeof ...(Guides)> {
        public:
            using GPdyn=GuidePackDyn<sizeof ... (Guides)>;
            using GPdyn::ndim;
            using GPdyn::gpar_;
            using GPdyn::extent;
            using GPdyn::num_elements;

            ///@brief alias to store the types of the input
            using guides_t=std::tuple<Guides...>;

            ///@brief this allows the compile time extraction of the Guide type
            template<int n>
            using g_t=typename std::tuple_element<n,guides_t>::type;
            
            template<int n>
            using gptr_t=typename std::shared_ptr<typename std::tuple_element<n,guides_t>::type>;

            GuidePack(){}
            explicit GuidePack(Guides && ... Args):GPdyn(std::move(Args)...){
                //extent_={Args.size()...};
//                guides_ = std::make_tuple(std::forward<(std::make_shared<Guides>(std::forward<Guides>(Args)...)...);
                //guides_ = std::make_tuple(std::make_shared<Guides>(std::move(Args))...);

            }

            explicit GuidePack(const Guides & ... Args):GPdyn(Args...){}

            /*!brief
             * Get the nth guide of the array
             * @tparam n
             * @return a reference to the Guide instance belonging to the Garray
             */
            template<int n>
            gptr_t<n>  g() {
                assert(n<=ndim);
                return boost::get<gptr_t<n>>(gpar_[n]);
//                return std::static_pointer_cast<g_t<n>>(gpar_[n]);
            }
//
            template<int n>
            const gptr_t<n> g()const{
                assert(n<=ndim);
                return boost::get<gptr_t<n>>(gpar_[n]);
            }

            template<int i>
            size_t idx(const typename g_t<i>::Element & el)const{
                return  g<i>()->idx(el);
            }

        private:
        };

    }
}

#endif /* _CORE_GUIDEPACK_HPP_ */
