
#include "assert.h"
#include "../core/Card.h" 

using namespace GameEngineCore;

namespace poker_tests {

void card_testing() {

	Card c(3, CLUBS);
	
	assert(c.GetValue() == 3);
	assert(c.GetSuit() == CLUBS);
	
	Card d(KING, HEARTS);
	assert(d.GetValue() == 13);
	assert(d.GetSuit() == HEARTS);
	
}

}