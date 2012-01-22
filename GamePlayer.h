/*
 * GamePlayer.h
 *
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#ifndef GAMEPLAYER_H_
#define GAMEPLAYER_H_

#include <string>
#include <iostream>

#include "Hand.h"
#include "Card.h"
#include "Money.h"
#include "AbstractPlayer.h"

//Todo. double not great for currency
//because of rounding.


class GamePlayer {

	public:
		GamePlayer(AbstractPlayer& player_impl);
		const Hand& GetHand() const { return m_hand; }

		void SetPot(Money new_value);
		const Money& GetPot() const { return m_pot; }

		std::string GetName() const;
		Money GetTotalBalance() const;
		void SetTotalBalance(Money new_value);
		void CardDealt(const Card& new_card);

		GameChoice MakeChoice(Money minimum_bid);

		std::string ToString() const;

	private:
		AbstractPlayer& m_player_impl;
		Hand m_hand;
		Money m_bank;
		Money m_pot;

};

std::ostream& operator<<(std::ostream& os, const GamePlayer& ply);

#endif /* GAMEPLAYER_H_ */
