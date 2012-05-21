/*
 * TelnetPlayer.cpp
 *
 */

#include <boost/bind.hpp>

#include "TelnetPlayer.h"
#include "poker_utils/CommonFunctions.h"

using namespace PokerUtils;

namespace ConcretePlayers {

const std::string TELNET_NEWLINE = "\n";

TelnetPlayer::TelnetPlayer(boost::asio::io_service& io_service) :
	socket(io_service),
	buffer(),
	read_state(RS_NOT_WAITING),
	name()
{}

TelnetPlayer::~TelnetPlayer() {

}

void TelnetPlayer::Start() {

	Write("Please enter your name: ");
	read_state = RS_WAITING_FOR_NAME;

	boost::asio::async_read_until(
		socket,
		buffer,
		TELNET_NEWLINE,
		boost::bind(&TelnetPlayer::HandleRead, this, _1)
	);
}

tcp::socket& TelnetPlayer::GetSocket() {
	return socket;
}

void TelnetPlayer::Write(const std::string& message) {
	boost::asio::async_write(
		socket,
		boost::asio::buffer(message.c_str(), message.length()),
		boost::bind(&TelnetPlayer::HandleWrite, this, boost::asio::placeholders::error)
	);
}

void TelnetPlayer::HandleRead(const boost::system::error_code& error) {

	std::istream is(&buffer);
	std::string message = AutoToString(is);
//	std::cout << "Read this: " << is << "\n";

	buffer.consume(buffer.size()); //clear buffer

	switch (read_state) {
		case (RS_WAITING_FOR_NAME):
			SetName(message);
			break;
		case (RS_NOT_WAITING):
			ChatMessage(name, message);
			break;
		case (RS_WAITING_FOR_PLAY):
			break;
	}
	read_state = RS_NOT_WAITING;
}

void TelnetPlayer::HandleWrite(const boost::system::error_code& error) {}

bool TelnetPlayer::IsPlaying() {
	return true;
}

void TelnetPlayer::SetName(const std::string& name) {
	this->name = "Derek";
}

string TelnetPlayer::GetName() {
	return name;
}

Money TelnetPlayer::GetTotalBalance() {
	return total_balance;
}

void TelnetPlayer::SetTotalBalance(Money new_value) {
	total_balance = new_value;
}

void TelnetPlayer::DealerAnnounce(std::string dealer) {
	string msg = "Dealer is" + dealer;
	Write(msg);
}

void TelnetPlayer::SmallBlindAnnounce(std::string payer, Money amount) {
	string msg = "Small Blind is" + payer + " with amount " + AutoToString(amount);
	Write(msg);
}

void TelnetPlayer::BigBlindAnnounce(std::string payer, Money amount) {
	string msg = "Big Blind is" + payer + " with amount " + AutoToString(amount);
	Write(msg);
}

void TelnetPlayer::OpponentMoneyUpdate(std::string player, Money pot, Money bank) {
	string msg = "player " + player + " now has " + AutoToString(bank);
	Write(msg);
}

void TelnetPlayer::OpponentCardAnnounce(std::string player, const Hand& players_hand) {
	string msg = "player " + player + " holds " + players_hand.GetHandTextualDescription();
	Write(msg);
}

void TelnetPlayer::WinnerAnnounce(std::string player, Money winnings) {
	string msg = "player " + player + " won " + AutoToString(winnings);
	Write(msg);
}

void TelnetPlayer::ChatMessage(std::string sender, std::string message) {
	string msg = "Chat: " + sender + ": " + message;
	Write(msg);
}

void TelnetPlayer::CardDealt(const Hand& hand, const Card& new_card) {
	string msg = "You have been dealt: " + new_card.ToLongString();
	Write(msg);
}

GameChoice TelnetPlayer::MakeChoice(Money minimum_bid) {
	GameChoice gc;
	return gc;
}


} /* namespace ConcretePlayers */
