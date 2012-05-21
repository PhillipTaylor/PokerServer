/*
 * TelnetServer.cpp
 *
 */

#include <boost/bind.hpp>

#include "TelnetServer.h"

namespace NetworkEngines {
namespace Telnet {

TelnetServer::TelnetServer() :
	io_service(),
	connection_acceptor(io_service, tcp::endpoint(tcp::v4(), TELNET_PORT_NUMBER))
{ }

void TelnetServer::InitialiseService() {

	StartAccept();
	io_service.run();

}

void TelnetServer::StartAccept() {

	TelnetPlayer *new_player = new TelnetPlayer(io_service);

	connection_acceptor.async_accept(
		new_player->GetSocket(),
		boost::bind(
			&TelnetServer::HandleAccept,
			this,
			new_player,
			boost::asio::placeholders::error
		)
	);

}

void TelnetServer::HandleAccept(TelnetPlayer* new_player, const boost::system::error_code& error) {

	if (!error) {
		new_player->Start();
	}

	StartAccept();
}



} /* namespace Telnet */
} /* namespace NetworkEngines */
