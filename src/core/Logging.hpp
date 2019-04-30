// @brief Provides centralized logging functions (currently some wrapper functions around boost log)
// Copyright 2019 Roelof Rietbroek
//Currently uses a singleton implementation (constructors are private)
// Licensing:
// This file is for private use of the author(s). The author(s) have not
// yet granted permission to publish the software under the GPL license.
// Redistribution of this software  is prohibited.

#ifndef SRC_CORE_LOGGER_
#define SRC_CORE_LOGGER_
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include "core/Singleton.hpp"
//Define preprocessor macros (currently wrap around trivial logging)
#define LOGDEBUG BOOST_LOG_TRIVIAL(debug)
#define LOGINFO BOOST_LOG_TRIVIAL(info)
#define LOGWARNING BOOST_LOG_TRIVIAL(warning)
#define LOGERROR BOOST_LOG_TRIVIAL(error)
#define LOGFATAL BOOST_LOG_TRIVIAL(fatal)


namespace frommle {

	namespace core{

//@brief Singleton logging class provides a centralized way to tweak logging settings
class Logging: public Singleton<Logging> {
public:
	static void setInfoLevel(){
		Logging::setLevel(boost::log::trivial::info);
	}
	static void setWarningLevel(){
		Logging::setLevel(boost::log::trivial::warning);
	}
	
	static void setDebugLevel(){
		Logging::setLevel(boost::log::trivial::debug);
	}


private:
	friend class Singleton<Logging>;
	
	static void setLevel(const boost::log::trivial::severity_level level ){
		boost::log::core::get()->set_filter(
				boost::log::trivial::severity >= level
				);
	}
	//@brief private (!) constructor
	Logging(){
		//default severity level is info and above
		setLevel(boost::log::trivial::info);
	}
};

	}
}

#endif/* SRC_CORE_LOGGER_*/
