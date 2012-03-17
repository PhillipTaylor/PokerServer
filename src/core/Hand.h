/*
 * Hand.h
 *
 *  Created on: 8 Jan 2012
 *      Author: phill
 */

#ifndef HAND_H_
#define HAND_H_

#include <string>
#include <vector>
#include "Card.h"

namespace GameEngineCore {

const unsigned int MAX_CARDS_IN_HAND = 7;

//describes the hand.
enum HandType {
	HT_NO_HAND,
	HT_HIGH_CARD,
	HT_PAIR,
	HT_TWO_PAIR,
	HT_STRAIGHT,
	HT_THREE_OF_A_KIND,
	HT_FLUSH,
	HT_FULL_HOUSE,
	HT_FOUR_OF_A_KIND,
	HT_STRAIGHT_FLUSH
};

typedef struct _HandValue {
	HandType type;
	const Card* primaryCard; //e.g. most influential card (e.g. Straight Ace High)
	const Card* secondaryCard; //e.g second most influential card (e.g. Full House, Aces over Fours).
} HandValue;

class Hand {
	public:
		Hand();

		void AddCard(const Card& card); //when cards are dealt
		const Card& operator[](int position) const; //for showing cards
		size_t GetCardCount() const { return m_hand.size(); }

		/*
		 * Returns a HandScore used for determining what hand
		 * the user holds or returns NO_HAND if the user contains
		 * nothing.
		*/

		const HandValue& GetBestHandValue() const { return m_best_hand; }
		std::string GetHandTextualDescription() const;
		std::string ToString() const;

	private:
		void CalculateHandScore();
		const Card* GetCardPtr(int suit, int value) const;
		const Card* GetCardPtrAnySuit(int value) const;

        const HandValue GetStraightFlushValue() const;
        const HandValue GetFourOfAKindValue() const;
        const HandValue GetFullHouseValue() const;
        const HandValue GetFlushValue() const;
        const HandValue GetStraightValue() const;
        const HandValue GetThreeOfAKindValue() const;
        const HandValue GetTwoPairValue() const;
        const HandValue GetPairValue() const;
        const HandValue GetHighCardValue() const;

		std::vector<Card> m_hand;

		//best hand is calculated when cards added.
		HandValue m_best_hand;
};

int hand_compare(const Hand* hand1, const Hand* hand2);
std::ostream& operator<<(std::ostream& os, const Hand& hand);

}

#endif /* HAND_H_ */
