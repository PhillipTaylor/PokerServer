/*
 * DeckTesting.cpp
 *
 *  Created on: 25 Mar 2012
 *      Author: phill
 */

#ifndef DECKTESTING_CPP_
#define DECKTESTING_CPP_

#include <iostream>

#include "core/Card.h"
#include "core/Deck.h"
#include "core/Hand.h"

using namespace GameEngineCore;
using std::cout;
using std::endl;
using std::string;

namespace poker_tests {

void deck_testing() {

	Deck mydeck;

	for (int i = 0; i < CARDS_IN_DECK; i++) {
		cout << "Card " << i << " was " << mydeck.Top() << endl;
		cout << "Deck is now " << mydeck << endl;
	}

	//5 players.
	Hand player[5];
	Deck gDeck;

	//deal 5 cards to 5 players
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++)
			player[k].AddCard(gDeck.Top());
	}

	//Randomising test
	Deck rand_decks[5];

	string hash_ref = rand_decks[0].ToHashString();
	int dupes = 0;

	for (int i = 1; i < 5; i++) {
		if (rand_decks[i].ToHashString() == hash_ref) {
			cout << "Another deck has the hash ref: " << rand_decks[i].ToHashString() << endl;
			dupes++;
		}
	}

	cout << "Duplicate decks: " << dupes << endl;
	if (dupes == 4)
		cout << "BUG ALERT: NOT RANDOM DECKS BEING GENERATED!" << endl;

}

}
#endif /* DECKTESTING_CPP_ */
