/*
 * Copyright (c) 2011 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Parser/GenericElementParser.h>
#include <Swiften/Elements/StreamResumed.h>

namespace Swift {
	class StreamResumedParser : public GenericElementParser<StreamResumed> {
		public:
			StreamResumedParser();
			~StreamResumedParser();

			virtual void handleStartElement(const std::string&, const std::string&, const AttributeMap&);
			virtual void handleEndElement(const std::string&, const std::string&);

		private:
			enum Level { 
				TopLevel = 0
			};
			int level;
	};
}
