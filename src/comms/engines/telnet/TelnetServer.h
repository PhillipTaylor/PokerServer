/*
 * TelnetServer.h
 *
 * This is the entry point for the TelnetServer providing the
 * pathway for people to play Poker via a Telnet connection.
 *
 * The Telnet service is provided by the Boost::ASIO (Asyncronous IO Library)
 */

#ifndef TELNETSERVER_H_
#define TELNETSERVER_H_

#include <boost/asio.hpp>
#include "TelnetPlayer.h"

const short TELNET_PORT_NUMBER = 13305;

namespace NetworkEngines {
namespace Telnet {

using boost::asio::ip::tcp;
using ConcretePlayers::TelnetPlayer;

class TelnetServer {
	public:
		TelnetServer();
		void InitialiseService();
	private:
		boost::asio::io_service io_service;
		tcp::acceptor connection_acceptor;

		void StartAccept();
		void HandleAccept(TelnetPlayer* session, const boost::system::error_code& error);
};

} /* namespace Telnet */
} /* namespace NetworkEngines */
#endif /* TELNETSERVER_H_ */
