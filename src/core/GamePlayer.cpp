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

#include <iostream>
using std::cout; using std::endl;

GamePlayer::GamePlayer(AbstractPlayer& player_impl) :
	m_player_impl(&player_impl),
	m_hand(),
	m_pot(0),
	m_folded(false)
{}

GamePlayer::GamePlayer(const GamePlayer& gp) :
	m_player_impl(gp.m_player_impl),
	m_hand(gp.m_hand),
	m_pot(gp.m_pot),
	m_folded(false)
{}

GamePlayer GamePlayer::operator =(const GamePlayer& gp) {
	m_player_impl = gp.m_player_impl; //no deep copy required.
	m_hand = gp.m_hand;
	m_pot = gp.m_pot;
	m_folded = gp.m_folded;
	return *this;
}

void GamePlayer::SetPot(Money new_value) {
	m_pot = new_value;
}

void GamePlayer::DealerAnnounce(std::string dealer) {
	m_player_impl->DealerAnnounce(dealer);
}

void GamePlayer::SmallBlindAnnounce(std::string payer, Money amount) {
	m_player_impl->SmallBlindAnnounce(payer, amount);
}

void GamePlayer::BigBlindAnnounce(std::string payer, Money amount) {
	m_player_impl->BigBlindAnnounce(payer, amount);
}

void GamePlayer::OpponentMoneyUpdate(std::string player, Money pot, Money bank) {
	m_player_impl->OpponentMoneyUpdate(player, pot, bank);
}

void GamePlayer::OpponentCardAnnounce(std::string player, const Hand& players_hand) {
	m_player_impl->OpponentCardAnnounce(player, players_hand);
}

void GamePlayer::WinnerAnnounce(std::string player, Money winnings) {
	m_player_impl->WinnerAnnounce(player, winnings);
}

bool GamePlayer::IsPlaying() const {
	bool concrete_answer = m_player_impl->IsPlaying();
	return (concrete_answer && !m_folded);
}

string GamePlayer::GetName() const {
	return m_player_impl->GetName();
}

Money GamePlayer::GetTotalBalance() const {
	return m_player_impl->GetTotalBalance();
}

void GamePlayer::SetTotalBalance(Money new_value) {
	m_player_impl->SetTotalBalance(new_value);
}

void GamePlayer::CardDealt(const Card& new_card) {
	m_hand.AddCard(new_card);
	m_player_impl->CardDealt(m_hand, new_card);
	cout << "HAND COUNT: " << m_hand.GetCardCount() << endl;
}

bool GamePlayer::IsAllIn() {
	return (IsPlaying() && m_player_impl->GetTotalBalance() == 0);
}

void GamePlayer::Fold() {
	m_folded = true;
}

Money GamePlayer::PayPot(Money value) {

	Money total_pot = m_player_impl->GetTotalBalance();
	Money placed;

	if (total_pot < value) { // USER DOES NOT HAVE THE CASH!
		                     // TODO: Add support for split pots
		placed = total_pot;
		total_pot = 0;
	} else {
		placed = value;
		total_pot -= value;
	}

	m_pot += value;
	m_player_impl->SetTotalBalance(total_pot);

	return placed;
}

GameChoice GamePlayer::MakeChoice(Money minimum_bid) {

	GameChoice gc = m_player_impl->MakeChoice(minimum_bid);

	if (gc.choice == FOLD)
		m_folded = true;
	else if (gc.choice == CALL ) {
		//need to top up the pot with the difference.
		Money diff = (minimum_bid - m_pot);
		PayPot(diff);
	} else if (gc.choice == RAISE ) {

		assert(gc.value > minimum_bid);

		Money diff = (gc.value - m_pot);
		PayPot(diff);
	}

	return gc;
}

string GamePlayer::ToString() const {
	string s(m_player_impl->GetName());
	s += " (hand: " + m_hand.ToString() + ")";
	return s;
}

ostream& operator<<(ostream& os, const GamePlayer& ply) {
	os << ply.ToString();
	return os;
}
