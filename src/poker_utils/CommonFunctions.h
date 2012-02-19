/*
 * CommonFunctions.h
 *
 *  Created on: 18 Feb 2012
 *      Author: phill
 */

#ifndef COMMONFUNCTIONS_H_
#define COMMONFUNCTIONS_H_

#include <sstream>

using std::stringstream;

namespace PokerUtils {

	template <class T>
	std::string AutoToString(const T& obj) {
		stringstream ss;
		ss << obj;
		return ss.str();
	}

}

#endif /* COMMONFUNCTIONS_H_ */
