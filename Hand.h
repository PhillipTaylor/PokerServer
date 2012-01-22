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

const unsigned int MAX_CARDS_IN_HAND = 7;

//describes the hand.
enum HandType {
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

typedef struct HandValue {
	bool handFound;
	int firstCardValue; //e.g. set to ACE is pair of Aces
	int secondCardValue; //e.g set to 9 if full house, ACES and 9s.
	int suitValue; //e.g. HEARTS in a flush
} HandScore;

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

		const HandType GetBestHandType() const { return m_best_hand_type; }
		const HandScore& GetBestHandScore() const { return m_best_hand_score; }
		std::string GetHandTextualDescription() const;
		std::string ToString() const;

	private:
		void CalculateHandScore();

        const HandScore GetStraightFlushScore() const;
        const HandScore GetFourOfAKindScore() const;
        const HandScore GetFullHouseScore() const;
        const HandScore GetFlushScore() const;
        const HandScore GetStraightScore() const;
        const HandScore GetThreeOfAKindScore() const;
        const HandScore GetTwoPairScore() const;
        const HandScore GetPairScore() const;
        const HandScore GetHighCardScore() const;

		std::vector<Card> m_hand;

		//describes the best hand detected.
		HandType m_best_hand_type;
		HandScore m_best_hand_score;
};

std::ostream& operator<<(std::ostream& os, const Hand& hand);

#endif /* HAND_H_ */
