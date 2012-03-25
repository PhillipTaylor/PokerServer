/*
 * HandDetectionTesting.cpp
 *
 *  Created on: 25 Mar 2012
 *      Author: phill
 */

#include "../core/Card.h"
#include "../core/Deck.h"
#include "../core/Hand.h"
#include <iostream>

using namespace GameEngineCore;
using std::cout;
using std::endl;
using std::string;

namespace poker_tests {

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


}
