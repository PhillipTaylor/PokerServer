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
		virtual string GetName() = 0;
		virtual Money GetTotalBalance() = 0;
		virtual void SetTotalBalance(Money new_value) = 0;

		virtual void CardDealt(const Hand& hand, const Card& new_card) = 0;
		virtual GameChoice MakeChoice(Money minimum_bid) = 0;

		virtual ~AbstractPlayer() {};

};

#endif /* ABSTRACTPLAYER_H_ */
