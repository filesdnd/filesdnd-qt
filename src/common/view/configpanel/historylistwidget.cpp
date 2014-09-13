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

#include "historylistwidget.h"
#include "appconfig.h"
#include "filehelper.h"
#include "settingsmanager.h"

#include <QFile>
#include <QDebug>
#include <QAbstractItemView>

HistoryListWidget::HistoryListWidget(QWidget *parent) :
    _view(0),
    QListWidget(parent)
{
    // Stylesheet
    setStyleSheet(FileHelper::loadFileContent(HISTORY_LIST_CSS));
    setAttribute(Qt::WA_MacShowFocusRect, false);

    // Elements context menu
    createContextMenuActions();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onContextMenuRequested(const QPoint&)));
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onItemDoubleClicked(QListWidgetItem *)));
}

HistoryListWidget::~HistoryListWidget()
{
    clearHistory();

    delete _historyInfo;
    delete _historyOpenAction;
    delete _deleteFromHistory;
    delete _deleteFromDisk;
    delete _clearHistory;
    delete _historyClipboardCopy;
    delete _historyOpenDownloadFolder;
}

QSize HistoryListWidget::contentSize() const
{
    return QListWidget::contentsSize();
}

void HistoryListWidget::createContextMenuActions()
{
    _historyOpenAction = new QAction(QIcon(HISTORY_LAUNCH_ICON), tr("Ouvrir le fichier"), &_contextMenu);
    connect(_historyOpenAction, SIGNAL(triggered()), this, SLOT(onHistoryOpenActionTriggered()));

    _deleteFromHistory = new QAction(QIcon(HISTORY_REMOVE_ICON), tr("Supprimer de l'historique"), &_contextMenu);
    connect(_deleteFromHistory, SIGNAL(triggered()), this, SLOT(onDeleteFromHistoryTriggered()));

    _deleteFromDisk = new QAction(QIcon(HISTORY_DELETE_FILE_ICON), tr("Supprimer le fichier du disque"), &_contextMenu);
    connect(_deleteFromDisk, SIGNAL(triggered()), this, SLOT(onDeleteFromDiskTriggered()));

    _clearHistory = new QAction(QIcon(HISTORY_CLEAR_HISTORY_ICON), tr("Vider l'historique"), &_contextMenu);
    connect(_clearHistory, SIGNAL(triggered()), this, SLOT(onClearHistoryTriggered()));

    _historyClipboardCopy = new QAction(QIcon(CLIPBOARD_ICON), tr("Copier dans le presse papier"), &_contextMenu);
    connect(_historyClipboardCopy, SIGNAL(triggered()), this, SLOT(onClipboardActionTriggered()));

    _historyInfo = new QAction(&_contextMenu);
    _historyInfo->setEnabled(false);

    _historyOpenDownloadFolder = new QAction(QIcon(FOLDER_ICON), tr("Ouvrir le dossier de téléchargement"), &_contextMenu);
    connect(_historyOpenDownloadFolder, SIGNAL(triggered()), this, SLOT(onOpenDownloadFolderTriggered()));

    _contextMenu.addAction(_historyInfo);
    _contextMenu.addSeparator();
    _contextMenu.addAction(_historyClipboardCopy);
    _contextMenu.addAction(_historyOpenAction);
    _contextMenu.addAction(_deleteFromHistory);
    _contextMenu.addAction(_deleteFromDisk);
    _contextMenu.addSeparator();
    _contextMenu.addAction(_historyOpenDownloadFolder);
    _contextMenu.addAction(_clearHistory);
}

void HistoryListWidget::onHistoryOpenActionTriggered()
{
    openActionHistoryItem(_rightClickHistoryElement);
}

void HistoryListWidget::onDeleteFromHistoryTriggered()
{
    int correspondingRow = row(_rightClickHistoryElement);
    QListWidgetItem *item = takeItem(correspondingRow);

    delete item;

    emit deleteFromHistory(correspondingRow);
}

void HistoryListWidget::onDeleteFromDiskTriggered()
{
    HistoryElementView *element = qobject_cast<HistoryElementView *>(itemWidget(_rightClickHistoryElement));

    FileHelper::deleteFileFromDisk(element->getText());
    onDeleteFromHistoryTriggered();
}

void HistoryListWidget::onClearHistoryTriggered()
{
    emit clearHistoryTriggered();
    clearHistory();
}

void HistoryListWidget::onClipboardActionTriggered()
{
    HistoryElementView *elementView = qobject_cast<HistoryElementView *>(itemWidget(_rightClickHistoryElement));

    FileHelper::saveToClipboard(elementView->getText());
}

void HistoryListWidget::onOpenDownloadFolderTriggered()
{
    QDir dlDir;

    dlDir.mkpath(SettingsManager::getDestinationFolder());
    FileHelper::openURL("file:///" + QDir::toNativeSeparators(SettingsManager::getDestinationFolder()));
}

void HistoryListWidget::openActionHistoryItem(QListWidgetItem *item)
{
    QFileInfo fileInfo;
    HistoryElementView *elementView = qobject_cast<HistoryElementView *>(itemWidget(item));

    switch (elementView->getType())
    {
    case HISTORY_FOLDER_TYPE:
    case HISTORY_FILE_TYPE:
        fileInfo = QFileInfo(SettingsManager::getDestinationFolder() + "/" + elementView->getText());

        if (fileInfo.exists())
            FileHelper::openURL("file:///" + fileInfo.absoluteFilePath());
        break;
    case HISTORY_URL_TYPE:
        FileHelper::openURL(elementView->getText());
        break;
    }
}

void HistoryListWidget::clearHistory()
{
    QListWidgetItem *item;

    while (count() != 0)
    {
         item = takeItem(0);

        delete item;
    }
}

void HistoryListWidget::onContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = itemAt(pos);

    if (item)
    {
        HistoryElementView *historyElement = qobject_cast<HistoryElementView *>(itemWidget(item));
        _rightClickHistoryElement = item;

        switch (historyElement->getType())
        {
        case HISTORY_FOLDER_TYPE:
        case HISTORY_FILE_TYPE:
            manageFileHistoryContextMenu(historyElement);
            break;
        default:
            manageTextUrlHistoryContextMenu(historyElement);
            break;
        }

        _contextMenu.exec(mapToGlobal(pos));
    }
}

void HistoryListWidget::manageFileHistoryContextMenu(HistoryElementView *historyElement)
{
    QString info = historyElement->getText();
    QFileInfo file(SettingsManager::getDestinationFolder() + "/" + info);
    bool enabled;

    _historyOpenDownloadFolder->setVisible(true);
    _historyOpenAction->setVisible(true);
    _deleteFromDisk->setVisible(true);
    _historyClipboardCopy->setVisible(false);

    if (file.isDir())
    {
        _historyOpenAction->setIcon(QIcon(FOLDER_ICON));
        _historyOpenAction->setText(tr("Ouvrir le dossier"));
    }
    else
    {
        _historyOpenAction->setIcon(QIcon(HISTORY_LAUNCH_ICON));
        _historyOpenAction->setText(tr("Ouvrir le fichier"));
    }
    info.append(" (").append(historyElement->getFileSize()).append(")");
    _historyInfo->setIcon(historyElement->getIcon());
    _historyInfo->setText(info);
    _deleteFromHistory->setEnabled(!historyElement->isDownloading());

    enabled = (historyElement->isDownloading()
               || !FileHelper::exists(historyElement->getText()));
    _historyOpenAction->setEnabled(!enabled);
    _deleteFromDisk->setEnabled(!enabled);
}

void HistoryListWidget::manageTextUrlHistoryContextMenu(HistoryElementView *historyElement)
{
    QString historyInfoString;
    int maxCharDisplayed = 50;

    _historyOpenAction->setEnabled(true);
    _historyOpenDownloadFolder->setVisible(false);
    _deleteFromDisk->setVisible(false);
    _historyClipboardCopy->setVisible(true);

    _deleteFromHistory->setEnabled(true);

    if (historyElement->getType() == HISTORY_URL_TYPE)
    {
        _historyInfo->setIcon(QIcon(URL_ICON));
        _historyOpenAction->setVisible(true);
        _historyOpenAction->setText(tr("Ouvrir le lien"));
    }
    else
    {
        _historyInfo->setIcon(QIcon(TEXT_ICON));
        _historyOpenAction->setVisible(false);
    }

    historyInfoString = historyElement->getText().left(maxCharDisplayed);
    if (historyElement->getText().size() > maxCharDisplayed)
        historyInfoString.append(" ...");

    _historyInfo->setText(historyInfoString);
}

void HistoryListWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    openActionHistoryItem(item);
}

void HistoryListWidget::historyElementProgressUpdated(unsigned progress)
{
    if (count() > 0)
    {
        QListWidgetItem *firstItem = item(0);
        HistoryElementView *elt = qobject_cast<HistoryElementView *>(itemWidget(firstItem));

        if (elt->getType() == HISTORY_FOLDER_TYPE || elt->getType() == HISTORY_FILE_TYPE)
        {
            if (progress == 100)
                refreshAllHistory();
            else
            {
                elt->setProgress(progress);
                firstItem->setSizeHint(QSize(0, elt->sizeHint().height()));
            }
        }
    }
}

void HistoryListWidget::refreshAllHistory()
{
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *listItem = item(i);
        HistoryElementView *elt = qobject_cast<HistoryElementView *>(itemWidget(listItem));

        elt->refresh();
        listItem->setSizeHint(QSize(0, elt->sizeHint().height()));
    }
}

void HistoryListWidget::onHistoryChanged(const QList<HistoryElement> &history)
{
    clearHistory();
    foreach(HistoryElement elt, history)
    {
        HistoryElementView *historyViewElement = new HistoryElementView(elt.getDateTime("dd/MM - hh:mm"), elt.getText(), elt.getName(), elt.getSize(), elt.getType());
        QListWidgetItem *item = new QListWidgetItem();

        connect(historyViewElement, SIGNAL(cancelIncomingTransfert()),
                _view, SLOT(onCancelIncomingTransfert()));

        item->setSizeHint(QSize(0,historyViewElement->sizeHint().height()));
        addItem(item);
        setItemWidget(item, historyViewElement);
    }
}

void HistoryListWidget::setView(View *view)
{
    _view = view;
}
