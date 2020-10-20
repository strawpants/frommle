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

#include "core/GuideAppender.hpp"

#ifndef CORE_GUIDEPACK_HPP_
#define CORE_GUIDEPACK_HPP_

#define MAXGUIDES  10

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
                    GuidePackDyn();

                    template<class GP, typename std::enable_if< std::is_base_of<GuidePackDyn,GP>::value_type,int>::type =0 >
                        GuidePackDyn(GP && gpin);

                    template<class GP,typename std::enable_if< std::is_base_of<GuidePackDyn,GP>::value_type,int>::type =0 >
                        GuidePackDyn(const GP & gpin, int istart);

                    ///@brief admittedly, this is a pretty ackward contructor, but it is needed to apply SFINAE in order to distinguish whether we have GUides or a guidepack as arguments
                    template<class G1,class ... Guides,typename std::enable_if<!std::is_base_of<GuidePackDyn, G1>::value,int>::type =0>
                        explicit GuidePackDyn(G1 G1arg,Guides ... Args);


                    GuidePackDyn(const GuidePackDyn & in);


                    //using Gvar=GuideRegistry::Gvar;
                    using GauxVirtImpl<n>::append;
                    //also allow appending a shared_ptr
                    //std::shared_ptr<GuidePackDyn<n+1>> append(const GuideBasePtr & gin)const;

                    //and allow appending an entire  guidepack
                    template<int nadd>
                        std::shared_ptr<GuidePackDyn<n+nadd>> append(const GuidePackDyn<nadd> & gpin)const;

                    //allow apppending a boost variant

                    std::shared_ptr<GuidePackDyn<n+1>> append(Gvar gvar)const;

                    ///@brief strip m guides from the guidepack (from the left or right)
                    template<int m=1>
                        typename std::enable_if< m < n,std::shared_ptr<GuidePackDyn<n-m>>>::type strip(bool right=true)const;

                    int nDim()const override {return n;}
                    static const int ndim=n;

                    //@brief return the index corresponding to a certain Guide Element
                    template<class Element>
                        size_t idx(int i,const Element & el)const;

                    //@brief return the index corresponding to a certain Guide Element (compile time version)
                    template<int i,class Element>
                        size_t idx(const Element & el)const;

                    GuideBasePtr operator[](const int i)override;
                    const  GuideBasePtr operator[](const int i)const override;

                    //aliases for cleaner code when using pointers
                    GuideBasePtr at(const int i);
                    const GuideBasePtr at(const int i)const;


                    //direct access to the underlying boost variants
                    Gvar & gv(const int i)override;
                    const Gvar & gv(const int i)const override;

                    //        extract a specific type of guide from the guidepack
                    template<class T>
                        const std::shared_ptr<T> as(const int i)const;

                    template<class T>
                        std::shared_ptr<T> as(const int i);

                    template<class T>
                        const std::shared_ptr<T> dyn_as(const int i)const;

                    template<class T>
                        std::shared_ptr<T> dyn_as(const int i);

                    size_t num_elements()const override;
                    std::array<size_t,n> extent()const;


                    const_iterator begin()const override;
                    const_iterator end()const override;

                    void save(io::Group &ar)const;
                    void load(io::Group &ar);
                protected:
                    std::array<Gvar,n> gpar_{};

            };


    //declare as extern as they will be explicitly instantiated in GuidePack.cpp
    //THIS MUST BE CONSISTENT with MAXGUIDES (See also the according template instantiations in GuidePack.cpp)
    //The reason for doing this explicitly is to reduce compile times
    extern template class GuidePackDyn<0>;
    extern template class GuidePackDyn<1>;
    extern template class GuidePackDyn<2>;
    extern template class GuidePackDyn<3>;
    extern template class GuidePackDyn<4>;
    extern template class GuidePackDyn<5>;
    extern template class GuidePackDyn<6>;
    extern template class GuidePackDyn<7>;
    extern template class GuidePackDyn<8>;
    extern template class GuidePackDyn<9>;
    // ....
    ///Fill out all up to MAXGUIDES
    extern template class GuidePackDyn<MAXGUIDES>;
    
    
        //templated function implementations which depend on other template parameters except for n

        template<int n>
            template<class GP, typename std::enable_if< std::is_base_of<GuidePackDyn<n>,GP>::value_type,int>::type >
            GuidePackDyn<n>::GuidePackDyn(GP && gpin):GauxVirtImpl<n>(this){
                gpar_=std::move(gpin.gpar_);
            }

        template<int n>
            template<class GP,typename std::enable_if< std::is_base_of<GuidePackDyn<n>,GP>::value_type,int>::type  >
            GuidePackDyn<n>::GuidePackDyn(const GP & gpin, int istart):GauxVirtImpl<n>(this){
                //enforce that the input has enough guides to tap from
                assert((GP::ndim-istart) >= n);
                for(int i=0;i<n;++i){
                    gpar_[i]=gpin.gpar_[i+istart];
                }
            }
        template<int n>
            template<class G1,class ... Guides,typename std::enable_if<!std::is_base_of<GuidePackDyn<n>, G1>::value,int>::type >
            GuidePackDyn<n>::GuidePackDyn(G1 G1arg,Guides ... Args):GauxVirtImpl<n>(this){
                gpar_={{std::make_shared<G1>(std::move(G1arg)),std::make_shared<Guides>(std::move(Args))...}};
            }
        template<int n>
            template<int m>
            typename std::enable_if< m < n,std::shared_ptr<GuidePackDyn<n-m>>>::type GuidePackDyn<n>::strip(bool right)const{

                auto gpout=std::make_shared<GuidePackDyn<n-m>>();
                int istart=right?0:m;
                int iend=right?n-m:n;
                for(int i=istart; i<iend;++i){
                    gpout->gv(i)=this->gv(i);
                }

                //LOGWARNING << "assign guide" <<(*gpout)[0]->name()<<std::endl;
                return gpout;
            }


        template<int n>
            template<class Element>
            size_t GuidePackDyn<n>::idx(int i,const Element & el)const{
                return  boost::apply_visitor(guides::gvar_idx<Element>(el),gpar_[i]);
            }

        template<int n>
            template<int i,class Element>
            size_t GuidePackDyn<n>::idx(const Element & el)const{
                return  boost::apply_visitor(guides::gvar_idx<Element>(el),std::get<i>(gpar_));
            }

		template<int n>
			template<class T>
			const std::shared_ptr<T> GuidePackDyn<n>::as(const int i)const{return std::static_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}

		template<int n>
			template<class T>
			std::shared_ptr<T> GuidePackDyn<n>::as(const int i){return std::static_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}

		template<int n>
			template<class T>
			const std::shared_ptr<T> GuidePackDyn<n>::dyn_as(const int i)const{return std::dynamic_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}

		template<int n>
			template<class T>
			std::shared_ptr<T> GuidePackDyn<n>::dyn_as(const int i){return std::dynamic_pointer_cast<T>(boost::apply_visitor(gvar_baseptr(),gpar_[i]));}

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
    }
}

#endif /* _CORE_GUIDEPACK_HPP_ */
