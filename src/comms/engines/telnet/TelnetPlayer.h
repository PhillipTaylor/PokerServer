/*
 * TelnetPlayer.h
 *
 * Connects the GameEngine to a Player communicating via a Telnet session.
 */

#ifndef TELNETPLAYER_H_
#define TELNETPLAYER_H_

#include <boost/asio.hpp>

#include "core/Money.h"
#include "core/AbstractPlayer.h"
#include "core/Hand.h"

using namespace GameEngineCore;
using boost::asio::ip::tcp;

namespace ConcretePlayers {

enum SOCKET_READ_STATE {
	RS_WAITING_FOR_NAME,
	RS_WAITING_FOR_PLAY,
	RS_NOT_WAITING
};

class TelnetPlayer : public AbstractPlayer {

	public:
		TelnetPlayer(boost::asio::io_service& io_service);
		void Start();
		void ReadyForInput();
		tcp::socket& GetSocket();

		bool IsPlaying();
		string GetName();
		Money GetTotalBalance();
		void SetTotalBalance(Money new_value);

		void DealerAnnounce(std::string dealer);
		void SmallBlindAnnounce(std::string payer, Money amount);
		void BigBlindAnnounce(std::string payer, Money amount);
		void OpponentMoneyUpdate(std::string player, Money pot, Money bank);
		void OpponentCardAnnounce(std::string player, const Hand& players_hand);
		void WinnerAnnounce(std::string player, Money winnings);

		void ChatMessage(std::string sender, std::string message);
		void CardDealt(const Hand& hand, const Card& new_card);
		GameChoice MakeChoice(Money minimum_bid);

	private:
		tcp::socket socket;
		boost::asio::streambuf buffer;
		SOCKET_READ_STATE read_state;

		std::string name;
		Money total_balance;

		void SetName(const std::string& name);

		void Write(const std::string& message);
		void HandleRead(const boost::system::error_code& error);
		void HandleWrite(const boost::system::error_code& error);

		~TelnetPlayer();
};

} /* namespace ConcretePlayers */
#endif /* TELNETPLAYER_H_ */
