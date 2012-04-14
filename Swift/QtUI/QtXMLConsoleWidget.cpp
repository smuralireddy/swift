/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swift/QtUI/QtXMLConsoleWidget.h"
#include "Swiften/Base/boost_bsignals.h"
#include <QCloseEvent>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QCheckBox>
#include <QLabel>
#include <QtCore>
#include <qnamespace.h>
#include "QtSwiftUtil.h"
#include <string>

namespace Swift {


QtXMLConsoleWidget::QtXMLConsoleWidget() {
	setWindowTitle(tr("Console"));

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);

	textEdit = new QTextEdit(this);
	textEdit->setReadOnly(true);
	layout->addWidget(textEdit);

	QWidget* bottom = new QWidget(this);
	layout->addWidget(bottom);
	bottom->setAutoFillBackground(true);

	QHBoxLayout* buttonLayout = new QHBoxLayout(bottom);
	buttonLayout->setContentsMargins(10,0,20,0);
	buttonLayout->setSpacing(0);

	enabled = new QCheckBox(tr("Trace input/output"), bottom);
	enabled->setChecked(true);
	buttonLayout->addWidget(enabled);

	buttonLayout->addStretch();

	QPushButton* clearButton = new QPushButton(tr("Clear"), bottom);
	connect(clearButton, SIGNAL(clicked()), textEdit, SLOT(clear()));
	buttonLayout->addWidget(clearButton);

	QPushButton* xmlButton = new QPushButton(tr("XMLChat"),bottom);
	buttonLayout->addWidget(xmlButton);
	connect(xmlButton,SIGNAL(clicked()),this,SLOT(showXMLChat()));

	setWindowTitle(tr("Debug Console"));
	emit titleUpdated();
}


QtXMLConsoleWidget::~QtXMLConsoleWidget() {
}

void QtXMLConsoleWidget::showXMLChat() {
	init();
}

void QtXMLConsoleWidget::showEvent(QShowEvent* event) {
	emit windowOpening();
	emit titleUpdated(); /* This just needs to be somewhere after construction */
	QWidget::showEvent(event);
}

void QtXMLConsoleWidget::show() {
	QWidget::show();
	emit windowOpening();
}

void QtXMLConsoleWidget::activate() {
	emit wantsToActivate();
}

void QtXMLConsoleWidget::closeEvent(QCloseEvent* event) {
	emit windowClosing();
	event->accept();
}

void QtXMLConsoleWidget::handleDataRead(const SafeByteArray& data) {
	appendTextIfEnabled(std::string(tr("<!-- IN -->").toUtf8()) + "\n" + safeByteArrayToString(data) + "\n", QColor(33,98,33));
}

void QtXMLConsoleWidget::handleDataWritten(const SafeByteArray& data) {
	appendTextIfEnabled(std::string(tr("<!-- OUT -->").toUtf8()) + "\n" + safeByteArrayToString(data) + "\n", QColor(155,1,0));
}

void QtXMLConsoleWidget::appendTextIfEnabled(const std::string& data, const QColor& color) {
	if (enabled->isChecked()) {
		QScrollBar* scrollBar = textEdit->verticalScrollBar();
		bool scrollToBottom = (!scrollBar || scrollBar->value() == scrollBar->maximum());

		QTextCursor cursor(textEdit->document());
		cursor.beginEditBlock();
		cursor.movePosition(QTextCursor::End);
		QTextCharFormat format;
		format.setForeground(QBrush(color));
		cursor.mergeCharFormat(format);
		cursor.insertText(P2QSTRING(data));
		cursor.endEditBlock();

		if (scrollToBottom) {
			scrollBar->setValue(scrollBar->maximum());
		}
	}
}

void QtXMLConsoleWidget::init()	{
	window = new QWidget();
	window->resize(320,240);
	window->setWindowTitle(tr("XML chat"));		
	QVBoxLayout* iqLayout = new QVBoxLayout;
	QLabel* label = new QLabel("What do you want send?");
	QPushButton* iqButton = new QPushButton(tr("&IQ"));
	connect(iqButton,SIGNAL(clicked()),this,SLOT(clickedIQ()));
	QPushButton* presenceButton = new QPushButton(tr("Presence"));
	QObject::connect(presenceButton,SIGNAL(clicked()),this,SLOT(clickedPresence()));
	QPushButton* messageButton = new QPushButton(tr("Message"));
	QObject::connect(messageButton,SIGNAL(clicked()),this,SLOT(clickedMessage()));
	iqLayout->addWidget(label);
	iqLayout->addWidget(iqButton);
	iqLayout->addWidget(presenceButton);
	iqLayout->addWidget(messageButton);
	window->setLayout(iqLayout);
	window->show();
}
QWidget* QtXMLConsoleWidget::getIQWidget() {
	QVBoxLayout* layout = new QVBoxLayout();
	QGroupBox* groupBox = new QGroupBox(tr("Type attributes"));
	get = new QRadioButton(tr("&get"));
	set = new QRadioButton(tr("&set"));
	jid = new QLineEdit("enterjid");
	text = new QTextEdit;
	QPushButton* enter = new QPushButton("Enter");
	connect(enter,SIGNAL(clicked()),this,SLOT(handleIQSend()));
	get->setChecked(true);
	set->setChecked(false);
	layout->addWidget(get);
	layout->addWidget(set);
	layout->addWidget(jid);
	layout->addWidget(text);
	layout->addWidget(enter);
	groupBox->setLayout(layout);
	return groupBox;
}
QWidget* QtXMLConsoleWidget::getMessageWidget() {
	QVBoxLayout* layout = new QVBoxLayout();
	QGroupBox* groupBox = new QGroupBox(tr("Type attributes"));
	normal = new QRadioButton(tr("&normal"));
	chat = new QRadioButton(tr("&chat"));
	grpchat = new QRadioButton(tr("&Groupchat"));
	jid = new QLineEdit("enter jid");
	subject = new QLineEdit("enter subject");
	text = new QTextEdit("enter message");
	QPushButton* enter = new QPushButton("Enter");
	connect(enter,SIGNAL(clicked()),this,SLOT(handleMessageSend()));
	normal->setChecked(true);
	chat->setChecked(false);
	grpchat->setChecked(false);
	layout->addWidget(normal);
	layout->addWidget(chat);
	layout->addWidget(grpchat);
	layout->addWidget(jid);
	layout->addWidget(subject);
	layout->addWidget(text);
	layout->addWidget(enter);
	groupBox->setLayout(layout);
	return groupBox;
}
QWidget* QtXMLConsoleWidget::getPresenceWidget() {	
	QGroupBox* groupBox = new QGroupBox();
	QVBoxLayout* layout = new QVBoxLayout();
	presenceType = new QComboBox();
	presenceType->addItem("Available");
	presenceType->addItem("Error");
	presenceType->addItem("Probe");
	presenceType->addItem("Subscribe");
	presenceType->addItem("Subscribed");
	presenceType->addItem("Unavailable");
	presenceType->addItem("Unsubscribe");
	presenceType->addItem("Unsubscribed");
	QLabel* label = new QLabel("Select type");
	presencePayload = new QTextEdit("Or Enter XML Payload");
	QLabel* label2 = new QLabel("Enter status");
	status = new QLineEdit();
	QPushButton *enter = new QPushButton("Enter");
	connect(enter,SIGNAL(clicked()),this,SLOT(handlePresenceSend()));
	layout->addWidget(label);
	layout->addWidget(presenceType);
	layout->addWidget(label2);
	layout->addWidget(status);
	layout->addWidget(enter);
	layout->addWidget(presencePayload);
	groupBox->setLayout(layout);
	return groupBox;
}
void QtXMLConsoleWidget::clickedIQ() {
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(getIQWidget());
	widget->setLayout(layout);
	widget->show();
}
void QtXMLConsoleWidget::clickedPresence() {
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(getPresenceWidget());
	widget->setLayout(layout);
	widget->show();
}
void QtXMLConsoleWidget::clickedMessage() {
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(getMessageWidget());
	widget->setLayout(layout);
	widget->show();
}
void QtXMLConsoleWidget::handleIQSend() {
	int type;
	if (get->isChecked()) {
		type = 0;
		}
	else {
		type = 1;
	}
	QString jidText = jid->displayText();
	std::string jidString = Q2PSTRING(jidText);
	QString paydataText = text->toPlainText();
	std::string paydata = Q2PSTRING(paydataText);
	onRequestedIQSend(type,jidString,paydata);
}
void QtXMLConsoleWidget::handleMessageSend() {
	int type;
	if (normal->isChecked()) {
		type = 0;
		}
	else if (chat->isChecked()) {
		type = 1;
	}
	else {
		type = 2;
	}
	QString jidText = jid->displayText();
	std::string jidString = Q2PSTRING(jidText);
	QString subjectText = subject->displayText();
	std::string subjectString = Q2PSTRING(subjectText);
	QString dataText = text->toPlainText();
	std::string data = Q2PSTRING(dataText);
	onRequestedMessageSend(type,jidString,subjectString,data);
}
void QtXMLConsoleWidget::handlePresenceSend() {
	QString string = status->displayText();
	std::string status = Q2PSTRING(string);
	QString payload = presencePayload->toPlainText();
	std::string stdpayload = Q2PSTRING(payload);
	onRequestedPresenceSend(presenceType->currentIndex(),status,stdpayload);
}

}
