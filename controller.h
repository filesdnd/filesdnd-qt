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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpSocket>
#include <QHostInfo>

#include "zeroconf/bonjourservicebrowser.h"
#include "zeroconf/bonjourserviceresolver.h"
#include "zeroconf/bonjourservicereconfirmer.h"
#include "zeroconf/bonjourrecord.h"
#include "helpers/servicehelper.h"
#include "views/view.h"
#include "entities/service.h"
#include "model.h"
#include "udp/udpdiscovery.h"
#include "threads/servicethread.h"
#include "threads/devicethread.h"
#include "updatemanager.h"

/**
  * @class Controller
  *
  * MVC Controller
  * Handle the mdns protocol to retrieve near devices
  */
class Controller : public QObject
{
    Q_OBJECT
public:
    /**
      * Constructor
      */
    Controller();
    /**
      * Destructor
      */
    ~Controller();
    /**
      * Show the main view
      */
    void startView();
    /**
      * Get the numbers of devices registred in the model
      * Principaly for unit tests
      *
      * @return Number of devices available
      */
    unsigned getDevicesNumbers();
    /**
      * Check the state of the bonjour service
      */
    void checkForBonjourState();
    /**
     * Sanitize records to make the list unique and remove our own service
     *
     * @param list Service list to sanitize
     * @return Sanitized list
     */
    QList<BonjourRecord> sanitizeRecords(const QList<BonjourRecord> &list);
    /**
     * Check if a record belong to a list
     *
     * @param list List to check
     * @param record Record searched
     * @return True if the record belong to the list, false otherwise
     */
    bool listContainsRecord(const QList<BonjourRecord> &list, const BonjourRecord &record);
    /**
     * Thread events handler
     */
    bool event(QEvent *event);

public slots:
    /**
      * SLOT : On new device created by the model
      * Bind the device for GUI interactions
      *
      * @param device Device created to bind
      */
    void onNewDeviceCreated(Device *device);
    /**
     * SLOT : Log the MDNS error
     *
     * @param error MDNS error description
     */
    void error(DNSServiceErrorType error);
    /**
     * Decrement the number of devices that need to be resolved
     *
     * @param resolver Resolver finished
     */
    void onRevolveEnded(BonjourServiceResolver *resolver);
    /**
     * On pong response received
     *
     * @param uid Device uid
     */
    void onPong(const QString &uid);
    /**
      * On another instance launched, focus the window
      */
    void onOtherInstance(QString message);
    /**
     * Cancel the current transfert for a device
     *
     * @param uid Uid of the concerned device
     */
    void onCancelTransfert(const QString &uid);
    /**
     * Notify that a device is not responding
     *
     * @param device Device not responding
     */
    void onDeviceNotResponding(Device *device);
    /**
     * FDNDApplication notified that the mac os x dock was clicked
     */
    void onDockClicked();

signals:
    /**
      * Emit an error to the view about the bonjour service
      *
      * @param state Error state of the bonjour service
      */
    void bonjourServiceError(BonjourServiceState state);

private slots:
    /**
      * On bonjour records updated.
      * Create model's records and display them on the view
      *
      * @param list The list of records available
      */
    void updateRecords(const QList<BonjourRecord> &list);

    /**
      * On udp records updated.
      * Create model's records and display them on the view
      *
      * @param list The list of records available
      */
    void updateDevices(const QList<Device *> &list);
    /**
     * A new device is discover by Bonjour
     *
     * @param device Discovered device
     */
    void updateBonjourDevices(Device *device);
    /**
      * On device connection needed (click or drop)
      * TCP connection between the server and the device
      *
      * @param device The device
      */
    void connectToDevice(Device *device);
    /**
      * Send file request from GUI
      *
      * @param uid The UID of the device
      * @param urls List of urls
      * @param type Action to do on data reception
      */
    void onSendFile(const QString &uid, const QList<QUrl> &urls, DataType type);
    /**
      * Send text request from GUI
      *
      * @param uid The UID of the device
      * @param string Text to send
      * @param type Action to do on data reception
      */
    void onSendText(const QString &uid, const QString &string, DataType type);
    /**
     * @brief Triggered when the windows is focused, start a UDP discovery
     */
    void onWindowFocused();
    /**
     * @brief Remove all the files in the Windows send to folder
     */
    void clearSendToFolder();
    /**
     * @brief Update all the files in the Windows send to folder
     */
    void createSendTo();

private:
    /// Device browser using Bonjour protocol
    BonjourServiceBrowser *_bonjourBrowser;
    /// Main view of the application
    View *_view;
    /// Model of the application contains the devices information
    Model _model;
    /// Address resolver by udp
    UdpDiscovery _udpDiscovery;
    /// Service bonjour
    Service _service;
    /// State of the bonjour service
    BonjourServiceState _bonjourState;
    /// Number of devices that need to be resolved
    qint16 _deviceNeedResolve;
    /// Service thread
    ServiceThread _serviceThread;
    /// Update manager
    UpdateManager _updater;
    /// Udp scan timer
    QTimer _udpDiscoveryTimer;
    /// Time in seconds during the last window focus
    uint _lastTimeFocused;
};

#endif // CONTROLLER_H
