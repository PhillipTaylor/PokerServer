/*
 * Hand.cpp
 *
 */

#include <assert.h>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "poker_utils/Logger.h"

#include "Hand.h"
#include "Card.h"

using namespace PokerUtils::Logger;
using std::map;
using std::string;
using std::pair;
using std::vector;
using std::runtime_error;
using std::stringstream;
using std::sort;

namespace GameEngineCore {

const HandValue HANDVALUE_NO_HAND = { //This syntax is a C++11 intialiser list on a global const.
	HT_NO_HAND,
	nullptr,
	nullptr
};

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

void Hand::CalculateHandScore() {

	typedef const HandValue (Hand::*scoringFunctionPtr)() const;

	//declare an array of scoring functions...
	scoringFunctionPtr funcs[] = {
			&Hand::GetStraightFlushValue,
			&Hand::GetFourOfAKindValue,
			&Hand::GetFullHouseValue,
			&Hand::GetFlushValue,
			&Hand::GetStraightValue,
			&Hand::GetThreeOfAKindValue,
			&Hand::GetTwoPairValue,
			&Hand::GetPairValue,
			&Hand::GetHighCardValue
	};

	HandValue hand_score;

	//invoke until one returns.
	for (int i = 0; i < 9; i++) {
		hand_score = (this->*(funcs[i]))();

		if (hand_score.type != HT_NO_HAND)
			break;
	}

	m_best_hand = hand_score;
}

string Hand::GetHandTextualDescription() const {

	if (m_hand.size() == 0)
		return "No Cards in hand";

	string firstCardStr = CardValueToString(m_best_hand.primaryCard->GetValue(), true);

	string secondCardStr = "";

	if (m_best_hand.secondaryCard != nullptr)
		secondCardStr = CardValueToString(m_best_hand.secondaryCard->GetValue(), true);

	switch(m_best_hand.type) {
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
			return "Flush with " + CardSuitToString(m_best_hand.primaryCard->GetSuit());
		case (HT_FULL_HOUSE):
			return "Full House, " + firstCardStr + "s full of " + secondCardStr + "s";
		case (HT_FOUR_OF_A_KIND):
			return "Four of a Kind with " + firstCardStr + "s";
		case (HT_STRAIGHT_FLUSH):
			if (m_best_hand.primaryCard->GetValue() == ACE)
				return "Royal Flush!!! (" + CardSuitToString(m_best_hand.primaryCard->GetSuit()) + ")";
			else
				return "Straight Flush of " +
						CardSuitToString(m_best_hand.primaryCard->GetSuit()) +
						" " + firstCardStr + " high";
		default:
			throw new runtime_error("This can't happen."); //to silence gcc "may return void warning"
			//clause never hit, because with no cards I return early at top of function, with one card
			//I hit the HT_HIGH_CARD case.
	}

}

const Card* Hand::GetCardPtr(int suit, int value) const {
	for (const Card& card : m_hand) {
		if (card.GetSuit() == suit && card.GetValue() == value)
			return &card;
	}

	return nullptr;
}

const Card* Hand::GetCardPtrAnySuit(int value) const {
	const Card* tmp = GetCardPtr(HEARTS, value);
	if (tmp == nullptr)
		tmp = GetCardPtr(SPADES, value);
	if (tmp == nullptr)
		tmp = GetCardPtr(CLUBS, value);
	if (tmp == nullptr)
		tmp = GetCardPtr(DIAMONDS, value);
	assert(tmp != nullptr);
	return tmp;
}

string Hand::ToString() const {
	string s;
	bool first = true;

	s = PokerUtils::AutoToString(m_hand.size());
	s += " cards: ";

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

const HandValue Hand::GetStraightFlushValue() const {


	//Element 0 will be 0. Values keep their real equivalents
	int suitNestedValues[NUM_SUITS][HIGHEST_CARD_VALUE+1];

	//intialise map
	for (int suit = 0; suit < NUM_SUITS; suit++) {
		for (int val = 0; val < (HIGHEST_CARD_VALUE + 1); val++)
			suitNestedValues[suit][val] = 0;
	}

	//count instances
	for (const Card& card : m_hand) {

		int c_suit = card.GetSuit();
		int c_val = card.GetValue();

		if (card.GetValue() == ACE) {  //Ace is HIGH or LOW
			suitNestedValues[c_suit][1]++;
			suitNestedValues[c_suit][c_val]++;
		} else
			suitNestedValues[c_suit][c_val]++;

	}

	int seq = 0;
	int highestSuitSoFar = -1;
	int highestValueSoFar = -1;

	//now looks for 5 in a row, per suit, in suit order + card order.
	//This loop may be reversible so we can break out early.

	for (int suit = 0; suit < NUM_SUITS; suit++) {
		for (int val = 1; val < (HIGHEST_CARD_VALUE + 1); val++) { //ignore bucket 0

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

	if (highestValueSoFar != -1) {

		HandValue retval;

		//retrieve matching primary card and return
		retval.type = HT_STRAIGHT_FLUSH;
		retval.primaryCard = GetCardPtr(highestSuitSoFar, highestValueSoFar);
		retval.secondaryCard = nullptr;

		return retval;
	} else {
		return HANDVALUE_NO_HAND;
	}

}

const HandValue Hand::GetFourOfAKindValue() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (auto iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 4) {
			HandValue retval;
			retval.type = HT_FOUR_OF_A_KIND;
			retval.primaryCard = GetCardPtr(CLUBS, iter->first);
			retval.secondaryCard = nullptr;
			return retval;
		}
	}

	return HANDVALUE_NO_HAND;
}

const HandValue Hand::GetFullHouseValue() const {

	map<int, int > occurs = {
		{ DIAMONDS, 0 },
		{ CLUBS, 0 },
		{ SPADES, 0 },
		{ HEARTS, 0 },
	};

	for (const Card& card : m_hand)
		occurs[card.GetValue()]++;

	int threes_value = -1;
	int twos_value = -1;

	//iterate backwards to get the highest match first.
	for (auto iter = occurs.rbegin(); iter != occurs.rend(); ++iter) {
		if (iter->second >= 3 && threes_value == -1)
			threes_value = iter->first;
		else if (iter->second >= 2 && twos_value == -1)
			twos_value = iter->first;
	}

	if (threes_value != -1 && twos_value != -1) {
		HandValue retval;
		retval.type = HT_FULL_HOUSE;

		//resolve best of primary value. (must be holding one of these two cards.
		retval.primaryCard = GetCardPtrAnySuit(threes_value);
		retval.secondaryCard = GetCardPtrAnySuit(twos_value);

		return retval;
	} else
		return HANDVALUE_NO_HAND;
}

const HandValue Hand::GetFlushValue() const {

	map<int, int > occur = {
		{ CLUBS, 0 },
		{ DIAMONDS, 0},
		{ HEARTS, 0},
		{ SPADES, 0}
	};

	//count cards by suit
	for (const Card& card : m_hand)
		occur[card.GetSuit()]++;

	const Card* best_card = nullptr;

	for (int suit = 0; suit < NUM_SUITS; suit++) {
		//flush found!
		if (occur[suit] >= 5) {
			//find highest card of that suit
			for (const Card& card : m_hand) {
				if (card.GetSuit() == suit) {
					if (best_card == nullptr || best_card->GetValue() < card.GetValue())
						best_card = &card;
				}
			}

			assert(best_card != nullptr);

			HandValue retval;
			retval.type = HT_FLUSH;
			retval.primaryCard = best_card;
			retval.secondaryCard = nullptr;
			return retval;

		}
	}

	return HANDVALUE_NO_HAND;

}

const HandValue Hand::GetStraightValue() const {

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
	for (auto iter = occur.begin(); iter != occur.end(); ++iter) {

		if (iter->second == 0) {
			seq = 0;
		} else {
			seq++;
			if (seq >= 5)
				highestSoFar = iter->first;
		}
	}

	if (highestSoFar != -1) {
		HandValue retval;
		retval.type = HT_STRAIGHT;
		retval.primaryCard = GetCardPtrAnySuit(highestSoFar);
		retval.secondaryCard = nullptr;
		return retval;
	} else
		return HANDVALUE_NO_HAND;
}

const HandValue Hand::GetThreeOfAKindValue() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 3) {
			HandValue retval;
			retval.type = HT_THREE_OF_A_KIND;
			retval.primaryCard = GetCardPtrAnySuit(iter->first);
			retval.secondaryCard = nullptr;
			return retval;
		}
	}

	return HANDVALUE_NO_HAND;
}

const HandValue Hand::GetTwoPairValue() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	int first_pair_value = -1;

	//iterate backwards to get the highest matches first.
	for (auto iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 2) {

			if (first_pair_value == -1)
				first_pair_value = iter->first;
			else {
				HandValue retval;
				retval.type = HT_TWO_PAIR;
				retval.primaryCard = GetCardPtrAnySuit(first_pair_value);
				retval.secondaryCard = GetCardPtrAnySuit(iter->first);
				return retval;
			}

		}
	}

	return HANDVALUE_NO_HAND;
}

const HandValue Hand::GetPairValue() const {

	map<int, int> occur;

	for (int i = LOWEST_CARD_VALUE; i <= HIGHEST_CARD_VALUE; i++)
		occur[i] = 0;

	for (const Card& card : m_hand)
		occur[card.GetValue()]++;

	//iterate backwards to get the highest match first.
	for (map<int, int>::reverse_iterator iter = occur.rbegin(); iter != occur.rend(); ++iter) {
		if (iter->second == 2) {
			HandValue retval;
			retval.type = HT_PAIR;
			retval.primaryCard = GetCardPtrAnySuit(iter->first);
			retval.secondaryCard = nullptr;
			return retval;
		}
	}

	return HANDVALUE_NO_HAND;
}

//reminder: never return NO_HAND
const HandValue Hand::GetHighCardValue() const {

	const Card* best_card = nullptr;

	for (const Card& card : m_hand) {
		if (best_card == nullptr)
			best_card = &card;
		else if (best_card->GetValue() < card.GetValue())
			best_card = &card;
	}

	HandValue retval;
	retval.type = HT_HIGH_CARD;
	retval.primaryCard = best_card;
	retval.secondaryCard = nullptr;
	return retval;
}

int hand_compare(const Hand* hand1, const Hand* hand2) {

	const HandValue& score1 = hand1->GetBestHandValue();
	const HandValue& score2 = hand2->GetBestHandValue();

	if (score1.type > score2.type)
		return -1;
	else if (score1.type < score2.type)
		return 1;

	//same type of hand (both straight or flush or something, compare cards!)
	if (score1.primaryCard->GetValue() > score2.primaryCard->GetValue())
		return -1;
	else if (score1.primaryCard->GetValue() < score2.primaryCard->GetValue())
		return 1;

	if (score1.type == HT_FULL_HOUSE || score1.type == HT_TWO_PAIR) {
		if (score1.secondaryCard->GetValue() > score2.secondaryCard->GetValue())
			return -1;
		else if (score1.secondaryCard->GetValue() < score2.secondaryCard->GetValue())
			return 1;
	}

	if (score1.primaryCard->GetSuit() > score2.primaryCard->GetSuit())
		return -1;
	else if (score1.primaryCard->GetSuit() < score2.primaryCard->GetSuit())
		return -1;

	if (score1.type == HT_FULL_HOUSE || score1.type == HT_TWO_PAIR) {
		if (score1.secondaryCard->GetSuit() > score2.secondaryCard->GetSuit())
			return -1;
		else if (score1.secondaryCard->GetSuit() < score2.secondaryCard->GetSuit())
			return 1;
	}

	return 0; //exact same hand value
}

/*
	 * THIS IS INACCURATE TO A _TRUE_ IMPLEMENTATION
	 * RESOLVE SCORES...
	 *
	 * HT_HIGH_CARD    - assume equal
	HT_PAIR,           - assume equal
	HT_TWO_PAIR,       - look to second pair
	HT_STRAIGHT        - assume equal (multi suit straight impossible)
	HT_THREE_OF_A_KIND - assume equal
	HT_FLUSH,          - use flush suit
	HT_FULL_HOUSE,     - look to second pair
	HT_FOUR_OF_A_KIND, - assume equal (multi suit four of a kind impossible)
	HT_STRAIGHT_FLUSH  - look to suit
*/

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
	os << hand.ToString();
	return os;
}

}
