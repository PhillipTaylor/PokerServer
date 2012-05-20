/*
 * CallBot.h
 *
 * CallBot is a fake Poker player that blindly calls any bet it's faced with
 * causing it to go into a showdown at the end of a game of Poker if it has
 * enough money. This concrete player implementation is primary used in the test suite
 * for simulating games in a predictable manor.
 */

#ifndef CALLBOTPLAYER_H_
#define CALLBOTPLAYER_H_

#include "core/AbstractPlayer.h"
#include "core/Money.h"

using namespace GameEngineCore;

namespace ConcretePlayers {

class CallBotPlayer: public GameEngineCore::AbstractPlayer {
	public:
		CallBotPlayer();

		bool IsPlaying();
		std::string GetName();
		Money GetTotalBalance();
		void SetTotalBalance(Money new_value);

		void DealerAnnounce(std::string dealer);
		void SmallBlindAnnounce(std::string payer, Money amount);
		void BigBlindAnnounce(std::string payer, Money amount);
		void OpponentMoneyUpdate(std::string player, Money pot, Money bank);
		void OpponentCardAnnounce(std::string player, const Hand& players_hand);
		void WinnerAnnounce(std::string player, Money winnings);

		void ChatMessage(std::string sender, std::string message);
		void CardDealt(const Hand& hand, const Card& new_card);
		GameChoice MakeChoice(Money minimum_bid);

		~CallBotPlayer() {}

	private:
		Money m_total_balance;
		std::string m_name;
};

} /* namespace ConcretePlayers */
#endif /* CALLBOTPLAYER_H_ */
