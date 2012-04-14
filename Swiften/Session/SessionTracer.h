/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <string>

#include <Swiften/Session/Session.h>
#include <Swiften/Base/SafeByteArray.h>

namespace Swift {
	class SessionTracer {
		public:
			SessionTracer(boost::shared_ptr<Session> session);

		private:
			void printData(char direction, const SafeByteArray& data);

			boost::shared_ptr<Session> session;
	};
}
