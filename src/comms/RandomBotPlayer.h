/*
 * RandomBotPlayer.h
 *
 *  Created on: 18 Feb 2012
 *      Author: phill
 */

#ifndef RANDOMBOTPLAYER_H_
#define RANDOMBOTPLAYER_H_

#include "../core/AbstractPlayer.h"
#include "../core/Money.h"

using namespace GameEngineCore;

namespace ConcretePlayers {

class RandomBotPlayer: public AbstractPlayer {
public:
	RandomBotPlayer();

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

	~RandomBotPlayer() {}

private:
	Money m_total_balance;
	std::string m_name;


};

} /* namespace ConcretePlayers */
#endif /* RANDOMBOTPLAYER_H_ */
