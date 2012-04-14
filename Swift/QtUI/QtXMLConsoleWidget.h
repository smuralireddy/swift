/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swift/Controllers/UIInterfaces/XMLConsoleWidget.h>
#include "QtTabbable.h"
#include <QApplication>

#include "Swiften/Base/boost_bsignals.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>

class QTextEdit;
class QCheckBox;
class QColor;

namespace Swift {
	class QtXMLConsoleWidget : public QtTabbable, public XMLConsoleWidget {
			Q_OBJECT

		public:
			QtXMLConsoleWidget();
			~QtXMLConsoleWidget();

			void show();

			void activate();
			
			virtual void handleDataRead(const SafeByteArray& data);
			virtual void handleDataWritten(const SafeByteArray& data);
			void init();						
		private slots :
			void showXMLChat();
			void handleIQSend();
			void handleMessageSend();
			void handlePresenceSend();
			void clickedIQ();
			void clickedPresence();
			void clickedMessage();
		private:
			virtual void closeEvent(QCloseEvent* event);
			virtual void showEvent(QShowEvent* event);			
			void appendTextIfEnabled(const std::string& data, const QColor& color);
			QWidget* getIQWidget();
			QWidget* getMessageWidget();
			QWidget* getPresenceWidget();
			QTextEdit* textEdit;
			QCheckBox* enabled;
			QWidget xmlWindow;
			QWidget* window;
			QRadioButton *get,*set;
			QRadioButton *normal,*chat,*grpchat;
			QComboBox* presenceType;
			QLineEdit *jid,*subject,*status;
			QTextEdit *text,*presencePayload;
	};
}
