/*
 * main.cpp
 *
 */

#include <iostream>
#include <ctime>
#include <exception>
#include "poker_utils/Logger.h"
#include "tests/PokerTests.h"

using namespace std;
using namespace PokerUtils::Logger;
using namespace poker_tests;

void init();

int main(int argc, char** argv) {

	init();

	PokerTests pt;
	pt.RunTests();

	log_info << "Game Engine Returned\n";
	return 0;
}

/* This function is critical for RNG */
void init() {
	LoggingInit(DEBUG);
	log_info << "Logging Initialised!\n";
	srand(time(nullptr));
	log_info << "RNG Initialised\n";
}

