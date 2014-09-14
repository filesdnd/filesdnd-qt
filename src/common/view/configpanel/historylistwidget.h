/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#ifndef HISTORYLISTWIDGET_H
#define HISTORYLISTWIDGET_H

#include <QListWidget>
#include <QAction>
#include <QMenu>

#include "view.h"
#include "historyelementview.h"

/**
 * @class Advanced history list widget
 *
 * This class shows the history
 */
class HistoryListWidget : public QListWidget
{
    Q_OBJECT
public:
    /// Constructor
    explicit HistoryListWidget(QWidget *parent = 0);
    /// Destructor
    ~HistoryListWidget();

    /// See QListView::contentsSize()
    QSize contentSize() const;

    /**
     * Try to open a file or a link from an history item
     *
     * @param item History Item
     */
    void openActionHistoryItem(QListWidgetItem *item);
    /**
     * Clear the history
     */
    void clearHistory();
    /**
     * @brief Setter : _view
     *
     * @param view The main view
     */
    void setView(View *view);

signals:
    /**
     * Notify the service for an history element deleted
     */
    void deleteFromHistory(int row);
    /**
     * Notify the service for an history clean up
     */
    void clearHistoryTriggered();


private slots:
    /**
     * On open file action triggered
     */
    void onHistoryOpenActionTriggered();
    /**
     * On delete from history triggered
     */
    void onDeleteFromHistoryTriggered();
    /**
     * On delete from disk triggered
     */
    void onDeleteFromDiskTriggered();
    /**
     * On clear history triggered
     */
    void onClearHistoryTriggered();
    /**
     * On clipboard action triggered
     */
    void onClipboardActionTriggered();
    /**
     * On open download history triggered
     */
    void onOpenDownloadFolderTriggered();
    /**
     * Show the history menu
     */
    void onContextMenuRequested(const QPoint &pos);
    /**
      * On history item double clicked
      */
    void onItemDoubleClicked(QListWidgetItem *item);

public slots:
    /**
     * SLOT : Download (on last history element) progress changed
     *
     * @param progress Download progress percentage
     */
    void historyElementProgressUpdated(unsigned progress);
    /**
     * SLOT : on history changed
     * @param history New history value
     */
    void onHistoryChanged(const QList<HistoryElement> &history);

private:
    /// List of displayed elements
    QList<HistoryElement> _history;
    /// Main View
    View *_view;
    /// Context menu of the history view
    QMenu _contextMenu;
    /// History element correponding to the right click on the history view
    QListWidgetItem *_rightClickHistoryElement;
    /// Open the corresponding history view element
    QAction *_historyOpenAction;
    /// Open the current download folder
    QAction *_historyOpenDownloadFolder;
    /// Information for an history element
    QAction *_historyInfo;
    /// Open the corresponding history view file
    QAction *_deleteFromHistory;
    /// Delete the corresponding file from the disk
    QAction *_deleteFromDisk;
    /// Copy the current history text into the clipboard
    QAction *_historyClipboardCopy;
    /// CLear the entire history
    QAction *_clearHistory;

    /**
     * Create actions for the history context menu
     */
    void createContextMenuActions();
    /**
     * Manage the right click on an history element containing a file or a folder
     *
     * @param historyElement Aimed element
     */
    void manageFileHistoryContextMenu(HistoryElementView *historyElement);
    /**
     * Manage the right click on an history element containing text or URL
     *
     * @param historyElement Aimed element
     */
    void manageTextUrlHistoryContextMenu(HistoryElementView *historyElement);
};

#endif // HISTORYLISTWIDGET_H
