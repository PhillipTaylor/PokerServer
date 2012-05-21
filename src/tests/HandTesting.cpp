/*
 * HandTesting.cpp
 *
 *  Created on: 25 Mar 2012
 *      Author: phill
 */

#include "core/Card.h"
#include "core/Hand.h"
#include "core/Deck.h"
#include <iostream>

using namespace GameEngineCore;
using std::cout;
using std::endl;
using std::string;

namespace PokerTests {

void hand_testing() {

	//5 players.
	Hand player[5];
	Deck gDeck;

	//deal 5 cards to 5 players
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++)
			player[k].AddCard(gDeck.Top());
	}

	//test Hand::ToString and cards dealt okay
	for (int l = 0; l < 5; l++)
		cout << "Hand l=" << l << " is now " << player[l] << endl;

	//test Hand::operator[] //check for just player 0
	for (size_t m = 0; m < player[0].GetCardCount(); m++)
		cout << "Player 0, card " << m << " is " << player[0][m].ToLongString() << endl;

	//Testing card algorithms. See what all the players got!!! (This should be random each time)
	for (int n = 0; n < 5; n++) {
		cout << "Player " << n << " has " << player[n].GetHandTextualDescription() << endl;
	}
}
}



