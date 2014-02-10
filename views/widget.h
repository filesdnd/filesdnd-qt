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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSpacerItem>
#include <QPropertyAnimation>
#include <QMouseEvent>

#include "smalldeviceview.h"

/**
  * @class Widget
  *
  * Frameless windows shown when the main windows is hiden
  */
class Widget : public QObject
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      *
      * @param parent Should be the main window
      */
    explicit Widget(QWidget *parent);
    /**
      * Destructor
      */
    ~Widget();

    /**
     * Fade in all the widgets
     */
    void showWidgets();
    /**
      * Clear the view of the widget
      */
    void clearDevices();
    /**
      * Add a device to the view
      *
      *
      * @param name Label to display
      * @param uid The device UID
      * @param type Device type for the device icon
      * @param available Defines if the device is available or not
      * @param parent Device's parent widget
      */
    void addDevice(const QString &name, const QString &uid, DeviceType type, bool available);
    /**
      * On device unavailable (when a connection is running)
      *
      * @param uid The device UID
      */
    void setDeviceUnavailable(const QString &uid);
    /**
      * On device available (when a connection isn't running)
      *
      * @param uid The device uid
      */
    void setDeviceAvailable(const QString &uid);
    /**
      * On device available or unavailable
      *
      * @param uid The device uid
      * @param available Device availability
      */
    void setDeviceAvailable(const QString &uid, bool available);
    /**
      * Find a graphical device by its name
      *
      * @param uid Device uid
      * @return Device searched or NULL if not found
      */
    SmallDeviceView* getDeviceByUID(const QString& uid) const;
    /**
     * It will set _wasClosed at false
     *
     * It means that the widget can be shown again
     */
    void canBeShown();
    /**
     * Fade out all the widgets
     */
    void hideWidgets();
    /**
     * Update the device list
     *
     * @param devices Device list
     */
    void updateDevices(QList<Device*> devices);
    
private slots:
    /**
      * On file dropped on a device
      *
      * @param name The name of the device
      * @param urls List of urls
      * @param type Action to do on data reception
      */
    void onSendFile(const QString &name, const QList<QUrl> &urls, DataType type);
    /**
      * On file dropped on a device
      *
      * @param name The name of the device
      * @param string The string to send
      * @param type Action to do on data reception
      */
    void onSendText(const QString &name, const QString &string, DataType type);

public slots:
    /**
     * Update the flags of the widget (depending on options)
     */
    void updateWindowFlags();
    /**
     * When a widget is double clicked, display the main view and fade widgets
     */
    void onWidgetDoubleClicked();
    /**
     * When all the widget should be closed (user action on widget context menu)
     */
    void onHideAllWidgets();

signals:
    /**
      * Notify the vie to retrieve its normal size
      */
    void normalSizeRequested();
    /**
      * Link to the view
      *
      * @param name The name of the device
      * @param urls List of urls
      * @param type Action to do on data reception
      */
    void sendFile(const QString &name, const QList<QUrl> &urls, DataType type);
    /**
      * Link to the view
      *
      * @param name The name of the device
      * @param string The string to send
      * @param type Action to do on data reception
      */
    void sendText(const QString &name, const QString &string, DataType type);
    /// Thrown when the widget is double clicked
    void doubleClicked();

private:
    /// List of devices shown
    QList<SmallDeviceView *> _devices;
    /// Parent of each widget, should be the main window
    QWidget *_parent;
};

#endif // WIDGET_H
