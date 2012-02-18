/*
 * Deck.cpp
 *
 *  Created on: 8 Jan 2012
 *      Author: phill
 */

#include <assert.h>
#include <algorithm>
#include <sstream>
#include "Deck.h"

using namespace std;

Deck::Deck() :
	m_deck(),
	m_pos(CARDS_IN_DECK)
{
	m_deck.reserve(CARDS_IN_DECK);

	//Generate all cards and add them to the stack
	for (int value = LOWEST_CARD_VALUE; value <= HIGHEST_CARD_VALUE; value++) {
		m_deck.push_back(Card(value, HEARTS));
		m_deck.push_back(Card(value, CLUBS));
		m_deck.push_back(Card(value, DIAMONDS));
		m_deck.push_back(Card(value, SPADES));
	}

	//Shuffle cards
	random_shuffle(m_deck.begin(), m_deck.end());
}

const Card& Deck::Top() {
	assert(m_pos > 0);

	m_pos--;
	return m_deck[m_pos];
}

void Deck::Burn() {
	assert(m_pos > 0);
	m_pos--;
}

int Deck::GetCount() const {
	return m_pos;
}

/* serialises the card order, then builds a SHA1 hash
 * so that you can prove the card orders or different.
 */
string Deck::ToHashString() const {

	string card_list;
	card_list.reserve((CARDS_IN_DECK * 2) + 1);

	for (const Card& card : m_deck)
		card_list += card.ToShortString();

	return card_list;
}

string Deck::ToString() const {
	stringstream cardcount;
	cardcount << GetCount();
	string retval = "Deck of " + cardcount.str() + " Cards";
	return retval;
}

ostream& operator<<(ostream& os, const Deck& dk) {
	os << dk.ToString();
	return os;
}

