/*
 * Money.h
 *
 * Money is a temporary placeholder for a class that's supposed to
 * represent the currency and track money and bets within the game.
 * At the moment it's typedef for a double (which isn't usually a good choice
 * for money, especially when maths are being applied, like in this situation)
 * but to allow me to develop the core engine first, this class was used, with a
 * view to making a "correct" class representing money at a later point.
 */

#ifndef MONEY_H_
#define MONEY_H_

namespace GameEngineCore {

	//Can replace this with more complex type
	//later if need be...
	typedef double Money;

	const double MONEY_ZERO = 0.0f;

}

#endif /* MONEY_H_ */
