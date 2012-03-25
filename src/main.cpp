/*
 * main.cpp
 *
 *  Created on: 8 Jan 2012
 *      Author: phill
 */

#include <iostream>
#include <ctime>
#include <exception>
#include "core/Card.h"
#include "core/Deck.h"
#include "core/Hand.h"
#include "comms/RandomBotPlayer.h"
#include "core/GameEngine.h"
#include "poker_utils/Logger.h"

using namespace std;
using namespace GameEngineCore;
using namespace PokerUtils::Logger;
using ConcretePlayers::RandomBotPlayer;

void init();
void card_testing();
void deck_testing();
void hand_testing();
void hand_detection_testing();
void game_engine_testing();

int main(int argc, char** argv) {

	init();
	//card_testing();
	//deck_testing();
	//hand_testing();
	//hand_detection_testing();
	try {
		game_engine_testing();
	} catch (exception& err) {
		cout << "ERR: " << err.what() << endl;
	} catch (...) {
		cout << "Exception was catchable" << endl;
	}

	cout << "Game Engine Returned" << endl;
	return 0;
}

/* This function is critical for RNG */
void init() {
	LoggingInit(DEBUG);
	log_info << "Logging Initialised!\n";
	srand(time(nullptr));
	log_info << "RNG Initialised\n";
}

void card_testing() {

	Card c(3, CLUBS);
	Card d(KING, HEARTS);

	cout << "Got " << c << " and " << d << endl;
}

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

void hand_detection_testing() {
	Card c_one_and_nothing = Card(3, CLUBS);
	Hand ht1;

	ht1.AddCard(c_one_and_nothing);
	cout << "Expect High Card 3: " << ht1.GetHandTextualDescription() << endl;

	Card c_two_and_a_pair[] = {
		{3, CLUBS},
		{3, SPADES}
	};

	Hand ht2;
	ht2.AddCard(c_two_and_a_pair[0]);
	ht2.AddCard(c_two_and_a_pair[1]);
	cout << "Expect Pair of 3s: " << ht2.GetHandTextualDescription() << endl;

	Card c_two_pair[] = {
		{3, CLUBS},
		{3, SPADES},
		{7, DIAMONDS},
		{8, SPADES},
		{7, HEARTS},
		{ACE, HEARTS}
	};

	Hand ht3;
	for (Card& ht3c : c_two_pair)
		ht3.AddCard(ht3c);

	cout << "Expect Two Pair: " << ht3.GetHandTextualDescription() << endl;

	Card c_toak[] = {
		{7, CLUBS},
		{3, SPADES},
		{7, DIAMONDS},
		{8, SPADES},
		{7, HEARTS},
		{ACE, HEARTS}
	};

	Hand ht4;
	for (Card& ht4c : c_toak)
		ht4.AddCard(ht4c);

	cout << "Expect Three of a Kind: " << ht4.GetHandTextualDescription() << endl;

	Card c_stra[] = {
		{2, CLUBS},
		{3, SPADES},
		{5, DIAMONDS},
		{4, SPADES},
		{6, HEARTS},
		{KING, HEARTS}
	};

	Hand ht5;
	for (Card& ht5c : c_stra)
		ht5.AddCard(ht5c);

	cout << "Expect Straight #1 (standard): " << ht5.GetHandTextualDescription() << endl;

	Card c_stra2[] = {
		{2, CLUBS},
		{3, SPADES},
		{5, DIAMONDS},
		{4, SPADES},
		{7, HEARTS},
		{ACE, HEARTS}
	};

	Hand ht6;
	for (Card& ht6c : c_stra2)
		ht6.AddCard(ht6c);

	cout << "Expect Straight #2 (with low ace): " << ht6.GetHandTextualDescription() << endl;

	Card c_flu1[] = {
		{2, CLUBS},
		{3, CLUBS},
		{KING, CLUBS},
		{5, CLUBS},
		{ACE, CLUBS}
	};

	Hand ht7;
	for (Card& ht7c : c_flu1)
		ht7.AddCard(ht7c);

	cout << "Expect Flush #1: " << ht7.GetHandTextualDescription() << endl;

	Card c_flu2[] = {
		{2, SPADES},
		{3, CLUBS},
		{3, SPADES},
		{5, SPADES},
		{ACE, HEARTS},
		{7, SPADES},
		{ACE, SPADES}
	};

	Hand ht8;
	for (Card& ht8c : c_flu2)
		ht8.AddCard(ht8c);

	cout << "Expect Flush #2: " << ht8.GetHandTextualDescription() << endl;

	Card c_fh1[] = {
		{3, SPADES},
		{3, CLUBS},
		{3, HEARTS},
		{5, SPADES},
		{ACE, HEARTS},
		{7, SPADES},
		{ACE, SPADES}
	};

	Hand ht9;
	for (Card& ht9c : c_fh1)
		ht9.AddCard(ht9c);

	cout << "Expect Full House #1: " << ht9.GetHandTextualDescription() << endl;

	Card c_fh2[] = {
		{3, SPADES},
		{3, CLUBS},
		{3, HEARTS},
		{5, SPADES},
		{ACE, HEARTS},
		{ACE, DIAMONDS},
		{ACE, SPADES}
	};

	Hand ht10;
	for (Card& ht10c : c_fh2)
		ht10.AddCard(ht10c);

	cout << "Expect Full House #2: " << ht10.GetHandTextualDescription() << endl;

	Card c_foak[] = {
		{3, SPADES},
		{ACE, CLUBS},
		{3, HEARTS},
		{5, SPADES},
		{ACE, HEARTS},
		{ACE, DIAMONDS},
		{ACE, SPADES}
	};

	Hand ht11;
	for (Card& ht11c : c_foak)
		ht11.AddCard(ht11c);

	cout << "Expect Four Of A Kind: " << ht11.GetHandTextualDescription() << endl;

	Card c_sf1[] = {
		{3, CLUBS},
		{ACE, CLUBS},
		{5, CLUBS},
		{4, CLUBS},
		{2, DIAMONDS},
		{2, CLUBS},
		{6, CLUBS}
	};

	Hand ht12;
	for (Card& ht12c : c_sf1)
		ht12.AddCard(ht12c);

	cout << "Expect Straight Flush #1 (standard): " << ht12.GetHandTextualDescription() << endl;

	Card c_sf2[] = {
		{3, CLUBS},
		{ACE, CLUBS},
		{5, CLUBS},
		{4, CLUBS},
		{2, DIAMONDS},
		{2, CLUBS},
		{7, CLUBS}
	};

	Hand ht13;
	for (Card& ht13c : c_sf2)
		ht13.AddCard(ht13c);

	cout << "Expect Straight Flush #2 (low ace): " << ht13.GetHandTextualDescription() << endl;

	Card c_sf3[] = {
		{QUEEN, CLUBS},
		{ACE, CLUBS},
		{8, CLUBS},
		{JACK, CLUBS},
		{10, CLUBS},
		{9, CLUBS},
	};

	Hand ht14;
	for (Card& ht14c : c_sf3)
		ht14.AddCard(ht14c);

	cout << "Expect Straight Flush #3 (partial face): " << ht14.GetHandTextualDescription() << endl;

	Card c_sf4[] = {
		{10, DIAMONDS},
		{JACK, DIAMONDS},
		{QUEEN, DIAMONDS},
		{ACE, DIAMONDS},
		{ACE, SPADES},
		{KING, DIAMONDS},
		{7, CLUBS}
	};

	Hand ht15;
	for (Card& ht15c : c_sf4)
		ht15.AddCard(ht15c);

	cout << "Expect Straight Flush #2 (royal!): " << ht15.GetHandTextualDescription() << endl;
}

void game_engine_testing() {

	//put into a vector
	RandomBotPlayer rbp_allocs[10]; //actually have on the stack.
	vector<AbstractPlayer*> players;

	for (int i = 0; i < 10; i++) {
		rbp_allocs[i].SetTotalBalance(40);
		players.push_back(&rbp_allocs[i]);
	}

	GameEngine ge(players);
	ge.PlayGame();

}
