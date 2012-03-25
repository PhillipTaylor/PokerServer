/*
 * PokerTests.cpp
 *
 *  Created on: 25 Mar 2012
 *      Author: phill
 */

#include "PokerTests.h"
#include <iostream>

using std::cout;
using std::endl;

//Test files
#include "CardTesting.h"
#include "DeckTesting.h"
#include "HandTesting.h"
#include "HandDetectionTesting.h"
#include "GameEngineRandom.h"

namespace poker_tests {

PokerTests::PokerTests() {}

void PokerTests::RunTests() {

	cout << "Perform Card Testing" << endl;
	card_testing();
	cout << "Card Testing Complete" << endl;

	cout << "Perform Deck Testing" << endl;
	deck_testing();
	cout << "Deck Testing Complete" << endl;

	cout << "Perform Hand Testing" << endl;
	hand_testing();
	cout << "Hand Testing Complete" << endl;

	cout << "Perform Hand Detection Testing" << endl;
	hand_detection_testing();
	cout << "Hand Detection Testing Complete" << endl;

	cout << "Perform Random GameEngine Testing" << endl;
	game_engine_random_testing();
	cout << "Perform Random GameEngine Complete" << endl;
}

} /* namespace poker_tests */
