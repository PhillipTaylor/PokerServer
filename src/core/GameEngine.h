/*
 * GameEngine.h
 *
 * GameEngine basically captures most of the business logic for a game of Poker.
 * A game is instantiated with a set of players, who are dealt cards, asked to bet call or
 * raise and generally participate in a game. The function PlayGame doesn't yield until the
 * end of a game.
 */

#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <string>
#include <vector>
#include <ostream>
#include <functional>
#include <assert.h>
#include <list>
#include "Card.h"
#include "Deck.h"
#include "Money.h"
#include "GamePlayer.h"
#include "AbstractPlayer.h"
#include "poker_utils/Logger.h"

namespace GameEngineCore {

const unsigned int NO_NEXT_PLAYER = -1;

class GameEngine {
	public:
		explicit GameEngine(const std::vector<AbstractPlayer*>& players_impl);
		void PlayGame(); //This doesn't yield until the end of the game.

		std::string ToString() const;
	private:
		std::vector<GamePlayer> players;
		std::list<Card> table_cards;
		Deck game_deck;
		Money small_blind;
		Money big_blind;
		unsigned int dealer;
		unsigned int curr_player;
		int rounds_played;

		void NextPlayer();
		bool NextPlayer(unsigned int first_player); //given first_player returns if it was passed.
		Money GetTotalPot();

		void AssignDealer();
		Money CurrentMinimumBid();
		void DealOutBothPlayerCards();
		void DealOutSingleRoundOfCards();
		void RoundOfBetting(bool take_blinds = false);
		void DealFlop();
		void DealTableCard(bool burn = true); //for twist and river
		bool AllPotsEven(); //spin round of betting until all pots even or players are out.
		int PlayersRemaining(); //last player is winner!
		void CompareHands();

		//Settle Game scores remaining players hands, assigns winnings and tells
		//everyone who won.
		void SettleGame(); //no support for split pots yet.
		void LogInternals();
};

std::ostream& operator<<(std::ostream& os, const GameEngine& ge);

}

#endif /* GAMEENGINE_H_ */
