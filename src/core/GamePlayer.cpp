/*
 * GamePlayer.cpp
 *
 */

#include <string>
#include "GamePlayer.h"
#include "poker_utils/Logger.h"

using namespace PokerUtils::Logger;
using std::string;
using std::ostream;

namespace GameEngineCore {

GamePlayer::GamePlayer(AbstractPlayer& player_impl) :
	m_player_impl(&player_impl),
	m_hand(),
	m_pot(MONEY_ZERO),
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
	log_debug << GetName() << " Card State: " << m_hand << " (" << m_hand.GetHandTextualDescription() << ")\n";
}

bool GamePlayer::IsAllIn() {
	return (IsPlaying() && m_player_impl->GetTotalBalance() == MONEY_ZERO);
}

void GamePlayer::Fold() {
	m_folded = true;
}

Money GamePlayer::PayPot(Money value) {

	Money total_balance = m_player_impl->GetTotalBalance();
	Money placed;

	if (total_balance < value) { // User does not have
		placed = total_balance;
		total_balance = MONEY_ZERO;
	} else {
		placed = value;
		total_balance -= value;
	}

	m_pot += value;
	m_player_impl->SetTotalBalance(total_balance);

	return placed;
}

GameChoice GamePlayer::MakeChoice(Money minimum_bid) {

	string player_name = m_player_impl->GetName();

	if (m_player_impl->GetTotalBalance() == MONEY_ZERO) {
		GameChoice gc;
		gc.choice = CALL;
		log_info << "TURN: " << player_name << ": called (auto from all in)\n";
		return gc;
	}

	GameChoice gc = m_player_impl->MakeChoice(minimum_bid);

	if (gc.choice == FOLD) {
		m_folded = true;
		log_info << "TURN: " << player_name << ": folded\n";
	} else if (gc.choice == CALL ) {
		//need to top up the pot with the difference.
		Money diff = (minimum_bid - m_pot);
		PayPot(diff);
		log_info << "TURN: " << player_name << ": called at " << minimum_bid << "\n";
	} else if (gc.choice == RAISE ) {

		assert(gc.value > minimum_bid);

		Money diff = (gc.value - m_pot);
		PayPot(diff);
		log_info << "TURN: " << player_name << ": raised to " << gc.value << "\n";
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

}
