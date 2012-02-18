/*
 * AbstractPlayer.h
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#ifndef ABSTRACTPLAYER_H_
#define ABSTRACTPLAYER_H_

#include <string>

#include "Card.h"
#include "Hand.h"
#include "Money.h"

using std::string;

enum Decision {
	FOLD,
	CALL,
	RAISE
};

typedef struct GameChoice_ {
	Decision choice;
	Money value;
} GameChoice;

class AbstractPlayer {

	public:

		virtual bool IsPlaying() = 0;
		virtual string GetName() = 0;
		virtual Money GetTotalBalance() = 0;
		virtual void SetTotalBalance(Money new_value) = 0;

		virtual void DealerAnnounce(std::string dealer) = 0;
		virtual void SmallBlindAnnounce(std::string payer, Money amount) = 0;
		virtual void BigBlindAnnounce(std::string payer, Money amount) = 0;
		virtual void OpponentMoneyUpdate(std::string player, Money pot, Money bank) = 0;
		virtual void OpponentCardAnnounce(std::string player, const Hand& players_hand) = 0;
		virtual void WinnerAnnounce(std::string player, Money winnings) = 0;

		virtual void ChatMessage(std::string sender, std::string message) = 0;
		virtual void CardDealt(const Hand& hand, const Card& new_card) = 0;
		virtual GameChoice MakeChoice(Money minimum_bid) = 0;

		virtual ~AbstractPlayer() {};

};

#endif /* ABSTRACTPLAYER_H_ */
