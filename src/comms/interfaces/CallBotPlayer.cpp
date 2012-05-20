/*
 * CallBot.cpp
 *
 */

#include "CallBotPlayer.h"
#include "poker_utils/CommonFunctions.h"

namespace ConcretePlayers {

using std::string;
using std::stringstream;

static int bot_index = 0;

CallBotPlayer::CallBotPlayer() :
	m_total_balance(MONEY_ZERO)
{
	++bot_index;
	m_name = "Call Bot #" + PokerUtils::AutoToString(bot_index);

}

bool CallBotPlayer::IsPlaying() {
	return true;
}

string CallBotPlayer::GetName() {
	return m_name;
}
Money CallBotPlayer::GetTotalBalance() {
	return m_total_balance;
}

void CallBotPlayer::SetTotalBalance(Money new_value) {
	m_total_balance += new_value;
}

void CallBotPlayer::DealerAnnounce(std::string dealer) {}
void CallBotPlayer::SmallBlindAnnounce(std::string payer, Money amount) {}
void CallBotPlayer::BigBlindAnnounce(std::string payer, Money amount) {}
void CallBotPlayer::OpponentMoneyUpdate(std::string player, Money pot, Money bank) {}
void CallBotPlayer::OpponentCardAnnounce(std::string player, const Hand& players_hand) {}
void CallBotPlayer::WinnerAnnounce(std::string player, Money winnings) {}
void CallBotPlayer::ChatMessage(std::string sender, std::string message) {}
void CallBotPlayer::CardDealt(const Hand& hand, const Card& new_card) {}

GameChoice CallBotPlayer::MakeChoice(Money minimum_bid) {

	GameChoice gc;
	gc.choice = CALL;
	return gc;
}

} /* namespace ConcretePlayers */
