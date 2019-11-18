/*! \file
 \brief a set of tools to allows dynamic appending of guides to guidepacks
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

#include "core/GuideRegistry.hpp"
#include "core/Logging.hpp"
#include "core/Exceptions.hpp"
#ifndef SRC_CORE_GUIDE_APPEND_HPP_
#define SRC_CORE_GUIDE_APPEND_HPP_
namespace frommle{
	namespace guides{

		//forward declare ipolymprhic access to the guidePac
		class GuidePackBase;

		using GuidePackPtr=std::shared_ptr<GuidePackBase>;
	
		template<int n>
		class GuidePackDyn;

		template<int n>
		using GPackDynPtr=std::shared_ptr<GuidePackDyn<n>>;
		

		//general template forwward  delaration
		template<class ... T>
		class GauxMembers;
		
		//specialization for multiple temaptle arguments
		template<class T, class ... Types>
		class GauxMembers<T,Types...>:public virtual GauxMembers<Types...>{
			public:
				using next=GauxMembers<Types...>;
				using next::append;
				virtual GuidePackPtr append(const T& Guide)const=0;
				
		};
		
		//specialization for multiple temaptle arguments
		template<class T, class ... Types>
		class GauxMembers<GuideTlist<T,Types...>>:public virtual GauxMembers<Types...>{
			public:
				using next=GauxMembers<Types...>;
				using next::append;
				virtual GuidePackPtr append(const T& Guide)const=0;
		};

		///@brief specialiation for the final type to be considered
		template<class T>
			class GauxMembers<T>{
			public:
			virtual GuidePackPtr append(const T& Guide)const=0;
		};

		template<int n,class T>
		struct GPHelpBase{ 
			static GPackDynPtr<n+1> append(const T& Guide,const GuidePackDyn<n> & gp){
			auto gpout=std::make_shared<GuidePackDyn<n+1>>();
			for(int i=0; i<n;++i){

				gpout->gv(i)=gp.gv(i);
//				LOGINFO << (*gpout)[i]->size() << std::endl;
			}
//			using Gvar=GuideRegistry::Gvar;
//			auto newgv=std::make_shared<T>(Guide);
			gpout->gv(n)=std::make_shared<T>(Guide);
			//LOGWARNING << "assign guide" <<(*gpout)[0]->name()<<std::endl;
			return gpout;
		}
		};

		///@brief to prevent the compiler to go into an infinite loop we need a partial specialized class which stops a t a certain dimension
		template<int n,class T>
		struct GPHelp{
			static GPackDynPtr<n+1> append(const T & Guide, const GuidePackDyn<n> & gp){
				THROWNOTIMPLEMENTED(std::string("In GuideAppender.hpp, please specialize GPHelp<n,T> for n>")+std::to_string(n));
			};
		};

		template<class T>
		struct GPHelp<0,T>:public GPHelpBase<0,T>{
			using GPHelpBase<0,T>::append;
		};


		template<class T>
		struct GPHelp<1,T>:public GPHelpBase<1,T>{
			using GPHelpBase<1,T>::append;
		};

		template<class T>
		struct GPHelp<2,T>:public GPHelpBase<2,T>{
			using GPHelpBase<2,T>::append;
		};


		template<class T>
		struct GPHelp<3,T>:public GPHelpBase<3,T>{
			using GPHelpBase<3,T>::append;
		};


		template<class T>
		struct GPHelp<4,T>:public GPHelpBase<4,T>{
			using GPHelpBase<4,T>::append;
		};

		template<class T>
		struct GPHelp<5,T>:public GPHelpBase<5,T>{
			using GPHelpBase<5,T>::append;
		};

		//general template forwward  delaration
		template<int n ,class ... T>
		class GauxMembers_impl;
		
		//specialization for multiple temaptle arguments
		template<int n,class T, class ... Types>
		class GauxMembers_impl<n,T,Types...>:public GauxMembers_impl<n,Types...>, public virtual GauxMembers<T,Types...>{
			public:
				GauxMembers_impl()=default;
				GauxMembers_impl(const GuidePackDyn<n> * gpin):GauxMembers_impl<n,Types...>(gpin){}
				using next=GauxMembers_impl<n, Types...>;
				using next::append;
				using next::gp_;	
				virtual GuidePackPtr append(const T& Guide)const override{
					return GPHelp<n,T>::append(Guide,*gp_);
				}
				
		};
		
		//specialization for multiple temaptle arguments
		template<int n, class T, class ... Types>
		class GauxMembers_impl<n, GuideTlist<T,Types...>>:public GauxMembers_impl<n,Types...>, public virtual GauxMembers<GuideTlist<T,Types...>>{
			public:
				GauxMembers_impl()=default;
				GauxMembers_impl(const GuidePackDyn<n> * gpin):GauxMembers_impl<n,Types...>(gpin){}
				using next=GauxMembers_impl<n,Types...>;
				using next::append;
				using next::gp_;	
				virtual GuidePackPtr append(const T& Guide)const override{
					return GPHelp<n,T>::append(Guide,*gp_);
				}
		};

		///@brief specialiation for the final type to be considered
		template<int n, class T>
			class GauxMembers_impl<n,T>:public virtual GauxMembers<T>{
			public:
			GauxMembers_impl()=default;
			GauxMembers_impl(const GuidePackDyn<n> * gpin):gp_(gpin){}
			virtual GuidePackPtr append(const T& Guide)const override {
					return GPHelp<n,T>::append(Guide,*gp_);
			}
			//also add the possibility to directly append a guidevariant
			std::shared_ptr<GuidePackDyn<n+1>> append(GuideRegistry::Gvar gv )const{
				auto gpout=std::make_shared<GuidePackDyn<n+1>>();
				for(int i=0; i<n;++i){

					gpout->gv(i)=gp_->gv(i);
//				LOGINFO << (*gpout)[i]->size() << std::endl;
				}
				gpout->gv(n)=gv;
				return gpout;
			}
				

			protected:
			const GuidePackDyn<n> * gp_=nullptr;
		};

		using GauxPureVirt=GauxMembers<GuideRegistry>;
		template<int n>
		using GauxVirtImpl=GauxMembers_impl<n,GuideRegistry>;
		
}
}
#endif /* #SRC_CORE_GUIDE_APPEND_HPP_*/
