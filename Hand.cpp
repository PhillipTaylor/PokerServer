/*
 * Hand.cpp
 *
 *  Created on: 8 Jan 2012
 *      Author: phill
 */

#include <assert.h>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "Hand.h"

using std::map;
using std::string;
using std::pair;
using std::vector;
using std::runtime_error;
using std::stringstream;

Hand::Hand() :
	m_hand()
{
	m_hand.reserve(MAX_CARDS_IN_HAND);
}

void Hand::AddCard(const Card& card) {
	assert(m_hand.size() <= MAX_CARDS_IN_HAND);

	m_hand.push_back(card);
	CalculateHandScore();
}

string Hand::GetHandTextualDescription() const {

	if (m_hand.size() == 0)
		return "No Cards in hand";

	string firstCardStr = CardValueToString(m_best_hand_score.firstCardValue, true);
	string secondCardStr = CardValueToString(m_best_hand_score.secondCardValue, true);

	switch(m_best_hand_type) {
		case (HT_HIGH_CARD):
			return "High Card of " + firstCardStr;
		case (HT_PAIR):
			return "Pair of " + firstCardStr + "s";
		case (HT_TWO_PAIR):
			return "Two Pair of " + firstCardStr + "s and " + secondCardStr + "s";
		case (HT_STRAIGHT):
			return "Straight, " + firstCardStr + " high";
		case (HT_THREE_OF_A_KIND):
			return "Three of a Kind with " + firstCardStr + "s";
		case (HT_FLUSH):
			return "Flush with " + CardSuitToString(m_best_hand_score.suitValue);
		case (HT_FULL_HOUSE):
			return "Full House, " + firstCardStr + "s full of " + secondCardStr + "s";
		case (HT_FOUR_OF_A_KIND):
			return "Four of a Kind with " + firstCardStr + "s";
		case (HT_STRAIGHT_FLUSH):
			if (m_best_hand_score.firstCardValue == ACE)
				return "Royal Flush!!! (" + CardSuitToString(m_best_hand_score.suitValue) + ")";
			else
				return "Straight Flush of " +
						CardSuitToString(m_best_hand_score.suitValue) +
						" " + firstCardStr + " high";
		default:
			throw new runtime_error("This can't happen."); //to silence gcc "may return void warning"
			//claus never hit, because with no cards I return early at top of function, with one card
			//I hit the HT_HIGH_CARD case.
	}

}

string Hand::ToString() const {
	string s;
	bool first = true;

	for (const Card& c : m_hand) {

		if (!first)
			s+= ", ";

		s += c.ToString(true);

		first = false;
	}

	return s;
}

const Card& Hand::operator[](int position) const {
	assert(position >= 0);
	assert(m_hand.size() > (unsigned int)position);
	return m_hand[position];
}

void Hand::CalculateHandScore() {

	assert(m_hand.size() > 0);

	//the m_hand_score object represents a hand score, we simply update it.

	typedef const HandScore (Hand::*ScoringFuncPtr)() const; //All the Get..Score() functions match this signature.
	typedef pair<HandType, ScoringFuncPtr> HPair;

	vector<HPair > scoringFunctions = {                    //ordered by rank.
			{ HT_STRAIGHT_FLUSH,  &Hand::GetStraightFlushScore },
			{ HT_FOUR_OF_A_KIND,  &Hand::GetFourOfAKindScore },
			{ HT_FULL_HOUSE,      &Hand::GetFullHouseScore },
			{ HT_FLUSH,           &Hand::GetFlushScore },
			{ HT_STRAIGHT,        &Hand::GetStraightScore },
			{ HT_THREE_OF_A_KIND, &Hand::GetThreeOfAKindScore },
			{ HT_TWO_PAIR,        &Hand::GetTwoPairScore },
			{ HT_PAIR,            &Hand::GetPairScore },
			{ HT_HIGH_CARD,       &Hand::GetHighCardScore } //never returns NO_HAND
	};

	for (const HPair& scorePair: scoringFunctions) {

		HandType this_type = scorePair.first;
		ScoringFuncPtr this_impl = scorePair.second;

		HandScore hand_result = (this->*this_impl)(); //invoke scoring function!

		if (hand_result.handFound) {
			m_best_hand_type = this_type;
			m_best_hand_score = hand_result;
			return;
		}
	}

}

const HandScore Hand::GetStraightFlushScore() const {

	int offset = LOWEST_CARD_VALUE - 1; //leaves space for lowest card to be 1 (low ace)

	int suitNestedValues[NUM_SUITS][(HIGHEST_CARD_VALUE - offset) + 1];

	//intialise map
	for (int suit = CLUBS; suit <= SPADES; suit++) {
		for (int val = 0; val <= (HIGHEST_CARD_VALUE - offset); val++)
			suitNestedValues[suit][val - offset] = 0;
	}

	//count instances
	for (const Card& card : m_hand) {

		int c_suit = card.GetSuit();
		int c_val = card.GetValue();

		if (card.GetValue() == ACE) {  //Ace is HIGH or LOW
			suitNestedValues[c_suit][0]++;
			suitNestedValues[c_suit][c_val - offset]++;
		} else
			suitNestedValues[c_suit][c_val - offset]++;

	}

	int seq = 0;
	int highestSuitSoFar = -1;
	int highestValueSoFar = -1;

	//now looks for 5 in a row, per suit, in suit order + card order.
	//This loop may be reversible so we can break out early.

	for (int suit = CLUBS; suit <= SPADES; suit++) {
		for (int val = 0; val <= (HIGHEST_CARD_VALUE - offset); val++) {

			if (suitNestedValues[suit][val] == 0) {
				seq = 0;
			} else {
				seq++;
				if (seq >= 5) {
					highestValueSoFar = val;
					highestSuitSoFar = suit;
				}
			}
		}
		seq = 0;
	}

	HandScore retval;

	if (highestValueSoFar != -1) {
		retval.handFound = true;
		retval.firstCardValue = highestValueSoFar + offset;
		retval.suitValue = highestSuitSoFar;
	} else {
		retval.handFound = false;
	}

	return retval;

}

const HandScore Hand::GetFourOfAKindScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 4) {
			HandScore retval;
			retval.handFound = true;
			retval.firstCardValue = iter->first;
			return retval;
		}
	}

	HandScore retval;
	retval.handFound = false;
	return retval;
}

const HandScore Hand::GetFullHouseScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	int threes_value = -1;
	int twos_value = -1;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second >= 3 && threes_value == -1)
			threes_value = iter->first;
		else if (iter->second >= 2 && twos_value == -1)  //bug alert: don't fall from 3 into 2 if you refactor.
			twos_value = iter->first;
	}

	if (threes_value != -1 && twos_value != -1) {
		HandScore retval;
		retval.handFound = true;
		retval.firstCardValue = threes_value;
		retval.secondCardValue = twos_value;
		return retval;
	} else {
		HandScore retval;
		retval.handFound = false;
		return retval;
	}
}

const HandScore Hand::GetFlushScore() const {

	map<int, int> occur = {
		{ HEARTS,   0 },
		{ SPADES,   0 },
		{ CLUBS,    0 },
		{ DIAMONDS, 0 }
	};

	for (const Card& card : m_hand)
		occur[card.GetSuit()]++;

	for (pair<int, int> suit_count : occur) {
		if (suit_count.second >= 5) {
			HandScore retval;
			retval.handFound = true;
			retval.suitValue = suit_count.first;
			return retval;
		}
	}

	HandScore retval;
	retval.handFound = false;
	return retval;
}

const HandScore Hand::GetStraightScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand) {
		if (card.GetValue() == ACE) {  //Ace is HIGH or LOW
			occur[1]++;
			occur[ACE]++;
		} else
			occur[card.GetValue()]++;
	}

	int seq = 0;
	int highestSoFar = -1;

	//look for the longest sequence of 5...
	for (map<int, int>::iterator iter = occur.begin(); iter != occur.end(); ++iter) {

		if (iter->second == 0) {
			seq = 0;
		} else {
			seq++;
			if (seq >= 5)
				highestSoFar = iter->first;
		}
	}

	if (highestSoFar != -1) {
		HandScore retval;
		retval.handFound = true;
		retval.firstCardValue = highestSoFar;
		return retval;
	} else {
		HandScore retval;
		retval.handFound = false;
		return retval;
	}
}

const HandScore Hand::GetThreeOfAKindScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 3) {
			HandScore retval;
			retval.handFound = true;
			retval.firstCardValue = iter->first;
			return retval;
		}
	}

	HandScore retval;
	retval.handFound = false;
	return retval;
}

const HandScore Hand::GetTwoPairScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	int number_of_pairs = 0;
	int first_pair_value;

	//iterate backwards to get the highest matches first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 2) {

			if (number_of_pairs == 0)
				first_pair_value = iter->first;

			if (number_of_pairs == 1) {
				HandScore retval;
				retval.handFound = true;
				retval.firstCardValue = first_pair_value;
				retval.secondCardValue = iter->first;
				return retval;
			}

			number_of_pairs++;
		}
	}

	HandScore retval;
	retval.handFound = false;
	return retval;
}

const HandValue Hand::GetPairScore() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 2) {
			HandScore retval;
			retval.handFound = true;
			retval.firstCardValue = iter->first;
			return retval;
		}
	}

	HandScore retval;
	retval.handFound = false;
	return retval;
}

const HandValue Hand::GetHighCardScore() const {

	int card_val = LOWEST_CARD_VALUE;

	for (const Card& card : m_hand) {
		if (card_val < card.GetValue())
			card_val = card.GetValue();
	}

	HandScore retval;
	retval.handFound = true;
	retval.firstCardValue = card_val;
	return retval;
}

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
	os << hand.ToString();
	return os;
}

