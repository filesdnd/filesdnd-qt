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

#ifndef UDPDISCOVERY_H
#define UDPDISCOVERY_H

#include <QString>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QTimer>
#include <QNetworkInterface>

#include "helpers/logmanager.h"
#include "entities/device.h"
#include "config/appconfig.h"
#include "helpers/settingsmanager.h"

enum SocketType
{
    BROADCAST,
    MULTICAST
};

/**
 * @class UdpDiscovery
 *
 * Udp module for device detection
 */
class UdpDiscovery : public QObject
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    UdpDiscovery(QObject *parent = 0);
    /**
     * Destructor
     */
    ~UdpDiscovery();

    /**
     * Send a ping message to the specified address
     *
     * @param message Message to send
     * @param address Address of the device
     */
    void ping(const QString &message, const QHostAddress &address);
    /**
     * Send a pong message to the specified address in response of the ping
     *
     * @param message Message to send
     * @param address Address of the device
     */
    void pong(const QString &message, const QHostAddress &address);

private:
    /**
     * Retrieve the current network interface
     *
     * @param type Type of the socket (broadcast/multicast)
     * @see SocketType
     */
    QNetworkInterface getCurrentNetworkInterface(SocketType type);
    /**
     * Listen on the multicast interface
     */
    void startMulticastListening(QObject *parent);
    /**
     * Listen on the broadcast interface
     */
    void startBroadcastListening(QObject *parent);
    /**
     * Stop listen on any interface
     */
    void stopListening();
    /**
     * Send a message in the specified mode (multicast/broadcast)
     *
     * @param message Message to send
     */
    void sendDatagramToAny(QString message);
    /**
     * Send a message on the broadcast interface
     *
     * @param message Message to send
     */
    void sendDatagramBroadcast(QString message);
    /**
     * Send a message on the multicast interface
     *
     * @param message Message to send
     */
    void sendDatagramMulticast(QString message);
    /**
     * Parse a device string to retrieve a device
     *
     * @param message Message to send
     * @param info Informations about the device
     * @return The corresponding allocated device
     */
    Device *parse(QString message, QHostInfo &info);

    /// Multicast socket
    QUdpSocket *_multicastSocket;
    /// Broadcast socket
    QUdpSocket *_broadcastSocket;
    /// Address of the multicast group
    QHostAddress _groupAddress;
    /// List of detected devices
    QList<Device *> _list;
    /// Timer for device detection
    QTimer _timer;
    /// Udp port for device detection
    quint16 _port;

    /**
     * Send a datagram to the specified address
     *
     * @param message Message to send
     * @param address Address on which the message have to be sent
     */
    void sendDatagram(QString message, QHostAddress address);
    /**
     * Is the address local to the computer
     *
     * @param address Address to parse
     * @return True if the address is local, false otherwise
     */
    bool isLocalAdress(const QHostAddress &address);

public slots:
    /**
     * Annonce the client on the network
     *
     * @param port Server port
     */
    void announce(quint16 port);
    /**
     * Send the device informations on the network
     */
    void sendRecord(QHostAddress address);
    /**
     * Leave the network
     */
    void leave();
    /**
     * On udp datagram pending, handle messages
     *
     * @param socket Socket that should be read
     * @param type Type of the socket
     */
    void processPendingDatagrams(QUdpSocket *socket, SocketType type);
    /**
     * Wait for records
     */
    void getAllRecords();

    /**
     * Start dicover devices
     */
    void startDiscovery();

    void processPendingMulticastDatagrams();
    void processPendingBroadcastDatagrams();
signals:
    /**
     * New devices found
     *
     * @param lst List of found devices
     */
    void devicesFound(const QList<Device *> &lst);
    /**
     * New device found
     *
     * @param device Device found
     */
    void deviceFound(Device device);
    /**
     * Notify for a leaving device
     *
     * @param device Device leaving
     */
    void deviceLeaving(Device *device);
    /**
     * Notify the service that the record is needed on the network
     */
    void needRecord(QHostAddress *addr);
    /**
     * Notify that a pong (response to a ping is received)
     *
     * @param uid Device uid
     */
    void pongReceived(const QString &uid);

};

#endif // UDPDISCOVERY_H
