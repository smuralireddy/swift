/*
 * Copyright (c) 2010 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>

namespace Swift {
	class UIEvent {
		public:
			typedef boost::shared_ptr<UIEvent> ref;

			virtual ~UIEvent();
	};
}
