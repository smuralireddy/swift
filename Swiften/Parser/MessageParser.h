/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Parser/GenericStanzaParser.h>
#include <Swiften/Elements/Message.h>

namespace Swift {
	class MessageParser : public GenericStanzaParser<Message> {
		public:
			MessageParser(PayloadParserFactoryCollection* factories);

		private:
			virtual void handleStanzaAttributes(const AttributeMap&);
	};
}
