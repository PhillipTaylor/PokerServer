/*
 * GameEngineRandom.cpp
 *
 *  Created on: 25 Mar 2012
 *      Author: phill
 */

#include "../core/Card.h"
#include "../core/Deck.h"
#include "../core/Hand.h"
#include "../core/AbstractPlayer.h"
#include "../comms/RandomBotPlayer.h"
#include "../core/GameEngine.h"
#include <vector>
#include <iostream>

using namespace GameEngineCore;
using namespace ConcretePlayers;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace poker_tests {

void game_engine_random_testing() {

	//put into a vector
	RandomBotPlayer rbp_allocs[10]; //actually have on the stack.
	vector<AbstractPlayer*> players;

	for (int i = 0; i < 10; i++) {
		rbp_allocs[i].SetTotalBalance(40);
		players.push_back(&rbp_allocs[i]);
	}

	GameEngine ge(players);
	ge.PlayGame();
}

}
