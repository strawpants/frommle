// @brief Logging class implementation which is centralized singleton class managing program-wide logging. 
// Copyright 2014 Roelof Rietbroek

// Licensing:
// This file is for private use of the author(s). The author(s) have not
// yet granted permission to publish the software under the GPL license.
// Redistribution of this software  is currently prohibited.

#include <core/Logger.hpp>

namespace frommle {
//default constructor defaults to standard error logging with a "LOG:" prefix
Logger::Logger() :
		logFileStream(NULL), logStream(&std::cerr), prefix("LOG: "), newLine(
				true) {
}


//destructor closes possible open logfile
Logger::~Logger() {
//    	Close open file
	if (logFileStream) {
		if (logFileStream->is_open()) {
			logFileStream->close();
		}
		delete logFileStream; //we don't want memory leaks
		logFileStream = NULL;
	}
}


void Logger::setPrefix(const std::string prefix) {
	Logger::prefix = prefix;
}


void Logger::setStream(std::ostream & outStream) {
	logStream = &outStream;
}


void Logger::setFile(const std::string logFile) {
	logFileStream = new std::ofstream;
	logFileStream->open(logFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);

}


//    std::ofstream * Logger::getFileStream(){
//    	return logFileStream;
//    }

void Logger::unsetStream() {
	logStream = NULL;
}


void Logger::unsetFile() {
	//Close open file if it exists at all
	if (logFileStream) {
		if (logFileStream->is_open()) {
			logFileStream->close();
		}
		logFileStream = NULL;
	}

}


Logger & Logger::operator <<(std::ostream& (*input)(std::ostream&)) {
	if (logStream) {
		*logStream << input;
	}
	if (logFileStream) {
		*logFileStream << input;
	}
	newLine = true;
	return *this;
}


Logger & Logger::operator <<(std::ios& (*input)(std::ios&)) {
	if (logStream) {
		*logStream << input;
	}
	if (logFileStream) {
		*logFileStream << input;
	}
	return *this;
}


Logger & Logger::operator <<(std::ios_base& (*input)(std::ios_base&)) {
	if (logStream) {
		*logStream << input;
	}
	if (logFileStream) {
		*logFileStream << input;
	}
	return *this;

}


//implementation part of InfoLogger
//InfoLogger *InfoLogger::Instance = NULL;
unsigned int InfoLogger::verbosity = 1000; //large enough by default

InfoLogger::InfoLogger() {
	this->setPrefix("");//No prefix unless specified otherwise
	this->setStream(std::cerr); //already the default but set explicitly
}


//overloads get() inherited from the Singleton
Logger & InfoLogger::get(const unsigned int level) {
	//make sure unrequested stuff doesn't get logged
	if (level > verbosity) {
		return NullLogger::get();
	}

	return frommle::Singleton<frommle::InfoLogger>::get();
}


//@brief sets the verbosity level of the output
void InfoLogger::setVerbosity(const unsigned int level) {
	verbosity = level;
}


//implementation of ErrorLogger


ErrorLogger::ErrorLogger() {
	this->setPrefix("Error: ");
	this->setStream(std::cerr); //already the default but set explicitly

}



//the NullLogger does not log anywhere
NullLogger::NullLogger() {
	this->unsetFile();
	this->unsetStream();

}



//implementation part of DebugLogger
unsigned int DebugLogger::verbosity = 1000; //large enough by default

DebugLogger::DebugLogger() {
	this->setPrefix("DEBUG: ");
	this->setStream(std::cerr); //already the default but set explicitly
}


Logger & DebugLogger::get(const unsigned int level) {
	//make sure unrequested stuff doesn't get logged
	if (level > verbosity) {
		return NullLogger::get();
	}

	return frommle::Singleton<frommle::DebugLogger>::get();
}


//@brief sets the verbosity level of the output
void DebugLogger::setVerbosity(const unsigned int level) {
	verbosity = level;
}


}


