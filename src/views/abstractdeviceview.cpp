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

#include "abstractdeviceview.h"
#include "helpers/logmanager.h"

#include <QUrl>
#include <QFileInfo>
#include <QMimeData>

AbstractDeviceView::AbstractDeviceView(const QString &name, const QString &uid, bool available, QWidget *parent) :
    QWidget(parent)
{
    _name = name;
    _uid = uid;
    _available = available;
}

AbstractDeviceView::~AbstractDeviceView()
{

}

void AbstractDeviceView::dragEnterEvent(QDragEnterEvent *event)
{
    QString str;
    const QMimeData *mimeData = event->mimeData();
    QRegExp linkRx("^(http|https|ftp)://\\S+$");

    if (mimeData)
    {
        if (mimeData->hasUrls())
        {
            str = mimeData->urls().first().toString();
            if (str.split(":/").first().contains("file"))
            {
                if (mimeData->urls().size() > 1)
                    _dragType = DRAG_DIR_FILES; // Drop files
                else
                {
                    QFileInfo info(FileHelper::getFilePath(str));
                    if (info.isDir())
                        _dragType = DRAG_DIR_FILES; // Drop directory
                    else
                        _dragType = DRAG_FILE; // Drop file
                }
            }
            else
                _dragType = DRAG_URL; // Is url
        }
        else
        {
            if (linkRx.exactMatch(mimeData->text()))
                 _dragType = DRAG_URL; // Is url
            else
            {
                if (mimeData->hasHtml())
                    _dragType = DRAG_HTML; // Html
                else
                    if (mimeData->hasText())
                        _dragType = DRAG_TEXT; // Text
            }
        }
    }

    event->accept();
}

void AbstractDeviceView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void AbstractDeviceView::dropEvent(QDropEvent *event)
{
    event->accept();
}

void AbstractDeviceView::sendFiles(const QList<QUrl> &urls, DataType type)
{
    LogManager::appendLine("[Server] [" + _name + " -> File(s) drop] "
                           + QString::number(urls.size())
                           + " files");
    foreach (QUrl url, urls)
        LogManager::appendLine("[Server] [File] " + url.toString());
    emit sendFileSignal(getDeviceUID(), urls, type);
}

void AbstractDeviceView::sendUrl(const QString &url, DataType type)
{
    LogManager::appendLine("[Server] [" + _name + " -> URL drop] "
                           + url);
    emit sendTextSignal(getDeviceUID(), url, type);
}

void AbstractDeviceView::sendHtml(const QString &html, DataType type)
{
    LogManager::appendLine("[Server] [" + _name + " -> Html drop] " + html);
    emit sendTextSignal(getDeviceUID(), html, type);
}

void AbstractDeviceView::sendText(const QString &txt, DataType type)
{
    LogManager::appendLine("[Server] [" + _name + " -> Text drop] " + txt);
    emit sendTextSignal(getDeviceUID(), txt, type);
}

void AbstractDeviceView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

QString AbstractDeviceView::getDeviceName() const
{
    return _name;
}

QString AbstractDeviceView::getDeviceUID() const
{
    return _uid;
}

bool AbstractDeviceView::isAvailable() const
{
    return _available;
}

DataType AbstractDeviceView::defaultActionFor(DragType dragType)
{
    DataType type;

    switch(dragType)
    {
    case DRAG_FILE:
        type = TYPE_FILE_OPEN;
        break;
    case DRAG_TEXT:
        type = TYPE_TEXT;
        break;
    case DRAG_HTML:
        type = TYPE_TEXT;
        break;
    case DRAG_DIR_FILES:
         type = TYPE_FILE_SAVE;
        break;
    case DRAG_URL:
        type = TYPE_URL_OPEN;
        break;
    }

    return type;
}

void AbstractDeviceView::sendDataForDefault(const QMimeData *mimeData)
{
    DataType type = AbstractDeviceView::defaultActionFor(_dragType);;

    switch(_dragType)
    {
    case DRAG_FILE:
        sendFiles(mimeData->urls(), type);
        break;
    case DRAG_HTML:
//        sendHtml(mimeData->html(), type);
        sendHtml(mimeData->text(), type);
        break;
    case DRAG_TEXT:
        sendText(mimeData->text(), type);
        break;
    case DRAG_DIR_FILES:
        sendFiles(mimeData->urls(), type);
        break;
    case DRAG_URL:
        sendUrl(mimeData->text(), type);
        break;
    }
}

void AbstractDeviceView::cancelCurrentTransfert()
{
    emit cancelTransfert(_uid);
}
