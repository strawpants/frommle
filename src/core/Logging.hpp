// @brief Provides centralized logging functions (Must be checked for thread safety at a later stage)
// Copyright 2014 Roelof Rietbroek
//Currently uses a singleton implementation (constructors are private)
// Licensing:
// This file is for private use of the author(s). The author(s) have not
// yet granted permission to publish the software under the GPL license.
// Redistribution of this software  is prohibited.

#ifndef GEOW_LOGGER
#define GEOW_LOGGER

#include <iostream>
#include <fstream>
#include <iomanip>
#include <core/Singleton.hpp>
//Define preprocessor macros




//let the preprocessor remove debug code
#ifdef NDEBUG
#define DEBUGLOG1(x)
#define DEBUGLOG2(x)
#define DEBUGLOG3(x)
#else
#define DEBUGLOG1(x) do{frommle::DebugLogger::get(1) << x << std::endl;}while(0)
#define DEBUGLOG2(x) do{frommle::DebugLogger::get(2) << x << std::endl;}while(0)
#define DEBUGLOG3(x) do{frommle::DebugLogger::get(3) << x << std::endl;}while(0)
#endif

#define INFOLOG  frommle::InfoLogger::get(0)
#define INFOLOG1 frommle::InfoLogger::get(1)
#define INFOLOG2 frommle::InfoLogger::get(2)
#define INFOLOG3 frommle::InfoLogger::get(3)

#define ERRLOG frommle::ErrorLogger::get()

namespace frommle {
//@brief overall logger class (NOT a singleton). This logger class logs either to standard output, standard error or a file
class Logger {
public:
	//default Logger logs to standard error only
	Logger();
	//destructor closes possible open file handles
	virtual ~Logger();
	//set messageprefix which is printed at each newLine
	void setPrefix(const std::string prefix);
	void setStream(std::ostream & outStream);
	void setFile(const std::string logFile);
	void unsetStream();
	void unsetFile();
	//overload stream operator for all types
	template<typename T>
	Logger & operator <<(T input);
	//overload stream operator for std::endl and stream manipulators
	Logger & operator <<(std::ostream& (*input)(std::ostream&));
	Logger & operator <<(std::ios& (*input)(std::ios&));
	Logger & operator <<(std::ios_base& (*input)(std::ios_base&));
//copy constructor and asignment operator declared but  not implemented (prevents memory leaks of logFileStream)
	Logger (Logger const & copy);
	Logger & operator=(Logger const & copy);
private:
	std::ofstream * logFileStream;
	std::ostream * logStream;
	std::string prefix;
	bool newLine;
};

//@brief overloads stream operator for templated types
template<typename T>
Logger & Logger::operator <<(const T input) {
	//output prefix when a newline is started
	if (newLine) {
		if (logStream) {
			*logStream << prefix;
		}
		if (logFileStream) {
			*logFileStream << prefix;
		}
		newLine = false;
	}

	if (logStream) {
		*logStream << input;
	}
	if (logFileStream) {
		*logFileStream << input;
	}

	return *this;

}


//@brief Singleton Infologger class inheriting from a normal Logger
class InfoLogger: public Logger, public Singleton<InfoLogger> {
public:
	static void setVerbosity(const unsigned int level);
	static Logger & get(const unsigned int level);

private:
	friend class Singleton<InfoLogger>;
	InfoLogger();
	static unsigned int verbosity; //only output info up to this level
};


//@brief Singleton Errorlogger class inheriting from a normal Logger
class ErrorLogger: public Logger, public Singleton<ErrorLogger> {
public:

private:
	friend class Singleton<ErrorLogger>;
	ErrorLogger(); //private (!) constructor

};

//also make a NullLogger singleton which does no outputting
class NullLogger: public Logger, public Singleton<NullLogger> {
public:
private:
	friend class Singleton<NullLogger>;
	NullLogger();
};

//@brief Singleton Infologger class inheriting from a normal Logger
class DebugLogger: public Logger,public Singleton<DebugLogger> {
public:
	//returns unique instance of the static logger instance
	static Logger & get(const unsigned int level);
	static void setVerbosity(const unsigned int level);
private:
	friend class Singleton<DebugLogger>;
	DebugLogger(); //private (!) constructor
	static unsigned int verbosity; //only output info up to this level
};

}


#endif
