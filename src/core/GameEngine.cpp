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

using std::endl;
using std::cout;

#include "GameEngine.h"

using std::vector;
using std::reference_wrapper;
using std::string;
using std::ostream;
using std::random_shuffle;
using std::for_each;

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

	cout << "finished r 1" << endl;
	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	DealFlop();

	cout << "finished r 2" << endl;
	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	DealTableCard(); //turn
	cout << "finished r 3" << endl;

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	cout << "finished r 4" << endl;
	DealTableCard(); //river

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	cout << "finished r 5" << endl;
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
	}

	return NO_NEXT_PLAYER;
}

void GameEngine::AssignDealer() {

	curr_player = 0;
	dealer = NextPlayer(NO_INCR, NO_LOOP);

	assert(dealer != NO_NEXT_PLAYER);

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

	unsigned int first_player = curr_player;

	do {

		const Card card = game_deck.Top();
		players[curr_player].CardDealt(card);

		curr_player = NextPlayer(INCR, LOOP);
	} while (curr_player != first_player);
}

void GameEngine::RoundOfBetting() {

	//start one next to the dealer
	curr_player = dealer;
	curr_player = NextPlayer(INCR, NO_LOOP);

	assert(curr_player != NO_NEXT_PLAYER);

	unsigned int first_player = curr_player;

	//track what people are required to be chipping in!
	Money minimum_bid = big_blind;
	bool entire_pass = false;

	do {

		GameChoice gc = players[curr_player].MakeChoice(minimum_bid);

		if (gc.choice == RAISE)
			minimum_bid = gc.value;

		curr_player = NextPlayer(INCR, LOOP);

		if (curr_player == first_player)
			entire_pass = true;

	} while (!entire_pass && !AllPotsEven());

}

void GameEngine::DealFlop() {

	cout << "1" << endl;
	DealTableCard(true);
	cout << "2" << endl;
	DealTableCard(false);
	cout << "3" << endl;
	DealTableCard(false);

}

void GameEngine::DealTableCard(bool burn) {

	if (burn)
		game_deck.Burn();

	Card card = game_deck.Top();

	table_cards.push_back(card);

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

	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP);

	unsigned int first_player = curr_player;

	do {

		for (GamePlayer& observer : players) {
			observer.OpponentCardAnnounce(
				players[curr_player].GetName(),
				players[curr_player].GetHand()
			);
		}

		curr_player = NextPlayer(INCR, LOOP);

	} while (curr_player != first_player);

	//Get the Winner!
	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP);
	first_player = curr_player;

	//first player defaulted to "best" player.
	unsigned int best_player = curr_player;

	curr_player = NextPlayer(INCR, LOOP);

	do {

		if (hand_compare(
			players[curr_player].GetHand(),
			players[best_player].GetHand()
		)) {
			best_player = curr_player;
		}

		curr_player = NextPlayer(INCR, LOOP);

	} while (curr_player != first_player);

	//fold all losers automatically

	curr_player = dealer;
	curr_player = NextPlayer(INCR, LOOP);

	do {

		if (best_player != curr_player)
			players[curr_player].Fold();

		curr_player = NextPlayer(INCR, LOOP);

	} while (curr_player != first_player);

}

void GameEngine::SettleGame() {

	//Only one player left! Give them the spoils!

	Money total_collected = 0;

	GamePlayer* winner;

	for (GamePlayer& ply : players) {
		total_collected = ply.GetPot();
		ply.SetPot(0);

		if (ply.IsPlaying())
			winner = &ply;
	}

	//give the victor the spoils!
	Money total_winner_balance = winner->GetTotalBalance();
	total_winner_balance += total_collected;
	winner->SetTotalBalance(total_winner_balance);

	for (GamePlayer& ply : players)
		ply.WinnerAnnounce(winner->GetName(), total_collected);
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


