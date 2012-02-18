/*
 * ConsolePlayer.h
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#ifndef CONSOLEPLAYER_H_
#define CONSOLEPLAYER_H_

#include <string>
#include "../core/AbstractPlayer.h"

namespace ConcretePlayers {

class ConsolePlayer: public AbstractPlayer {

	public:
		ConsolePlayer();

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

		~ConsolePlayer() {}

	private:
		Money m_total_balance;
		std::string m_name;

};

} /* namespace ConcretePlayers */
#endif /* CONSOLEPLAYER_H_ */
