/*
 * Card.cpp
 *
 *  Created on: 8 Jan 2012
 *      Author: phill
 */

#include "Card.h"

#include <assert.h>
#include "../poker_utils/CommonFunctions.h"

using namespace std;

namespace GameEngineCore {

Card::Card(const int value, const int suit) :
	m_value(value),
	m_suit(suit)
{
	assert(m_suit == HEARTS || m_suit == CLUBS || m_suit == DIAMONDS || m_suit == SPADES);
	assert(m_value >= LOWEST_CARD_VALUE && m_value <= HIGHEST_CARD_VALUE);

}

string Card::ToString(const bool compressed) const {
	if (compressed)
		return ToShortString();
	else
		return ToLongString();
}

string Card::ToShortString() const {

	string retval;

	if (m_value < 11) {
		retval = CardValueToString(m_value);
	} else if (m_value == JACK) {
		retval = "J";
	} else if (m_value == QUEEN) {
		retval = "Q";
	} else if (m_value == KING) {
		retval = "K";
	} else if (m_value == ACE) {
		retval = "A";
	}

	switch (m_suit) {
		case (HEARTS):
			retval += "H";
			break;
		case (CLUBS):
			retval += "C";
			break;
		case (DIAMONDS):
			retval += "D";
			break;
		case (SPADES):
			retval += "S";
			break;
	}

	return retval;

}

string Card::ToLongString() const {
	return CardValueToString(m_value) + " of " + CardSuitToString(m_suit);
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
	os << card.ToString();
	return os;
}

bool operator==(const Card& c1, const Card& c2) {
	return (c1.GetSuit() == c2.GetSuit() && c1.GetValue() == c2.GetValue());
}

string CardValueToString(const int value, bool expandFaceName) {

	if (expandFaceName) {

		switch (value) {
			case (JACK):
				return "Jack";
			case (QUEEN):
				return "Queen";
			case (KING):
				return "King";
			case (ACE):
				return "Ace";
			default:
				return PokerUtils::AutoToString(value);
		}
	} else
		return PokerUtils::AutoToString(value);

}

string CardSuitToString(const int suit) {
	switch (suit) {
		case (CLUBS):
			return "Clubs";
		case (SPADES):
			return "Spades";
		case (DIAMONDS):
			return "Diamonds";
		case (HEARTS):
			return "Hearts";
	}
}

}
