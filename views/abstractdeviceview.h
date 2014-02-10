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

#ifndef ABSTRACTDEVICEVIEW_H
#define ABSTRACTDEVICEVIEW_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>

#include "device.h"
#include "../helpers/filehelper.h"

enum DragType
{
    DRAG_FILE,
    DRAG_TEXT,
    DRAG_DIR_FILES,
    DRAG_URL,
    DRAG_HTML
};

/**
  * @class AbstractDeviceView
  *
  * Abstract view for a device
  */
class AbstractDeviceView : public QWidget
{
    Q_OBJECT
public:
    /**
      * Constructor
      *
      * @param name Label to display
      * @param uid The uid of the device
      * @param type Device type for the device icon
      * @param available Defines if the device is available or not
      * @param parent Device's parent widget
      */
    explicit AbstractDeviceView(const QString &name, const QString &uid, bool available, QWidget *parent);
    /**
      * Destructor
      */
    virtual ~AbstractDeviceView();

    /**
      * @overload QWidget::dragEnterEvent(QDragEnterEvent *event)
      */
    virtual void dragEnterEvent(QDragEnterEvent *event);
    /**
      * @overload QWidget::dragMoveEvent(QDragMoveEvent *event)
      */
    virtual void dragMoveEvent(QDragMoveEvent *event);
    /**
      * @overload QWidget::dragLeaveEvent(QDragLeaveEvent *event)
      */
   virtual  void dragLeaveEvent(QDragLeaveEvent *event);
    /**
      * @overload QWidget::dropEvent(QDropEvent *event)
      */
    virtual void dropEvent(QDropEvent *event);

    /**
      * Get the device name from the view
      *
      * @return The text on the name label
      */
    virtual QString getDeviceName() const;
    /**
      * Get the device uid from the view
      *
      * @return The UId of the device
      */
    virtual QString getDeviceUID() const;
    /**
      * Getter : _available
      *
      * @return Device availability
      */
    virtual bool isAvailable() const;

    /**
      * Set the device name on the view
      *
      * @param name Name to display
      */
    virtual void setDeviceName(const QString &name) = 0;

    /**
      * Defines if the device is available or not
      *
      * @param available Availability
      * @param state Last transfert state
      */
    virtual void setAvailable(bool available, TransfertState state = NOSTATE) = 0;

signals:
    /**
      * Send file request
      *
      * @param filename The name of the file
      * @param urls List of urls
      * @param type Type of action to perform on data reception
      */
    void sendFileSignal(const QString &name, const QList<QUrl> &urls, DataType type);
    /**
      * Send text request
      *
      * @param string The string
      * @param name Name of the device
      * @param type Type of action to perform on data reception
      */
    void sendTextSignal(const QString &name, const QString &string, DataType type);
    /**
     * Notify the controller for the transfert interuption
     */
    void cancelTransfert(const QString &uid);

protected:
    /// Device name
    QString _name;
    /// Device UID
    QString _uid;
    /// Device availability
    bool _available;
    /// Kind of data dragged
    DragType _dragType;

    /**
     * Send files request
     *
     * @param urls Files path to send
     * @param type Action to perform
     */
    void sendFiles(const QList<QUrl> &urls, DataType type);
    /**
     * Send url request
     *
     * @param url Url to send
     * @param type Action to perform
     */
    void sendUrl(const QString &url, DataType type);
    /**
     * Send html request
     *
     * @param html Html to send
     * @param type Action to perform
     */
    void sendHtml(const QString &html, DataType type);
    /**
     * Send text request
     *
     * @param txt Text to send
     * @param type Action to perform
     */
    void sendText(const QString &txt, DataType type);
    /**
     * Retrieve the default data type for the specified drag type
     *
     * @param type Drag type
     * @return The corresponding default action
     */
    DataType defaultActionFor(DragType dragType);
    /**
     * Send data for default values
     *
     * @param mimeData Data
     */
    void sendDataForDefault(const QMimeData *mineData);
    /**
     * Cancel the current transfert
     */
    void cancelCurrentTransfert();
};

#endif // ABSTRACTDEVICEVIEW_H
