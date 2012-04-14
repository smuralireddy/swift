/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swift/Controllers/XMLConsoleController.h"
#include "Swift/Controllers/UIInterfaces/XMLConsoleWidgetFactory.h"
#include "Swift/Controllers/UIEvents/RequestXMLConsoleUIEvent.h"
#include "Swiften/Elements/StatusShow.h"

namespace Swift {

XMLConsoleController::XMLConsoleController(UIEventStream* uiEventStream, XMLConsoleWidgetFactory* xmlConsoleWidgetFactory) : xmlConsoleWidgetFactory(xmlConsoleWidgetFactory), xmlConsoleWidget(NULL) {
	uiEventStream->onUIEvent.connect(boost::bind(&XMLConsoleController::handleUIEvent, this, _1));
	
}

XMLConsoleController::~XMLConsoleController() {
	delete xmlConsoleWidget;
}
void XMLConsoleController::setStanzaChannel(StanzaChannel* stanzaChannel) {
	this->stanzaChannel = stanzaChannel;	
}
void XMLConsoleController::handleUIEvent(boost::shared_ptr<UIEvent> rawEvent) {
	boost::shared_ptr<RequestXMLConsoleUIEvent> event = boost::dynamic_pointer_cast<RequestXMLConsoleUIEvent>(rawEvent);
	if (event != NULL) {
		if (xmlConsoleWidget == NULL) {
			xmlConsoleWidget = xmlConsoleWidgetFactory->createXMLConsoleWidget();
			xmlConsoleWidget->onRequestedIQSend.connect(bind(&XMLConsoleController::sendIQ, this, _1, _2, _3));
			xmlConsoleWidget->onRequestedMessageSend.connect(bind(&XMLConsoleController::sendMessage, this, _1, _2, _3, _4));
			xmlConsoleWidget->onRequestedPresenceSend.connect(bind(&XMLConsoleController::sendPresence,this,_1,_2,_3));
		}
		xmlConsoleWidget->show();
		xmlConsoleWidget->activate();
	}
}

void XMLConsoleController::handleDataRead(const SafeByteArray& data) {
	if (xmlConsoleWidget) {
		xmlConsoleWidget->handleDataRead(data);
	}
}

void XMLConsoleController::handleDataWritten(const SafeByteArray& data) {
	if (xmlConsoleWidget) {
		xmlConsoleWidget->handleDataWritten(data);
	}
}
void XMLConsoleController::sendIQ(int type,std::string& to,std::string& payload) {
	IQ::Type attributeType;
	if (type == 0) {
		attributeType = IQ::Get;
	}
	else {
		attributeType = IQ::Set;
	}
	boost::shared_ptr<RawXMLPayload> rawPayload=boost::make_shared<RawXMLPayload>();
	rawPayload->setRawXML(payload);
	std::string  id = stanzaChannel->getNewIQID();
	boost::shared_ptr<Swift::IQ> iq = IQ::createRequest(attributeType,JID(to),id,rawPayload);
	stanzaChannel->sendIQ(iq);	
}
void XMLConsoleController::sendMessage(int type,const std::string& to,const std::string& sub,const std::string& data) {
	Message::Type attributeType;
	switch (type) {
		case 0:
			attributeType = Message::Normal;
			break;
		case 1:
			attributeType = Message::Chat;
			break;
		case 2:
			attributeType = Message::Groupchat;
			break;
		default:
			std::cerr<<"Invalid type at Message";
			return;
	}
	boost::shared_ptr<Message> message = boost::make_shared<Message>();
	message->setType(attributeType);
	message->setSubject(sub);
	message->setBody(data);
	message->setTo(JID(to));
	message->setID(stanzaChannel->getNewIQID());
	stanzaChannel->sendMessage(message);
}
void XMLConsoleController::sendPresence(int type,std::string& status,std::string& payload) {
	boost::shared_ptr<Presence> presence = Presence::create();
	Presence::Type attributeType;
	attributeType=static_cast<Presence::Type>(type);
	presence->setType(attributeType);
	presence->setStatus(status);
	boost::shared_ptr<RawXMLPayload> rawPayload = boost::make_shared<RawXMLPayload>();
	rawPayload->setRawXML(payload);
	presence->addPayload(rawPayload);
	stanzaChannel->sendPresence(presence);
}

}
