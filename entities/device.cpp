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

#include "device.h"
#include "../helpers/logmanager.h"
#include "appconfig.h"
#include "../helpers/filehelper.h"
#include "../udp/udpdiscovery.h"
#include "../helpers/folderzipper.h"
#include "../threads/deviceconnectionthreadevent.h"
#include "../threads/devicepingthreadevent.h"
#include "../threads/devicepongthreadevent.h"

#include <QStringList>
#include <QDataStream>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QByteArray>
#include <QHostAddress>
#include <QStandardPaths>
#include <QDir>
#include <QTime>
#include <QFileInfo>

Device::Device(QString name, QString stype, QString uid, QHostInfo info, int port, QString version) :
    _name(name),
    _stype(stype),
    _version(version),
    _uid(uid),
    _hostInfo(info),
    _available(true),
    _port(port),
    _progress(0),
    _downloadProgress(false),
    _lastState(NOSTATE),
    _pingTry(3),
    _pingTimer(this),
    _tcpSocket(this)
{
    if (stype.contains(TYPE_STRING_ANDROID))
        _type = TYPE_ANDROID;
    else if (stype.contains(TYPE_STRING_WINDOWS))
        _type = TYPE_WINDOWS;
    else if (stype.contains(TYPE_STRING_MAC))
        _type = TYPE_MAC;
    else
        _type = TYPE_LINUX;

    handleDeviceConstruction();
}

Device::Device(const Device &device) :
    _data(device._data),
    _name(device._name),
    _type(device._type),
    _stype(device._stype),
    _version(device._version),
    _uid(device._uid),
    _available(device._available),
    _hostInfo(device._hostInfo),
    _port(device._port),
    _progress(device._progress),
    _downloadProgress(device._downloadProgress),
    _lastState(device._lastState),
    _detectedBy(device._detectedBy),
    _filesToSend(device._filesToSend),
    _pingTry(device._pingTry),
    _pingTimer(this),
    _tcpSocket(this)
{
    handleDeviceConstruction();
}

Device::~Device()
{
    _pingTimer.stop();
    _thread.quit();
    _thread.wait();
}

void Device::handleDeviceConstruction()
{
    qRegisterMetaType<TransfertState>("TransfertState");
    qRegisterMetaType<MessageType>("MessageType");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    moveToThread(&_thread);
    _thread.start();

    _tcpSocket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    _tcpSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 0);
    _tcpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, 0);
    _tcpSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);

    _pingTimer.setSingleShot(true);
    connect(&_pingTimer, SIGNAL(timeout()), this, SLOT(onPingTimerOut()));

    connect(&_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(&_tcpSocket, SIGNAL(connected()), this, SLOT(onDeviceConnected()));
    connect(&_tcpSocket, SIGNAL(disconnected()), this, SLOT(onDeviceDisconnected()));
    connect(&_tcpSocket, SIGNAL(readyRead()), this, SLOT(onDataReceived()));

    if(_version.compare(PROTOCOL_VERSION) != 0) {
        LogManager::appendLine("[Device] Device protocol version is " + _version + " (current is " PROTOCOL_VERSION + ")");
        _lastState = DIFVERSION;
        setDeviceUnavailable();
    }
}

void Device::setName(const QString &name)
{
    _name = name;
}

void Device::setHostInfo(const QHostInfo &info)
{
    _hostInfo = info;
}

QString Device::getName() const
{
    return _name;
}

DeviceType Device::getType() const
{
    return _type;
}

QString Device::getStringType() const
{
    return _stype;
}

void Device::tryConnect()
{
    _lastState = CONNECTING;
    setDeviceUnavailable();
}

void Device::connectTo()
{
    bool connected = false;
    if (!_tcpSocket.isOpen())
    {
        // We can also use QNetworkInterface::allAddresses() for more details
        QList<QHostAddress> localAddressesList = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
        foreach (QHostAddress qhs, _hostInfo.addresses())
        {
            if (!localAddressesList.contains(qhs))
            {
                LogManager::appendLine("[Server] Try connecting to " + qhs.toString() + ":" + QString::number(_port));
                _tcpSocket.connectToHost(QHostAddress(qhs.toString()), _port, QIODevice::ReadWrite);

                if (_tcpSocket.waitForConnected(2000))
                {
                    LogManager::appendLine("[Server] Connected to " + qhs.toString());
                    connected = true;
                    break;
                }
            }
        }
        if (!connected)
        {
            onTransfertFail();
            LogManager::appendLine("[Server] Connection to device failed, no answer");
        }
    }
}


void Device::socketError(QAbstractSocket::SocketError)
{
    LogManager::appendLine("[Server] Socket ERROR - " + _tcpSocket.errorString() + " (IP - " + _tcpSocket.peerName() + ")");

    onTransfertFail();
}

void Device::onTransfertFail()
{
    _lastState = FAIL;
    setDeviceAvailable();
    _tcpSocket.close();
}

void Device::ping(UdpDiscovery *udpDiscovery)
{
    QString pingMessage;
    QList<QHostAddress> localAddressesList;

    pingMessage.append(PREFIX).append(ACTION_PING).append(_uid);

    if (udpDiscovery)
        _udpDiscovery = udpDiscovery;

    localAddressesList = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
    foreach (QHostAddress qhs, _hostInfo.addresses())
        if (/*qhs.protocol() == QAbstractSocket::IPv4Protocol && */!localAddressesList.contains(qhs))
            _udpDiscovery->ping(pingMessage, qhs);

    _pingTimer.start(PING_INTERVAL);
}

void Device::onPingTimerOut()
{
    if (--_pingTry > 0)
        ping();
    else
    {
        _pingTry = 3;
        if (!isConnected())
            emit deviceDoNotRespond(this);
    }
}

void Device::pong()
{
    _pingTimer.stop();
    _pingTry = 3;
}

void Device::setDeviceAvailable()
{
    if (!_available)
    {
        emit deviceAvailable(_uid, _lastState);
        _available = true;
    }
}

void Device::setDeviceUnavailable()
{
    if (_available || _lastState == CONNECTED)
    {
        emit deviceUnavailable(_uid, _lastState);
        _available = false;
    }
}

void Device::onDeviceConnected()
{
    pong();
    _lastState = CONNECTED;
    setDeviceUnavailable();

    sendUid();
    sendName();
    sendType();

    if (DataStruct::isFileType(_data._type))
        sendFiles();
    else
        sendString();
}

void Device::onDataReceived()
{
    QDataStream stream(&_tcpSocket);
    unsigned dataType;

    if (_tcpSocket.bytesAvailable() < sizeof(unsigned))
        return;

    do
    {
        if (_downloadProgress)
        {
            stream >> _progress;
            emit progressUpdated(getDisplayMessage(), _uid, _progress);
            _downloadProgress = false;

            continue;
        }

        stream >> dataType;

        switch (dataType)
        {
        case TYPE_ACK:
            LogManager::appendLine("[Server] SUCCESS - Ack received");
            _progress = 0;
            if (_data._urls.isEmpty())
                transfertSucceded();
            else
                trySendNextFile();
            break;

        case TYPE_DOWNLOAD_PROGRESS:
            _downloadProgress = true;
            break;

        case TYPE_FILE_TOO_BIG:
            onDeviceDisconnected();
            LogManager::appendLine("[Server] ERROR - File too big");
            _progress = 0;
            onTransfertFail();
            emit fileTooBig();
            break;

        case TYPE_MESSAGE:
            unsigned messageType;
            QString message;

            stream >> messageType;
            stream >> message;

            LogManager::appendLine("[Server] MESSAGE - " + message);

            emit displayMessage(MessageType(messageType), message);
            break;
        }
    }
    while (_tcpSocket.bytesAvailable() >= sizeof(unsigned));
}

QString Device::getDisplayMessage()
{
    QString message;
    QString filename = _data._string.split('/').last();
    int maxFilenameLenght = 15;

    filename.remove(ZIP_EXTENSION);
    message.append(filename.left(maxFilenameLenght));

    if (filename.size() > maxFilenameLenght)
        message.append("...");

    if (_filesToSend > 1)
    {
        message.append(" (");
        message.append(QString::number(_filesToSend - _data._urls.size()));
        message.append("/");
        message.append(QString::number(_filesToSend));
        message.append(")");
    }

    return message;
}

bool Device::isConnected()
{
    return (_tcpSocket.state() == QAbstractSocket::ConnectedState);
}

void Device::sendString()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // Data type
    stream << (unsigned)_data._type;
    // Datagram size
    stream << (unsigned)0;
    // String to send
    stream << _data._string;

    stream.device()->seek(sizeof(unsigned));
    stream << (quint32)(data.size() - (sizeof(unsigned) * 2));

    _tcpSocket.write(data);
}

void Device::sendUid()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // String to send
    stream << SettingsManager::getDeviceUID();

    _tcpSocket.write(data);
}

void Device::sendName()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // String to send
    stream << SettingsManager::getServiceDeviceName();

    _tcpSocket.write(data);
}

void Device::sendType()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // String to send
    stream << QString(SettingsManager::getType());

    _tcpSocket.write(data);
}

bool Device::sendNextFile()
{
    bool canSend = false;
    bool compress = true;

    if (!_data._urls.isEmpty())
    {
        QUrl current = _data._urls.takeFirst();
        _data._string = FileHelper::getFilePath(current.toString());
        LogManager::appendLine("[Server] Sending file " + _data._string);

        QFileInfo file(_data._string);
        if (file.isDir())
        {
            QString dirName = file.fileName();
            QString tmp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
            tmp = QDir::fromNativeSeparators(tmp);
            QString destination = tmp + "/" + dirName + ZIP_EXTENSION;

            LogManager::appendLine("[Server] Zipping directory (" + destination + ")");
            compress = FolderZipper::compressFolder(_data._string, destination);
            _data._string = destination;
        }
        if (compress)
            sendFile();
        else
            return sendNextFile();

        canSend = true;
    }

    return canSend;
}

void Device::sendFiles()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    _filesToSend = _data._urls.size();

    // Data type
    stream << (unsigned)_data._type;
    // Datagram size
    stream << (unsigned)_filesToSend;

    _tcpSocket.write(data);

    trySendNextFile();
}

void Device::trySendNextFile()
{
    if (!sendNextFile())
    {
        transfertSucceded();
    }
}

void Device::transfertSucceded()
{
    _tcpSocket.close();
    _lastState = SUCCESS;
    setDeviceAvailable();
}

void Device::sendFile()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QFile file(_data._string);
    qint64 sent = 0;
    qint64 size = 0;

    if (!file.open(QIODevice::ReadOnly))
    {
        onTransfertFail();
        LogManager::appendLine("[Server] ERROR - Cannot open file " + file.fileName());

        return;
    }

    size = file.size();

    // File size
    stream << size;
    // File name
    stream << file.fileName().split('/').last();

    _tcpSocket.write(data);
    while(sent < size)
    {
        if (!_tcpSocket.isOpen())
            return;
        data = file.read(READ_FILE_BUFFER);
        qint64 wrote = _tcpSocket.write(data);
        sent += wrote;
    }
}

void Device::setDataStruct(const DataStruct &dataStruct)
{
    _data = dataStruct;
}

unsigned Device::getProgress()
{
    return _progress;
}

void Device::onDeviceDisconnected()
{
    _tcpSocket.abort();
    _tcpSocket.close();
}

bool Device::isAvailable()
{
    return _available;
}

QString Device::getUID() const
{
    return _uid;
}

TransfertState Device::getLastTransfertState()
{
    return _lastState;
}

QHostInfo Device::getHostInfo() const {
    return _hostInfo;
}

int Device::getPort() const {
    return _port;
}

void Device::setDetectedBy(int det)
{
    _detectedBy = det;
}

void Device::setPort(const int &port)
{
    _port = port;
}

QString Device::getVersion() const {
    return _version;
}

void Device::setVersion(QString version)
{
    _version = version;
}


bool Device::isDetectedBy(int value)
{
    return (_detectedBy & value);
}

int Device::getDetectedBy()
{
    return _detectedBy;
}


void Device::mergeAddresses(const QHostInfo &hostInfo)
{
    QList<QHostAddress> newAddresses = _hostInfo.addresses();

    foreach (QHostAddress address, hostInfo.addresses())
    {
        if (!newAddresses.contains(address))
            newAddresses.append(address);
    }

    _hostInfo.setAddresses(newAddresses);
}

bool Device::event(QEvent *event)
{
    if (event->type() >= QEvent::User)
    {
        DevicePingThreadEvent *threadEvent;
        switch(event->type())
        {
        case EVENT_TYPE_PING:
            threadEvent = static_cast<DevicePingThreadEvent*>(event);
            ping(threadEvent->UdpComponent);
            break;
        case EVENT_TYPE_CONNECT:
            connectTo();
            break;
        case EVENT_TYPE_PONG:
            pong();
            break;
        case EVENT_TYPE_CANCEL_TRANSFERT:
            cancelTransfert();
            break;
        }
        return true;
    }
    else
        return QObject::event(event);
}

void Device::cancelTransfert()
{
    LogManager::appendLine("[Server] ERROR - Transfert canceled by user");

    _lastState = CANCELED;
    setDeviceAvailable();
    emit _tcpSocket.error(QAbstractSocket::ConnectionRefusedError);
    _tcpSocket.abort();
    _tcpSocket.close();
}

void Device::setBonjourRecord(const BonjourRecord &record)
{
    _bonjourRecord = record;
}

const BonjourRecord Device::getBonjourRecord() const
{
    return _bonjourRecord;
}
