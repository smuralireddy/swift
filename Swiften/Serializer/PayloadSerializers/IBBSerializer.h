/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Serializer/GenericPayloadSerializer.h>
#include <Swiften/Elements/IBB.h>

namespace Swift {
	class IBBSerializer : public GenericPayloadSerializer<IBB> {
		public:
			IBBSerializer();

			virtual std::string serializePayload(boost::shared_ptr<IBB>)  const;
	};
}
