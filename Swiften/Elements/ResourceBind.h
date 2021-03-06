/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <string>
#include <Swiften/Elements/Payload.h>
#include <Swiften/JID/JID.h>

namespace Swift {
	class ResourceBind : public Payload {
		public:
			ResourceBind() {}

			void setJID(const JID& jid) {
				jid_ = jid;
			}

			const JID& getJID() const { 
				return jid_; 
			}

			void setResource(const std::string& resource) {
				resource_ = resource;
			}

			const std::string& getResource() const {
				return resource_; 
			}

		private:
			JID jid_;
			std::string resource_;
	};
}
