/*
 * Copyright (c) 2011 Jan Kaluza
 * Licensed under the Simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include <boost/asio/io_service.hpp>

#include <Swiften/Network/ConnectionServerFactory.h>
#include <Swiften/Network/BoostConnectionServer.h>

namespace Swift {
	class ConnectionServer;

	class BoostConnectionServerFactory : public ConnectionServerFactory {
		public:
			BoostConnectionServerFactory(boost::shared_ptr<boost::asio::io_service>, EventLoop* eventLoop);

			virtual boost::shared_ptr<ConnectionServer> createConnectionServer(int port);

			virtual boost::shared_ptr<ConnectionServer> createConnectionServer(const Swift::HostAddress &hostAddress, int port);

		private:
			boost::shared_ptr<boost::asio::io_service> ioService;
			EventLoop* eventLoop;
	};
}
