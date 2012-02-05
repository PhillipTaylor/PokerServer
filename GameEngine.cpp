/*
 * GameEngine.cpp
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#include <vector>
#include <assert.h>
#include <algorithm>

#include "GameEngine.h"

using std::vector;
using std::reference_wrapper;
using std::string;
using std::ostream;
using std::random_shuffle;
using std::for_each;

GameEngine::GameEngine(const vector<AbstractPlayer*>& players_impl) :
	players(),
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

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	DealFlop();

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	DealTableCard(); //turn

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	DealTableCard(); //river

	RoundOfBetting();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

}

int GameEngine::NextPlayer(bool loop) {

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
	dealer = NextPlayer();

	assert(dealer != NO_NEXT_PLAYER);

	//Announce the dealer
	for (auto ply : players)
		ply.DealerAnnounce(players[dealer].GetName());

}

void GameEngine::TakeSmallBlinds() {

	++curr_player;
	curr_player= NextPlayer(true);
	assert(dealer != NO_NEXT_PLAYER);

	Money sb_balance = players[curr_player].GetTotalBalance();
	sb_balance -= small_blind;
	players[curr_player].SetTotalBalance(sb_balance);
	players[curr_player].SetPot(sb_balance);

	for (auto ply : players)
		ply.SmallBlindAnnounce(players[curr_player].GetName(), small_blind);

}

void GameEngine::TakeBigBlinds() {

	++curr_player;
	curr_player= NextPlayer(true);

	Money bb_balance = players[curr_player].GetTotalBalance();
	bb_balance -= big_blind;
	players[curr_player].SetTotalBalance(bb_balance);
	players[curr_player].SetPot(bb_balance);

	for (auto ply : players)
		ply.BigBlindAnnounce(players[curr_player].GetName(), big_blind);

}

void GameEngine::DealOutBothPlayerCards() {
	DealOutSingleRoundOfCards();
	DealOutSingleRoundOfCards();
}

void GameEngine::DealOutSingleRoundOfCards() {

	//Start to the player next to the dealer
	curr_player = dealer + 1;
	curr_player = NextPlayer(); //will hit same player as small blind

	assert(curr_player != NO_NEXT_PLAYER);

	game_deck.Burn();

	//THIS LOOP IS BADLY BROKEN!
	while (curr_player != dealer && curr_player != NO_NEXT_PLAYER) {

		const Card card = game_deck.Top();
		players[curr_player].CardDealt(card);

		curr_player = NextPlayer();
	}
}

void GameEngine::RoundOfBetting() {

	//start one next to the dealer
	curr_player = dealer;
	++curr_player;
	curr_player = NextPlayer();

	//track what people are required to be chipping in!
	Money minimum_bid = big_blind;

	while (curr_player != dealer && !AllPotsEven()) {

		GameChoice gc = players[curr_player].MakeChoice(minimum_bid);

		if (gc.choice == RAISE)
			minimum_bid = gc.value;

		++curr_player;
		curr_player = NextPlayer();
	}

}

void GameEngine::DealFlop() {

	DealTableCard(true);
	DealTableCard(false);
	DealTableCard(false);

}

void GameEngine::DealTableCard(bool burn) {

	if (burn)
		game_deck.Burn();

	const Card card = game_deck.Top();

	table_cards.push_back(card);

	//a copy of the table cards go into every players hands
	//so a hand can contain the best five of all seven available
	for (auto ply : players)
		ply.CardDealt(card);

}

bool GameEngine::AllPotsEven() {

	bool first = true;
	Money val;

	for (auto ply : players) {
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

	for (auto ply : players) {
		if (ply.IsPlaying())
			count++;
	}

	return count;
}

void GameEngine::SettleGame() {

}

Money GameEngine::GetTotalPot() {

	Money sum = 0;

	for_each(players.begin(), players.end(), [&sum] (GamePlayer ply) {
		sum += ply.GetPot();
	});

	return sum;
}

ostream& operator<<(ostream& os, const GameEngine& ge) {
	os << ge.ToString();
	return os;
}


