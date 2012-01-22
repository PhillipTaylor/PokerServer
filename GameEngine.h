/*
 * GameEngine.h
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include "Deck.h"
#include "Money.h"

class GameEngine {
	public:
		GameEngine();
		void playGame(); //This doesn't yield until the end of the game.
	private:
		Deck gameDeck;
};

#endif /* GAMEENGINE_H_ */
