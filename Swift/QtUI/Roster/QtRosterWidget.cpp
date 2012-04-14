/*
 * Copyright (c) 2011 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Roster/QtRosterWidget.h"
#include "Swift/Controllers/UIEvents/RequestContactEditorUIEvent.h"
#include "Swift/Controllers/UIEvents/RemoveRosterItemUIEvent.h"
#include "Swift/Controllers/UIEvents/RenameGroupUIEvent.h"
#include "Swift/Controllers/UIEvents/SendFileUIEvent.h"
#include "QtContactEditWindow.h"
#include "Swift/Controllers/Roster/ContactRosterItem.h"
#include "Swift/Controllers/Roster/GroupRosterItem.h"
#include "Swift/Controllers/UIEvents/UIEventStream.h"
#include "QtSwiftUtil.h"
#include <QDrag>
#include <QMimeData>

namespace Swift {

QtRosterWidget::QtRosterWidget(UIEventStream* eventStream, SettingsProvider* settings, QWidget* parent) : QtTreeWidget(eventStream, settings, parent) {

}

QtRosterWidget::~QtRosterWidget() {

}

void QtRosterWidget::handleEditUserActionTriggered(bool /*checked*/) {
	QModelIndexList selectedIndexList = getSelectedIndexes();
	if (selectedIndexList.empty()) {
		return;
	}
	QModelIndex index = selectedIndexList[0];
	if (!index.isValid()) {
		return;
	}
	RosterItem* item = static_cast<RosterItem*>(index.internalPointer());
	if (ContactRosterItem* contact = dynamic_cast<ContactRosterItem*>(item)) {
		eventStream_->send(boost::make_shared<RequestContactEditorUIEvent>(contact->getJID()));
	}
}

void QtRosterWidget::contextMenuEvent(QContextMenuEvent* event) {
	QModelIndex index = indexAt(event->pos());
	if (!index.isValid()) {
		return;
	}
	RosterItem* item = static_cast<RosterItem*>(index.internalPointer());
	QMenu contextMenu;
	if (ContactRosterItem* contact = dynamic_cast<ContactRosterItem*>(item)) {
		QAction* editContact = contextMenu.addAction(tr("Edit"));
		QAction* removeContact = contextMenu.addAction(tr("Remove"));
#ifdef SWIFT_EXPERIMENTAL_FT
		QAction* sendFile = NULL;
		if (contact->supportsFeature(ContactRosterItem::FileTransferFeature)) {
			sendFile = contextMenu.addAction(tr("Send File"));
		}
#endif
		QAction* result = contextMenu.exec(event->globalPos());
		if (result == editContact) {
			eventStream_->send(boost::make_shared<RequestContactEditorUIEvent>(contact->getJID()));
		}
		else if (result == removeContact) {
			if (QtContactEditWindow::confirmContactDeletion(contact->getJID())) {
				eventStream_->send(boost::make_shared<RemoveRosterItemUIEvent>(contact->getJID()));
			}
		}
#ifdef SWIFT_EXPERIMENTAL_FT
		else if (sendFile && result == sendFile) {
			QString fileName = QFileDialog::getOpenFileName(this, tr("Send File"), "", tr("All Files (*);;"));
			if (!fileName.isEmpty()) {
				eventStream_->send(boost::make_shared<SendFileUIEvent>(contact->getJID(), Q2PSTRING(fileName)));
			}
		}
#endif
	}
	else if (GroupRosterItem* group = dynamic_cast<GroupRosterItem*>(item)) {
		QAction* renameGroupAction = contextMenu.addAction(tr("Rename"));
		QAction* result = contextMenu.exec(event->globalPos());
		if (result == renameGroupAction) {
			renameGroup(group);
		}
	}
}

void QtRosterWidget::renameGroup(GroupRosterItem* group) {
	bool ok;
	QString newName = QInputDialog::getText(NULL, tr("Rename group"), tr("Enter a new name for group '%1':").arg(P2QSTRING(group->getDisplayName())), QLineEdit::Normal, P2QSTRING(group->getDisplayName()), &ok);
	if (ok) {
		eventStream_->send(boost::make_shared<RenameGroupUIEvent>(group->getDisplayName(), Q2PSTRING(newName)));
	}
}
void QtRosterWidget::mousePressedEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		dragStartPosition = event->pos();
	}
}
void QtRosterWidget::mouseMoveEvent(QMouseEvent *event)
 {
	if (!(event->buttons() & Qt::LeftButton)) {
		return;
	}
	if ((event->pos() - dragStartPosition).manhattanLength()< QApplication::startDragDistance()) {
		return;
	}
	clickedIndex=indexAt(event->pos());
	if (!clickedIndex.isValid()) {
		return;
	}
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData();
	RosterItem* item = static_cast<RosterItem*>(clickedIndex.internalPointer());
	ContactRosterItem* contact = dynamic_cast<ContactRosterItem*>(item);
	std::string jid=contact->getJID().toString();
	QString str=QString::P2QSTRING(jid);
	mimeData->setText(str);
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction | Qt::MoveAction);     
 }

}
