/*
 * GameEngine.cpp
 *
 */

#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>

#include "GameEngine.h"

using namespace PokerUtils::Logger;

using std::vector;
using std::string;
using std::ostream;
using std::random_shuffle;

namespace GameEngineCore {

GameEngine::GameEngine(const vector<AbstractPlayer*>& players_impl) :
	players(),
	table_cards(),
	game_deck(),
	small_blind(2),
	big_blind(4),
	dealer(0),
	rounds_played(0)
{
	players.reserve(players_impl.size());

	log_info << "Game Engine Initialised (" << players_impl.size() << " players)\n";

	for (auto iter : players_impl) {
		if (iter->GetTotalBalance() > MONEY_ZERO) {
			GamePlayer gp(*iter);
			players.push_back(gp);
			log_debug << "Player accepted into game: "
				<< gp.GetName() << " with money " << gp.GetTotalBalance() << "\n";
		}
	}

	assert(players.size() >= 2); //at least two players

}

string GameEngine::ToString() const {
	return "Game Engine Object";
}

void GameEngine::PlayGame() {

	AssignDealer();

	DealOutBothPlayerCards();
	LogInternals();

	RoundOfBetting(true);
	LogInternals();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	RoundOfBetting();
	LogInternals();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	DealTableCard(); //turn

	RoundOfBetting();
	LogInternals();

	if (PlayersRemaining() == 1) {
		log_info << "Only one player remains\n";
		SettleGame();
		return;
	}

	DealTableCard(); //river

	RoundOfBetting();
	LogInternals();

	if (PlayersRemaining() == 1) {
		SettleGame();
		return;
	}

	log_info << "Time for a showdown!!\n";
	CompareHands();
	LogInternals();
	SettleGame();
}

bool GameEngine::NextPlayer(unsigned int first_player) {
	unsigned int old_curr = curr_player;

	NextPlayer();
	assert(old_curr != curr_player);

	if (curr_player > old_curr && first_player > old_curr && first_player <= curr_player)
		return true;
	else if (curr_player < old_curr) {
		//loop wrap around occurred.
		if (first_player >= 0 && first_player <= curr_player)
			return true;
		else if (first_player >= old_curr)
			return true;
		else
			return false;
	} else
		return false;
}

void GameEngine::NextPlayer() { //These values have defaults defined.

	++curr_player;
	if (curr_player == players.size())
		curr_player = 0;

	unsigned int tmp;

	for (tmp = curr_player; tmp < players.size(); tmp++) {

		if (players[tmp].IsPlaying()) {
			curr_player = tmp;
			return;
		}
	}

	//got to end of list, start again at beginning

	for (tmp = 0; tmp < players.size(); tmp++) {

		if (players[tmp].IsPlaying()) {
			curr_player = tmp;
			return;
		}
	}

}

void GameEngine::AssignDealer() {

	dealer = 0;

	log_info << "Dealer: " << players[dealer].GetName() << "\n";

	//Announce the dealer
	for (GamePlayer& ply : players)
		ply.DealerAnnounce(players[dealer].GetName());

}

void GameEngine::DealOutBothPlayerCards() {
	DealOutSingleRoundOfCards();
	DealOutSingleRoundOfCards();
}

void GameEngine::DealOutSingleRoundOfCards() {

	//Start to the player next to the dealer
	curr_player = dealer;
	NextPlayer();

	game_deck.Burn();
	log_info << "Card Burnt\n";

	unsigned int first_player = curr_player;

	do {

		const Card card = game_deck.Top();
		players[curr_player].CardDealt(card);

		NextPlayer();
	} while (curr_player != first_player);
}

Money GameEngine::CurrentMinimumBid() {
	Money max_equal = big_blind;

	for (GamePlayer& ply : players) {
		Money ply_pot = ply.GetPot();
		if (ply_pot > max_equal)
			max_equal = ply_pot;
	}

	log_debug << "minimum bid at beginning of round: " << max_equal << "\n";
	return max_equal;
}

void GameEngine::RoundOfBetting(bool take_blinds) {

	++rounds_played;

	log_info << "Starting round of betting\n";
	//start one next to the dealer
	curr_player = dealer;
	NextPlayer();

	unsigned int first_player = curr_player;

	if (take_blinds) {
		players[curr_player].PayPot(small_blind);
		NextPlayer();
		players[curr_player].PayPot(big_blind);
		curr_player = first_player; //let them have their goes.
	}

	//track what people are required to be chipping in!
	Money minimum_bid = CurrentMinimumBid();

	assert(curr_player != NO_NEXT_PLAYER);

	bool entire_pass = false; //every player has had a chance to bet
	bool exit_loop;

	do {

		GameChoice gc = players[curr_player].MakeChoice(minimum_bid);

		if (gc.choice == RAISE)
			minimum_bid = gc.value;

		if (gc.choice == FOLD && PlayersRemaining() == 1)
			break;

		entire_pass = NextPlayer(first_player);

		log_debug << "NEXT TURN SELECTED: " << players[curr_player].GetName() << " (min bid: " << minimum_bid << ")\n";
		log_debug << "FIRST PASS: " << entire_pass << ", All Pots Even?: " << AllPotsEven() << "\n";

		exit_loop = (entire_pass && AllPotsEven());

	} while (!exit_loop);

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
		if (ply.IsPlaying()) {
			if (first) {
				val = ply.GetPot();
				first = false;
			} else if (ply.GetPot() != val && !ply.IsAllIn())
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

	Money total_collected = MONEY_ZERO;
	int winners = 0;

	for (GamePlayer& ply : players) {
		total_collected += ply.GetPot();
		ply.SetPot(MONEY_ZERO);

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


void GameEngine::LogInternals() {
#ifdef BUILD_DEBUG

	log_debug << "******* ROUND: " << rounds_played << "*****\n";
	log_debug << "Small Blind: " << small_blind;
	log_debug << ", Big Blind" << big_blind << "\n";

	for (GamePlayer& ply : players) {
		log_debug << "Player: " << ply.GetName();
		log_debug << ",POT:" << ply.GetPot();
		log_debug << ",BALANCE: " << ply.GetTotalBalance();

		if (ply.IsPlaying()) {
			log_debug << ",HAND: " << ply.GetHand()->ToString() << " - " << ply.GetHand()->GetHandTextualDescription();
		} else {
			log_debug << " --- Folded ---";
		}

		log_debug << "\n";
	}

	log_debug << "************\n";

#endif
}

Money GameEngine::GetTotalPot() {

	Money sum = MONEY_ZERO;

	for (GamePlayer& ply : players)
		sum += ply.GetPot();

	return sum;
}

ostream& operator<<(ostream& os, const GameEngine& ge) {
	os << ge.ToString();
	return os;
}

}
