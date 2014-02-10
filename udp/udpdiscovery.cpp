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

#include "udpdiscovery.h"

UdpDiscovery::UdpDiscovery(QObject *parent) :
    _multicastSocket(0),
    _broadcastSocket(0)
{
    _port = 0;
    _groupAddress = QHostAddress(MULTICAST_ADDR);
    _timer.setSingleShot(true);
    _timer.setInterval(GET_RECORD_INTERVAL);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(getAllRecords()));

    startMulticastListening(parent);
    startBroadcastListening(parent);
}

UdpDiscovery::~UdpDiscovery()
{
    if(_multicastSocket != NULL)
    {
        if (_multicastSocket->state() == QUdpSocket::BoundState)
            _multicastSocket->leaveMulticastGroup(_groupAddress);
        _multicastSocket->close();
        delete _multicastSocket;
    }

    if (_broadcastSocket != NULL)
    {
        _broadcastSocket->close();
        delete _broadcastSocket;
    }
}

QNetworkInterface UdpDiscovery::getCurrentNetworkInterface(SocketType type)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface retIn;
    foreach(QNetworkInterface interface, interfaces)
    {
        if( (interface.flags() & QNetworkInterface::IsLoopBack) == 0 && (interface.flags() & QNetworkInterface::IsUp) == QNetworkInterface::IsUp
            && (interface.flags() & QNetworkInterface::IsRunning) == QNetworkInterface::IsRunning
            && ((type == MULTICAST && (interface.flags() & QNetworkInterface::CanMulticast) == QNetworkInterface::CanMulticast)
                || (type == BROADCAST && (interface.flags() & QNetworkInterface::CanBroadcast) == QNetworkInterface::CanBroadcast)))
        {
            retIn = interface;
            break;
        }
    }

    return retIn;
}

bool UdpDiscovery::isLocalAdress(const QHostAddress &address)
{
    bool isLocalAddress = false;
    QList<QHostAddress> interfaces = QNetworkInterface::allAddresses();
    foreach(QHostAddress currentAddress, interfaces)
    {
        if(currentAddress == address)
        {
            isLocalAddress = true;
        }
    }
    return isLocalAddress;
}

void UdpDiscovery::startMulticastListening(QObject *parent)
{
    _multicastSocket = new QUdpSocket(parent);

    if(!_multicastSocket->bind(QHostAddress::AnyIPv4, UDP_DISCOVERY_MULTICAST_PORT, QUdpSocket::ShareAddress))
        LogManager::appendLine("[UDP Discovery] Bind problem");

    QNetworkInterface interface = getCurrentNetworkInterface(MULTICAST);
    if(interface.isValid() && _multicastSocket->state() == QAbstractSocket::BoundState)
    {
        _multicastSocket->setMulticastInterface(interface);
        LogManager::appendLine("[UDP Discovery] Start Multicast listening on interface " + interface.humanReadableName());
    }
    else
        LogManager::appendLine("[UDP Discovery] No interface found or bad state");

    if(!_multicastSocket->joinMulticastGroup(_groupAddress))
        LogManager::appendLine(" [UDP Discovery] Join problem");

    connect(_multicastSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingMulticastDatagrams()));
}

void UdpDiscovery::startBroadcastListening(QObject *parent)
{
    LogManager::appendLine("[UDP Discovery] Start Broadcast listening");
    _broadcastSocket = new QUdpSocket(parent);
    _broadcastSocket->bind(UDP_DISCOVERY_BROADCAST_PORT);

    connect(_broadcastSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingBroadcastDatagrams()));
}

void UdpDiscovery::sendDatagramBroadcast(QString message)
{
    QHostAddress address;
    QNetworkInterface ni = getCurrentNetworkInterface(BROADCAST);

    if(ni.isValid())
    {
        foreach (QNetworkAddressEntry entry , ni.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                LogManager::appendLine("[UDP Discovery] Interface " +
                                       entry.broadcast().toString() + " choosen for broadcast (" +
                                       QString::number(ni.addressEntries().size()) + " interfaces)");
                address = entry.broadcast();
                break;
            }
            else
                LogManager::appendLine("[UDP Discovery] Interface " +
                                       entry.broadcast().toString() + " dropped (" +
                                       QString::number(ni.addressEntries().size()) + " interfaces)");
        }
    } else {
        LogManager::appendLine("[UDP Discovery] No interface found");
    }

    LogManager::appendLine("[UDP Discovery] Broadcast send " + message + " to ip " + address.toString());
    _broadcastSocket->writeDatagram(message.toUtf8(), address, UDP_DISCOVERY_BROADCAST_PORT);
}

void UdpDiscovery::sendDatagramMulticast(QString message)
{
    LogManager::appendLine("[UDP Discovery] Multicast send " + message);
    _multicastSocket->writeDatagram(message.toUtf8(), _groupAddress, UDP_DISCOVERY_MULTICAST_PORT);
}

void UdpDiscovery::stopListening()
{
    if(_multicastSocket != NULL)
        _multicastSocket->close();
    if (_broadcastSocket != NULL)
        _broadcastSocket->close();
}

void UdpDiscovery::getAllRecords()
{
    emit devicesFound(_list);
}

void UdpDiscovery::sendDatagramToAny(QString message)
{
    sendDatagramMulticast(message);
    sendDatagramBroadcast(message);
}

void UdpDiscovery::startDiscovery()
{
    _list.clear();

    QString message(PREFIX);
    message.append(ACTION_GET_RECORD);
    sendDatagramToAny(message);

    _timer.start();
}

void UdpDiscovery::ping(const QString &message, const QHostAddress &address)
{
    sendDatagram(message, address);
}

void UdpDiscovery::pong(const QString &message, const QHostAddress &address)
{
    QString pongMessage = message;

    pongMessage.replace(ACTION_PING, ACTION_PONG);
    sendDatagram(pongMessage, address);
}

void UdpDiscovery::processPendingMulticastDatagrams()
{
    processPendingDatagrams(_multicastSocket, MULTICAST);
}

void UdpDiscovery::processPendingBroadcastDatagrams()
{
    processPendingDatagrams(_broadcastSocket, BROADCAST);
}

void UdpDiscovery::processPendingDatagrams(QUdpSocket *socket, SocketType type)
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress hostAdress;
        QHostInfo info;
        QList<QHostAddress> list;
        QString message;
        QString logMessage;

        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &hostAdress);

        if(!isLocalAdress(hostAdress))
        {
            message = QString::fromUtf8(datagram);

            if (type == BROADCAST)
                logMessage = "[UDP Discovery] Received datagram from broadcast : ";
            else
                logMessage = "[UDP Discovery] Received datagram from multicast : ";
            LogManager::appendLine(logMessage + message);

            if(message.startsWith(PREFIX))
            {
                if(message.endsWith(ACTION_ANNOUNCE))
                    startDiscovery();
                else
                {
                    if(message.endsWith(ACTION_GET_RECORD))
                    {
                        if (_port)
                            emit needRecord(new QHostAddress(hostAdress));
                    }
                    else
                    {
                        if(message.endsWith(ACTION_RECORD))
                        {
                            list.append(hostAdress);
                            info.setAddresses(list);
                            Device *device = parse(message, info);
                            if (device)
                            {
                                device->setDetectedBy(DETECTED_BY_UDP);
                                _list.push_back(device);
                            }
                        }
                        else
                        {
                            if (message.contains(ACTION_PING))
                                pong(message, hostAdress);
                            else
                            {
                                if (message.contains(ACTION_PONG))
                                    emit pongReceived(message.split(" ").last());
                                else
                                {
                                    if(message.endsWith(ACTION_LEAVE))
                                        LogManager::appendLine(QString("[UDP Discovery] Device Leave"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

Device* UdpDiscovery::parse(QString message, QHostInfo &info)
{
    Device *device = 0;
    QStringList lst = message.split(';');
    QString name, uid, type, version;
    int port;

    name = lst.at(0);
    name.remove(PREFIX);

    if (lst.size() > 1)
    {
        type = lst.at(1);
        if (lst.size() > 2)
            uid = lst.at(2);
        if (lst.size() > 3)
            version = lst.at(3);
        if (lst.size() > 4)
            port = lst.at(4).toInt();
    }
    if (uid != SettingsManager::getDeviceUID())
        device = new Device(name, type, uid, info, port, version);

    return device;
}

void UdpDiscovery::sendDatagram(QString message, QHostAddress address)
{
    LogManager::appendLine("[UDP Discovery] Send " + message + " to " + address.toString());
    _multicastSocket->writeDatagram(message.toUtf8(), address, UDP_DISCOVERY_MULTICAST_PORT);
}

void UdpDiscovery::announce(quint16 port)
{
    QString message(PREFIX);

    _port = port;
    message.append(ACTION_ANNOUNCE);
    sendDatagramToAny(message);
}

void UdpDiscovery::sendRecord(QHostAddress address)
{
    QString message(PREFIX);

    message.append(SettingsManager::getServiceDeviceName())
            .append(';')
            .append(SettingsManager::getType())
            .append(';')
            .append(SettingsManager::getDeviceUID())
            .append(';')
            .append(QString(PROTOCOL_VERSION))
            .append(';')
            .append(QString::number(_port))
            .append(';').append(ACTION_RECORD);

    sendDatagram(message, address);

    LogManager::appendLine("[UDP Discovery] Send record " + message);
}

void UdpDiscovery::leave()
{
    QString message(PREFIX);

    _port = 0;
    message.append(ACTION_ANNOUNCE);
    sendDatagramToAny(message);
}
