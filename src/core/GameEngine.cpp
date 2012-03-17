/*
 * GameEngine.cpp
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>

#include "GameEngine.h"

using namespace PokerUtils::Logger;

using std::vector;
using std::reference_wrapper;
using std::string;
using std::ostream;
using std::random_shuffle;
using std::for_each;

namespace GameEngineCore {

//to make bool values to NextPlayer more readable
const bool INCR = true;
const bool NO_INCR = false;

const bool LOOP = true;
const bool NO_LOOP = false;

GameEngine::GameEngine(const vector<AbstractPlayer*>& players_impl) :
	players(),
	table_cards(),
	game_deck(),
	small_blind(2),
	big_blind(4),
	dealer(0)
{
	players.reserve(players_impl.size());

	log_info << "Game Engine Initialised (" << players_impl.size() << " players)\n";

	for (auto iter : players_impl) {
		GamePlayer gp(*iter);
		players.push_back(gp);
	}

	assert(players.size() >= 2); //at least two players

	//shuffle the players positions for extra randomness
	//random_shuffle(players.begin(), players.end()); this is wrong thing to do.
}

string GameEngine::ToString() const {
	return "Game Engine Object";
}

void GameEngine::PlayGame() {

	AssignDealer();

	TakeSmallBlinds();
	TakeBigBlinds();

	DealOutBothPlayerCards();

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	DealFlop();

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	DealTableCard(); //turn

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	DealTableCard(); //river

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	log_info << "Time for a showdown!!";
	CompareHands();
	SettleGame();
}

/*
 * This works, but you need to understand how, and when it's
 * used so it doesn't "bite" you. Weird edges around looping
 * semantics make it difficult for me to think of a better
 * algorithm off the top of my head.
*/

int GameEngine::NextPlayer(bool increment, bool loop) { //These values have defaults defined.

	if (increment) {
		++curr_player;
		if (curr_player == players.size())
			curr_player = 0;
	}

	unsigned int tmp;

	for (tmp = curr_player; tmp < players.size(); tmp++) {
		if (players[tmp].IsPlaying())
			return tmp;
		log_debug << "....1: " << tmp << ", " << curr_player << "\n";
	}

	//got to end of list, start again at beginning

	unsigned int end_claus;

	if (loop)
		end_claus = players.size();
	else
		end_claus = curr_player;

	for (tmp = 0; tmp < end_claus; tmp++) {
		if (players[tmp].IsPlaying())
			return tmp;
		log_debug << "....2\n";
	}

	return NO_NEXT_PLAYER;
}

void GameEngine::AssignDealer() {

	curr_player = 0;
	dealer = NextPlayer(NO_INCR, NO_LOOP);

	assert(dealer != NO_NEXT_PLAYER);

	log_info << "Dealer: " << players[dealer].GetName() << "\n";

	//Announce the dealer
	for (GamePlayer& ply : players)
		ply.DealerAnnounce(players[dealer].GetName());

}

void GameEngine::TakeSmallBlinds() {

	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP);
	assert(dealer != NO_NEXT_PLAYER);

	Money sb_balance = players[curr_player].GetTotalBalance();
	sb_balance -= small_blind;
	players[curr_player].SetTotalBalance(sb_balance);
	players[curr_player].SetPot(sb_balance);

	log_info <<
		"Taking small blinds (blinds: " << small_blind <<
		", player: " << players[curr_player].GetName() << "\n";

	for (GamePlayer& ply : players)
		ply.SmallBlindAnnounce(players[curr_player].GetName(), small_blind);

}

void GameEngine::TakeBigBlinds() {

	// Two left of the dealer is absolute big blind.
	// Don't rely on curr_player iterator / function invocation
	// sequence to get the correct player here. Only the dealer is accurate.
	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP);
	curr_player = NextPlayer(INCR, LOOP);

	Money bb_balance = players[curr_player].GetTotalBalance();
	bb_balance -= big_blind;
	players[curr_player].SetTotalBalance(bb_balance);
	players[curr_player].SetPot(bb_balance);

	log_info <<
		"Taking small blinds (blinds: " << small_blind <<
		", player: " << players[curr_player].GetName() << "\n";

	for (GamePlayer& ply : players)
		ply.BigBlindAnnounce(players[curr_player].GetName(), big_blind);

}

void GameEngine::DealOutBothPlayerCards() {
	DealOutSingleRoundOfCards();
	DealOutSingleRoundOfCards();
}

void GameEngine::DealOutSingleRoundOfCards() {

	//Start to the player next to the dealer
	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP); //will hit same player as small blind

	game_deck.Burn();
	log_info << "Card Burnt\n";

	unsigned int first_player = curr_player;

	do {

		const Card card = game_deck.Top();
		players[curr_player].CardDealt(card);

		curr_player = NextPlayer(INCR, LOOP);
	} while (curr_player != first_player);
}

Money GameEngine::CurrentMinimumBid() {
	Money max_equal = big_blind;

	for (GamePlayer& ply : players) {
		Money ply_pot = ply.GetPot();
		if (ply_pot > max_equal)
			max_equal = ply_pot;
	}

	return max_equal;
}

void GameEngine::RoundOfBetting() {

	//track what people are required to be chipping in!
	Money minimum_bid = CurrentMinimumBid();

	log_info << "Starting round of betting (current min bid: " << minimum_bid << ")\n";
	//start one next to the dealer
	curr_player = dealer;
	curr_player = NextPlayer(INCR, NO_LOOP);

	assert(curr_player != NO_NEXT_PLAYER);

	unsigned int first_player = curr_player;

	bool entire_pass = false;

	do {

		GameChoice gc = players[curr_player].MakeChoice(minimum_bid);

		if (gc.choice == RAISE)
			minimum_bid = gc.value;

		if (gc.choice == FOLD && PlayersRemaining() == 1)
			break;

		curr_player = NextPlayer(INCR, LOOP);

		if (curr_player == first_player)
			entire_pass = true;

	} while (!entire_pass && !AllPotsEven());

}

void GameEngine::DealFlop() {

	log_info << "Dealing Flop\n";
	DealTableCard(true);
	DealTableCard(false);
	DealTableCard(false);

}

void GameEngine::DealTableCard(bool burn) {

	if (burn) {
		game_deck.Burn();
		log_info << "Burning Card\n";
	}

	Card card = game_deck.Top();

	table_cards.push_back(card);

	log_info << "Card Dealt: " << card << "\n";

	//a copy of the table cards go into every players hands
	//so a hand can contain the best five of all seven available
	for (GamePlayer& ply : players)
		ply.CardDealt(card);

}

bool GameEngine::AllPotsEven() {

	bool first = true;
	Money val;

	for (GamePlayer& ply : players) {
		if (ply.IsPlaying() && !ply.IsAllIn()) {
			if (first) {
				val = ply.GetPot();
				first = false;
			} else if (ply.GetPot() != val)
				return false;
		}
	}

	return true;

}

int GameEngine::PlayersRemaining() {

	int count = 0;

	for (GamePlayer& ply : players) {
		if (ply.IsPlaying())
			count++;
	}

	return count;
}

void GameEngine::CompareHands() {

	//Ok...the moment of truth for our players...
	//Show the cards to everyone!


	// First loop: get best hand possible
	// Second loop: automatically fold anyone who doesn't have the same hand.
	const Hand* best_hand = nullptr;

	for (GamePlayer& ply : players) {
		if (ply.IsPlaying()) {
			if (best_hand == nullptr) {
				best_hand = ply.GetHand();
			} else {
				int retval = hand_compare(best_hand, ply.GetHand());

				if (retval == 1)
					best_hand = ply.GetHand();
			}
		}

		for (GamePlayer& observer : players) {
			Hand copy(*ply.GetHand());
			observer.OpponentCardAnnounce(ply.GetName(), copy);
		}
	}

	int winners = 0;

	for (GamePlayer& ply : players) {

		if (ply.IsPlaying()) {

			int retval = hand_compare(best_hand, ply.GetHand());

			//assert(retval != 1);
			if (retval == 1) {
				log_debug << "best possible hand " << best_hand->ToString() << " beaten by " << ply.GetHand()->ToString() << "\n";
			}

			if (retval != 0) {
				ply.Fold();
			} else
				winners++;
		}
	}

}

void GameEngine::SettleGame() {

	//Pay the winners

	Money total_collected = 0;
	int winners = 0;

	for (GamePlayer& ply : players) {
		total_collected += ply.GetPot();
		ply.SetPot(0);

		if (ply.IsPlaying())
			++winners;
	}

	Money winnings = (Money)((float)total_collected / winners);
	log_info << "sharing winnings of " << winnings << " between " << winners << " winners\n";

	//give the victors the spoils!
	for (GamePlayer& ply : players) {
		if (ply.IsPlaying()) {

			Money balance = ply.GetTotalBalance();
			balance += winnings;
			ply.SetTotalBalance(balance);

			for (GamePlayer& observer : players)
				observer.WinnerAnnounce(ply.GetName(), winnings);
		}
	}

}

Money GameEngine::GetTotalPot() {

	Money sum = 0;

	for (GamePlayer& ply : players)
		sum += ply.GetPot();

	return sum;
}

ostream& operator<<(ostream& os, const GameEngine& ge) {
	os << ge.ToString();
	return os;
}

}
