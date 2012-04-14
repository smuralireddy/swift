/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swiften/Base/boost_bsignals.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <Swiften/Client/StanzaChannel.h>
#include "Swift/Controllers/UIEvents/UIEventStream.h"
#include <Swiften/Base/SafeByteArray.h>
#include <Swiften/Elements/Message.h>
#include <Swiften/Elements/IQ.h>
#include <Swiften/Elements/RawXMLPayload.h>
#include <Swiften/JID/JID.h>
#include <Swiften/Elements/Message.h>
#include <Swiften/Base/IDGenerator.h>


namespace Swift {
	
	class XMLConsoleWidgetFactory;
	class XMLConsoleWidget;
	

	class XMLConsoleController {
		public:
			XMLConsoleController(UIEventStream* uiEventStream, XMLConsoleWidgetFactory* xmlConsoleWidgetFactory);
			~XMLConsoleController();
			void setStanzaChannel(StanzaChannel *);

		public:
			void handleDataRead(const SafeByteArray& data);
			void handleDataWritten(const SafeByteArray& data);
		private:
			void sendIQ(int type,std::string& to,std::string& payload);
			void sendMessage(int type,const std::string& to,const std::string& sub,const std::string& data);
			void sendPresence(int type,std::string&,std::string& payload);
			void handleUIEvent(boost::shared_ptr<UIEvent> event);

		private:
			XMLConsoleWidgetFactory* xmlConsoleWidgetFactory;
			XMLConsoleWidget* xmlConsoleWidget;
			StanzaChannel* stanzaChannel;
	};
}
