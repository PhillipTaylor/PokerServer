/*
 * AbstractPlayer.h
 *
 * The AbstractPlayer class is an abstract virtual class that provides a generic interface
 * for the GameEngine and allows it to communicate with players in different ways. For example
 * the GameEngine does not need to concern itself with whether the player is playing via the Console,
 * a network connection or is a bot used in testing. Example subclasses of AbstractPlayer are
 * "RandomBotPlayer" which makes random choices "TelnetPlayer", "ConsolePlayer" etc.
 *
 * Some player related state is kept in GamePlayer that represents a full game player and holds
 * a reference to an AbstractPlayer which purely tries to separate game interaction from communications
 * concerns.
 */

#ifndef ABSTRACTPLAYER_H_
#define ABSTRACTPLAYER_H_

#include <string>

#include "Card.h"
#include "Hand.h"
#include "Money.h"

using std::string;

namespace GameEngineCore {

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

}

#endif /* ABSTRACTPLAYER_H_ */
