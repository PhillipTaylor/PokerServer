/*
 * ConsolePlayer.h
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#ifndef CONSOLEPLAYER_H_
#define CONSOLEPLAYER_H_

#include <string>
#include "AbstractPlayer.h"

namespace ConcretePlayers {

class ConsolePlayer: public AbstractPlayer {

	public:
		ConsolePlayer();

		std::string GetName();
		Money GetTotalBalance();
		void SetTotalBalance(Money new_value);

		void CardDealt(const Hand& hand, const Card& new_card);
		GameChoice MakeChoice(Money minimum_bid);

		~ConsolePlayer() {}

	private:
		Money m_total_balance;
		std::string m_name;

};

} /* namespace ConcretePlayers */
#endif /* CONSOLEPLAYER_H_ */
