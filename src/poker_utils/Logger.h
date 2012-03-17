/*
 * Logger.h
 *
 *  Created on: 19 Feb 2012
 *      Author: phill
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <sstream>
#include "CommonFunctions.h"

using std::ostream;

namespace PokerUtils {
namespace Logger {

enum LogLevel {
	NOT_INITIALISED,
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	DISABLED
};

extern LogLevel global_log_level_;

void LoggingInit(LogLevel log_level);

class Logger {
	public:
		explicit Logger(LogLevel debug_level);
		void print(const std::string& str);

	private:
		LogLevel m_log_level;
};

//Actually create objects that can be invoked like this:
// log_debug << xxx << xxx etc

extern Logger log_trace;
extern Logger log_debug;
extern Logger log_info;
extern Logger log_warn;
extern Logger log_error;

//Now define operator<< for allowing simple invocation

/*Logger& operator<<(Logger& logger, const char *str) {
	logger.print(str);
	return logger;
}*/

template <class T>
Logger& operator<<(Logger& logger, const T& obj) {
	logger.print(AutoToString(obj));
	return logger;
}

} /* namespace Logger */
} /* namespace PokerUtils */
#endif /* LOGGER_H_ */
