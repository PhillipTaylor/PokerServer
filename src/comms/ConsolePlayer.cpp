/*
 * ConsolePlayer.cpp
 *
 *  Created on: 22 Jan 2012
 *      Author: phill
 */

#include "ConsolePlayer.h"

#include <iostream>
#include <string>

#include "../core/Card.h"
#include "../core/Hand.h"

using namespace::std;

namespace ConcretePlayers {

ConsolePlayer::ConsolePlayer() :
	m_total_balance(0)
{

	//Ask the user for their name!
	cout << "Enter your name player: ";
	cin >> m_name;

}

bool ConsolePlayer::IsPlaying() {
	return true;
}

std::string ConsolePlayer::GetName() {
	return m_name;
}

Money ConsolePlayer::GetTotalBalance() {
	return m_total_balance;
}

void ConsolePlayer::SetTotalBalance(Money new_value) {
	cout << m_name <<": Your balance is now: " << new_value << endl;
	m_total_balance = new_value;
}

void ConsolePlayer::DealerAnnounce(std::string dealer) {
	cout << m_name << ": The dealer is " << dealer << endl;
}

void ConsolePlayer::SmallBlindAnnounce(std::string payer, Money amount) {
	cout << m_name << ": Small blind of " << amount << " paid by " << payer << endl;
}

void ConsolePlayer::BigBlindAnnounce(std::string payer, Money amount) {
	cout << m_name << ": Big bling of " << amount << " paid by " << payer << endl;
}

void ConsolePlayer::OpponentMoneyUpdate(std::string player, Money pot, Money bank) {
	cout << m_name << ": Balance Update: " << player << " now has " << bank << " in bank and " << pot << " in pot" << endl;
}

void ConsolePlayer::OpponentCardAnnounce(std::string player, const Hand& players_hand) {
	cout << m_name << ": " << player << " is holding: " << players_hand.GetHandTextualDescription() << endl;
}

void ConsolePlayer::WinnerAnnounce(std::string player, Money winnings) {
	cout << m_name << "Congratulations to " << player << " for winning " << winnings << endl;
}

void ConsolePlayer::ChatMessage(std::string sender, std::string message) {
	cout << m_name << "CHAT: " << sender << ": " << message << endl;
}

void ConsolePlayer::CardDealt(const Hand& hand, const Card& new_card) {
	cout << m_name << ": You've just been dealt: " << new_card << endl;
	cout << m_name << ": You now hold: " << hand << endl;
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

		Money raise_max = (m_total_balance - minimum_bid);

		Money raise_val;

		cout << "[" << m_name << "] Raise by: (max=" << raise_max << ")" << endl;
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
