/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Elements/Nickname.h>
#include <Swiften/Parser/GenericPayloadParser.h>

namespace Swift {
	class NicknameParser : public GenericPayloadParser<Nickname> {
		public:
			NicknameParser();

			virtual void handleStartElement(const std::string& element, const std::string&, const AttributeMap& attributes);
			virtual void handleEndElement(const std::string& element, const std::string&);
			virtual void handleCharacterData(const std::string& data);

		private:
			int level;
			std::string text;
	};
}
