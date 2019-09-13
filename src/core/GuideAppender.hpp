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

#include "core/IndexGuide.hpp"
#include "core/TimeGuide.hpp"
#include "core/Logging.hpp"
#include "core/Exceptions.hpp"
#include <boost/variant.hpp>
#ifndef SRC_CORE_GUIDE_APPEND_HPP_
#define SRC_CORE_GUIDE_APPEND_HPP_
namespace frommle{
	namespace core{

		//forward declare ipolymprhic access to the guidePac
		class GuidePackBase;

		using GuidePackPtr=std::shared_ptr<GuidePackBase>;
	
		template<class ... Guides>
		class GuidePack;
		
		template<int n>
		class GuidePackDyn;

		template<int n>
		using GPackPtr=std::shared_ptr<GuidePackDyn<n>>;
		
		template<class ... T>
		struct GuideTlist{
            static const int nguides=sizeof...(T);
			using Gvar=boost::variant<std::shared_ptr<T>...>;	
		};
		
		
		using GuideRegistry=GuideTlist<IndexGuide,DateGuide>;
		

		//general template forwward  delaration
		template<class ... T>
		class GauxMembers;
		
		//specialization for multiple temaptle arguments
		template<class T, class ... Types>
		class GauxMembers<T,Types...>:public virtual GauxMembers<Types...>{
			public:
				using next=GauxMembers<Types...>;
				using next::append;
				virtual GuidePackPtr append(T& Guide)=0;
				
		};
		
		//specialization for multiple temaptle arguments
		template<class T, class ... Types>
		class GauxMembers<GuideTlist<T,Types...>>:public virtual GauxMembers<Types...>{
			public:
				using next=GauxMembers<Types...>;
				using next::append;
				virtual GuidePackPtr append(T& Guide)=0;
		};

		///@brief specialiation for the final type to be considered
		template<class T>
			class GauxMembers<T>{
			public:
			virtual GuidePackPtr append(T& Guide)=0;
		};

		template<int n,class T>
		struct GPHelpBase{ 
			static GPackPtr<n+1> append(T& Guide,const GuidePackDyn<n> & gp){
			auto gpout=std::make_shared<GuidePackDyn<n+1>>();
			for(int i=0; i<n;++i){
				(*gpout)[i]=gp[i];
			}
			(*gpout)[n]=std::make_shared<T>(Guide);
			return gpout;
		}
		};
		
		template<int n,class T>
		struct GPHelp{
			static GPackPtr<n+1> append(T & Guide, const GuidePackDyn<n> & gp){
				THROWNOTIMPLEMENTED(std::string("Please specialize GPHelp<n,T> for n=")+std::to_string(n));					
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
				virtual GuidePackPtr append(T& Guide)override{
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
				virtual GuidePackPtr append(T& Guide)override{
					return GPHelp<n,T>::append(Guide,*gp_);
				}
		};

		///@brief specialiation for the final type to be considered
		template<int n, class T>
			class GauxMembers_impl<n,T>:public virtual GauxMembers<T>{
			public:
			GauxMembers_impl()=default;
			GauxMembers_impl(const GuidePackDyn<n> * gpin):gp_(gpin){}
			virtual GuidePackPtr append(T& Guide)override {
					return GPHelp<n,T>::append(Guide,*gp_);
			}
			protected:
			const GuidePackDyn<n> * gp_=nullptr;
		};

		using GauxPureVirt=GauxMembers<GuideRegistry>;
		template<int n>
		using GauxVirtImpl=GauxMembers_impl<n,GuideRegistry>;
		
		////general template forwward  delaration
		//template<class ... T>
		//class GAppender;

		////specialization for multiple temaptle arguments
		//template<class T, class ... Types>
		//class GAppender<T,Types...>:public virtual GAppender<Types...>{
			//public:
				//using next=GAppender<Types...>;
				//using next::append;
				//virtual  GuidePackPtr append(T& Guide)=0;
		//};
		
		////specialization for multiple temaptle arguments
		//template<class T, class ... Types>
		//class GAppender<GuideTlist<T,Types...>>:public virtual GAppender<Types...>{
			//public:
				//using next=GAppender<Types...>;
				//using next::append;
				//virtual GuidePackPtr append(T& Guide)=0;
		//};

		////specialization for multiple temaptle arguments
		//template<class T>
		//class GAppender<GuideTlist<T>>{
			//public:
				//virtual  GuidePackPtr append(T& Guide)=0;
		//};


		/////@brief specialiation for the final type to be considered
		//template<class T>
			//class GAppender<T>{
			//public:
			//virtual GuidePackPtr append(T & Guide)=0;
		//};



		//template<class T, class ... Guides>
		//struct appendtoGuide;


		//template<class T,class ... Guides>
		//struct appendtoGuide<T, GuidePack<Guides...>>{
			//static GuidePackPtr app(const GuidePack<Guides...> * gp,T & guide);
		//};
		

		////general template forwward  delaration
		//template<class GP,class ... T>
		//class GAppenderimpl;

		////specialization for typelisted specialization
		//template<class GP,class T, class ... Types>
		//class GAppenderimpl<GP,GuideTlist<T,Types...>>:public virtual GAppenderimpl<GP,Types...>,public virtual GAppender<GuideTlist<T,Types...>>{
			//public:
				//GAppenderimpl(){}
				//GAppenderimpl(const GP * gpin):GAppenderimpl<GP,Types...>(gpin){} 
				//using GAppenderimpl<GP,Types...>::append;
				//using GAppenderimpl<GP,Types...>::gp_;
				//GuidePackPtr append(T& Guide)override{
					//return appendtoGuide<T,GP>::app(gp_,Guide);
				//}

		//};

		////specialization for multiple temaptle arguments
		//template<class GP,class T, class ... Types>
		//class GAppenderimpl<GP,T,Types...>:public GAppenderimpl<GP,Types...>,public virtual GAppender<T,Types...>{
			//public:
				//GAppenderimpl(){}
				//GAppenderimpl(const GP * gpin):GAppenderimpl<GP,Types...>(gpin){} 
				//using GAppenderimpl<GP,Types...>::append;
				//using GAppenderimpl<GP,Types...>::gp_;
				//GuidePackPtr append(T& Guide)override{
					//return appendtoGuide<T,GP>::app(gp_,Guide);
					////LOGINFO << Guide.name() << typeid(T).name() ;
				//}
		//};
		/////@brief specialiation for the final type to be considered
		//template<class GP,class T>
			//class GAppenderimpl<GP,T>: public virtual GAppender<T>{
			//public:
			//GAppenderimpl(){}
			//GAppenderimpl(const GP * gpin):gp_(gpin){
			
					//LOGINFO << "initializing top level appender";
			//} 
				//GuidePackPtr append(T & Guide)override{
					//return appendtoGuide<T,GP>::app(gp_,Guide);
					////LOGINFO << Guide.name() << typeid(T).name() ;
			//}
			
				//const GP* gp_=nullptr;
		//};




		//using GuideRegister=GAppender<GuideRegistry>;
		//template<class GP>
		//using GuideRegisterimpl=GAppenderimpl<GP,GuideRegistry>;


}
}
#endif /* #SRC_CORE_GUIDE_APPEND_HPP_*/
