/*! \file
  \brief some explicitly instantiated templated functions
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
  */

#include "core/GuideBase.hpp"
#include "io/Group.hpp"
#include "core/Exceptions.hpp"
#include "core/Logging.hpp"
#include "core/GuideAppender.hpp"
#include "core/GuidePack.hpp"

///free function to allow streaming the typehas to a stream

namespace frommle{

	namespace guides{
		template<int n>
			GuidePackDyn<n>::GuidePackDyn():GauxVirtImpl<n>(this){
				//fill up the internal array with empty GuideBases
				std::for_each(gpar_.begin(),gpar_.end(),[](GuideRegistry::Gvar & gb){
						gb=std::make_shared<GuideBase>();
						});
			}


		template<int n>
			GuidePackDyn<n>::GuidePackDyn(const GuidePackDyn<n> & in):GauxVirtImpl<n>(this){
				gpar_=in.gpar_;
			}


		template<int n>
			GuideBasePtr GuidePackDyn<n>::operator[](const int i){
				return boost::apply_visitor(gvar_baseptr(),gpar_[i]);
			}

		template<int n>
			const  GuideBasePtr GuidePackDyn<n>::operator[](const int i)const {
				return boost::apply_visitor(gvar_baseptr(),gpar_[i]);
			}

		template<int n>
			GuideBasePtr GuidePackDyn<n>::at(const int i){return this->operator[](i);}
		template<int n>
			const GuideBasePtr GuidePackDyn<n>::at(const int i)const{return this->operator[](i);}

		//direct access to the underlying boost variants
		template<int n>
			GuideRegistry::Gvar & GuidePackDyn<n>::gv(const int i){return gpar_[i];}
		template<int n>
			const GuideRegistry::Gvar & GuidePackDyn<n>::gv(const int i)const {return gpar_[i];}


		template<int n>
			GVIterator GuidePackDyn<n>::begin()const { return GVIterator(gpar_.begin());}
		template<int n>
			GVIterator GuidePackDyn<n>::end()const { return GVIterator(gpar_.end());}

		template<int n>
			void GuidePackDyn<n>::save(io::Group &ar)const{
				for(auto const & gd:gpar_){
					boost::apply_visitor(guides::gvar_save(&ar),gd);
				}
			}

		template<int n>
			void GuidePackDyn<n>::load(io::Group &ar){
				for(auto & gd:gpar_){
					boost::apply_visitor(guides::gvar_load(&ar),gd);
				}
			}

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
			std::shared_ptr<GuidePackDyn<n+1>> GuidePackDyn<n>::append(Gvar gvar)const{

				auto gpout=std::make_shared<GuidePackDyn<n+1>>();
				for(int i=0; i<n;++i){
					gpout->gv(i)=this->gv(i);
				}

				//append new guide directly as boost:variant
				gpout->gv(n)=gvar;
				return gpout;

			}



		//explicitly instantiate a few dynamic guidepacks
		//MUST BE CONSISTENT with MAXGUIDES frpm GuidePack.hpp
		template class GuidePackDyn<0>;
		template class GuidePackDyn<1>;
		template class GuidePackDyn<2>;
		template class GuidePackDyn<3>;
		template class GuidePackDyn<4>;
		template class GuidePackDyn<5>;
		template class GuidePackDyn<6>;
		template class GuidePackDyn<7>;
		template class GuidePackDyn<8>;
		template class GuidePackDyn<9>;
		// ....
		///Fill out all up to MAXGUIDES
		template class GuidePackDyn<MAXGUIDES>;

	}
}

