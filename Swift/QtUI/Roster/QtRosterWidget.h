/*
 * Copyright (c) 2011 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swift/QtUI/Roster/QtTreeWidget.h"
#include <QPoint>
#include <QContextMenuEvent>
#include <QMenu>
#include <QInputDialog>
#include <QFileDialog>
namespace Swift {
class QtUIPreferences;

class QtRosterWidget : public QtTreeWidget {
	Q_OBJECT
	public:
		QtRosterWidget(UIEventStream* eventStream, SettingsProvider* settings, QWidget* parent = 0);
		virtual ~QtRosterWidget();
		void mousePressedEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);
	public slots:
		void handleEditUserActionTriggered(bool checked);
	protected:
		void contextMenuEvent(QContextMenuEvent* event);
	private:
		void renameGroup(GroupRosterItem* group);
		QPoint dragStartPosition;
		QModelIndex clickedIndex;
};

}
