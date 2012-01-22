/*
 * GamePlayer.cpp
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#include <string>
#include "GamePlayer.h"

using std::string;
using std::ostream;

GamePlayer::GamePlayer(AbstractPlayer& player_impl) :
	m_player_impl(player_impl),
	m_hand(),
	m_pot(0)
{}

void GamePlayer::SetPot(Money new_value) {
	m_pot = new_value;
}

string GamePlayer::GetName() const {
	return m_player_impl.GetName();
}

Money GamePlayer::GetTotalBalance() const {
	return m_player_impl.GetTotalBalance();
}

void GamePlayer::SetTotalBalance(Money new_value) {
	m_player_impl.SetTotalBalance(new_value);
}

void GamePlayer::CardDealt(const Card& new_card) {
	m_hand.AddCard(new_card);
	m_player_impl.CardDealt(m_hand, new_card);
}

GameChoice GamePlayer::MakeChoice(Money minimum_bid) {
	return m_player_impl.MakeChoice(minimum_bid);
}

string GamePlayer::ToString() const {
	string s(m_player_impl.GetName());
	s += " (hand: " + m_hand.ToString() + ")";
	return s;
}

ostream& operator<<(ostream& os, const GamePlayer& ply) {
	os << ply.ToString();
	return os;
}
