/*
 * Logger.cpp
 *
 *  Created on: 19 Feb 2012
 *      Author: phill
 */

#include "Logger.h"

using namespace std;

namespace PokerUtils {
namespace Logger {

LogLevel global_log_level_ = NOT_INITIALISED;
bool in_string = false;

Logger log_trace(TRACE);
Logger log_debug(DEBUG);
Logger log_info(INFO);
Logger log_warn(WARN);
Logger log_error(ERROR);

void LoggingInit(LogLevel log_level) {
	global_log_level_ = log_level;
}

Logger::Logger(LogLevel log_level) :
	m_log_level(log_level)
{}

/* This logger is NOT multithreading safe. */

void Logger::print(const std::string& str) {
	if (m_log_level >= global_log_level_) {

		if (!in_string) {

			switch(m_log_level) {
				case (TRACE):
					cout << "TRACE: ";
					break;
				case (DEBUG):
					cout << "DEBUG: ";
					break;
				case (INFO):
					cout << "INFO: ";
					break;
				case (WARN):
					cout << "WARN: ";
					break;
				case (ERROR):
					cout << "ERROR: ";
					break;
				default:
					cout << "UNRECOGNISED_LOG_LEVEL";
					break;
			}

			in_string = true;
		}

		if (str[str.length()-1] == '\n')
			in_string = false;

		cout << str;
	}
}

} /* namespace PokerUtils */
} /* namespace PokerUtils */
