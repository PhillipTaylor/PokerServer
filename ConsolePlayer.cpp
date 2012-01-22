/*
 * ConsolePlayer.cpp
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#include "ConsolePlayer.h"

#include <iostream>
#include <string>

#include "Card.h"
#include "Hand.h"

using namespace::std;

namespace ConcretePlayers {

ConsolePlayer::ConsolePlayer() {

	//Ask the user for their name!
	cout << "Enter your name player: ";
	cin >> m_name;

}

std::string ConsolePlayer::GetName() {
	return m_name;
}

Money ConsolePlayer::GetTotalBalance() {
	return m_total_balance;
}

void ConsolePlayer::SetTotalBalance(Money new_value) {
	cout << "Your balance is now: " << new_value << endl;
	m_total_balance = new_value;
}

void ConsolePlayer::CardDealt(const Hand& hand, const Card& new_card) {
	cout << "You've just been dealt: " << new_card << endl;
	cout << "You now hold: " << hand << endl;
}

GameChoice ConsolePlayer::MakeChoice(Money minimum_bid) {

	cout << "*** It is your turn " << m_name << " ***" << endl;
	cout << "f = fold" << endl;
	cout << "c = call (requires: " << minimum_bid << ")" << endl;
	cout << "r = raise" << endl;

	//TODO: This class isn't going to live for long.
	//People aren't going to gather around in front a console.
	//but this could be cleaned up.

	string choice;
	cin >> choice;

	if (choice == "f") {
		GameChoice gc;
		gc.choice = FOLD;
		return gc;
	} else if (choice == "c") {
		GameChoice gc;
		gc.choice = CALL;
		gc.value = minimum_bid;
		return gc;
	} else if (choice == "r") {
		GameChoice gc;
		gc.choice = RAISE;

		Money raiseMax = (m_total_balance - minimum_bid);

		Money raise_val;

		cout << "[" << m_name << "] Raise by: (max=" << raiseMax << ")" << endl;
		cin >> raise_val;

		gc.value = raise_val;
		return gc;
	} else {
		cout << "Choice not recognised. Folding instead!";
		GameChoice gc;
		gc.choice = FOLD;
		return gc;
	}
}

} /* namespace ConcretePlayers */
