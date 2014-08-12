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

#include <QHostInfo>
#include <QString>
#include <QDataStream>
#include <QDebug>
#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QClipboard>
#include <QUuid>

#include "service.h"
#include "datastruct.h"
#include "helpers/logmanager.h"
#include "helpers/filehelper.h"
#include "helpers/settingsmanager.h"
#include "config/appconfig.h"
#include "txtrecord.h"
#include "helpers/folderzipper.h"
#include "threads/clipboardthreadevent.h"
#include "controller.h"

const QString Service::HistoryFileName = FileHelper::getFileStorageLocation() + "/" + HISTORY_FILE;

Service::Service(UdpDiscovery *discovery, Controller *controller) :
    _socket(0),
    _bDataSize(false),
    _bDataType(false),
    _bUid(false),
    _bName(false),
    _bType(false),
    _bFilename(false),
    _bFileSize(false),
    _progressCounter(0),
    _bonjourRegister(0),
    _tcpServer(this),
    _timer(this),
    _controller(controller)
{
    qRegisterMetaType<HistoryElement>("HistoryElement");
    qRegisterMetaType<QList<HistoryElement> >("QList<HistoryElement>");
    qRegisterMetaType< ServiceErrorState >("ServiceErrorState");

    _socket = new QTcpSocket();
    _udpDiscovery = discovery;

    connect(_udpDiscovery, SIGNAL(needRecord(QHostAddress *)), this, SLOT(sendRecord(QHostAddress *)));

    connect(&_tcpServer, SIGNAL(newConnection()),
            this, SLOT(onNewConnection()));
    connect(&_timer, SIGNAL(timeout()),
            this, SLOT(onTimerOut()));

    deserializeHistory();
}

Service::~Service()
{
    serviceUnregister();
    _timer.stop();
    if (_socket)
    {
        onDeviceDisconnected();
        delete _socket;
    }
}

void Service::sendRecord(QHostAddress *address)
{
    if (isRegistered()) {
        _udpDiscovery->sendRecord(*address);
        delete address;
    }
}

QList<HistoryElement> Service::getHistory()
{
    return _history;
}

void Service::onNewConnection()
{
    manageNewConnection(_tcpServer);
}

void Service::manageNewConnection(QTcpServer &server)
{
    if (!_socket->isOpen())
    {
        delete _socket;
        _socket = server.nextPendingConnection();

        connect(_socket, SIGNAL(readyRead()),
                this, SLOT(onDataReceived()));
        connect(_socket, SIGNAL(disconnected()),
                this, SLOT(onDeviceDisconnected()));
        connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
    }
}

void Service::onTimerOut()
{
    serviceUnregister();
    serviceRegister();
}

void Service::resetService()
{
    _socket->close();
    if (_file.isOpen())
        _file.close();

    _bDataSize = false;
    _bUid = false;
    _bName = false;
    _bType = false;
    _bFilename = false;
    _bDataType = false;
    _bFileSize = false;
    _progressCounter = 0;
    _socketContent.clear();
}

void Service::socketError(QAbstractSocket::SocketError)
{
    LogManager::appendLine("[Service] Socket ERROR - " + _socket->errorString() + " (IP - " + _socket->peerName() + ")");

    deleteFileReset();
}

void Service::deleteFileReset()
{
    removeCurrentFile();
    serializeHistory();
    resetService();
}

void Service::serviceRegister()
{

    if (!isRegistered())
    {
        _tcpServer.listen(QHostAddress::Any);
        if (!_tcpServer.isListening())
        {
            LogManager::appendLine("[Service] TcpServer ERROR - Could not listen");
            emit serviceError(CANNOT_LAUNCH_SERVICE, true);
            return;
        }

        _udpDiscovery->announce(_tcpServer.serverPort());
        _bonjourRegister = new BonjourServiceRegister(this);
        connect(_bonjourRegister, SIGNAL(error(DNSServiceErrorType)), this, SLOT(error(DNSServiceErrorType)));

        BonjourRecord br(SettingsManager::getServiceDeviceName(), QLatin1String(SERVICE_TYPE), QLatin1String(BONJOUR_DOMAIN));

        TxtRecord record;
        record.append(QLatin1String(KEY_TYPE), QLatin1String(SettingsManager::getType().toStdString().c_str()));
        record.append(QLatin1String(KEY_UID), SettingsManager::getDeviceUID());
        record.append(QLatin1String(KEY_VERSION), QLatin1String(PROTOCOL_VERSION));
        record.getData();
        _bonjourRegister->registerService(br, record.getData(), _tcpServer.serverPort());
        _timer.start(RESTART_REGISTER_TIMER);
        LogManager::appendLine("[Service] Service registered (UID : " + SettingsManager::getDeviceUID() + ", PORT : " + QString::number(_tcpServer.serverPort()) + ")");
    }
}

bool Service::isRegistered()
{
    return (_bonjourRegister != NULL);
}

void Service::serviceUnregister()
{
    if (isRegistered())
    {
        _udpDiscovery->leave();
        if (_tcpServer.isListening())
            _tcpServer.close();
        delete _bonjourRegister;
        _bonjourRegister = 0;
        _timer.stop();
        LogManager::appendLine("[Service] Service unregistered");
    }
}

void Service::onDeviceDisconnected()
{
    _socket->abort();
    _socket->close();
}

void Service::onDataReceived()
{    
    QDataStream stream(_socket);
    do
    {
        if (!_bUid)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return;
            stream >> _dataUid;
            _bUid = true;
        }
        if (!_bName)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return;
            stream >> _dataName;
            _bName = true;
        }
        if (!_bType)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return;
            stream >> _serverType;
            _bType = true;
        }
        if (!_bDataType)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return;
            stream >> _dataType;
            _bDataType = true;
        }
        if (!_bDataSize)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return;
            stream >> _dataSize;
            _bDataSize = true;
        }

        if (DataStruct::isFileType(DataType(_dataType)))
        {
            if (!readFile())
                return;
        }
        else
        {
            if (!readText())
                return;
        }

    } while(_socket->bytesAvailable() > 2);

    resetService();
}

void Service::sendACK()
{
    if (_socket->isOpen())
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        LogManager::appendLine("[Service] Data received, sending ACK");

        // Data type
        stream << (unsigned)TYPE_ACK;

        _socket->write(data);
    }
}

void Service::sendMessage(MessageType type, const QString &message)
{
    if (_socket->isOpen())
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        LogManager::appendLine("[Service] Send message : " + message);

        // Data type
        stream << (unsigned)TYPE_MESSAGE;
        // Message type
        stream << (unsigned)type;
        // Message
        stream << message;

        _socket->write(data);
    }
}

void Service::sendProgress(unsigned percentage)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (unsigned)TYPE_DOWNLOAD_PROGRESS;
    _socket->write(data);

    data.clear();
    stream.skipRawData(4);

    stream << (unsigned)percentage;
    _socket->write(data);
}

bool Service::readFile()
{
    QDataStream stream(_socket);
    int notifyFactor = NOTIFY_FACTOR;
    QDir receptionDir;
    static QString filename;

    if (_socket->isOpen())
    {
        if (!_bFileSize)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return false;
            stream >> _fileSize;
            _bFileSize = true;
        }

        if (!_bFilename)
        {
            if (_socket->bytesAvailable() < sizeof(unsigned))
                return false;
            stream >> filename;
            _bFilename = true;
            QString tmp = filename;
            tmp.remove(ZIP_EXTENSION);
            _currentHistoryElement = HistoryElement(QDateTime::currentDateTime(), tmp, HISTORY_FILE_FOLDER_TYPE);
            addCurrentElementToHistory();

            if (filename.contains(ZIP_EXTENSION))
                emit receivingFolder(tmp, _fileSize);
            else
                emit receivingFile(filename, _fileSize);
        }

        if(_file.isOpen() && _fileSize != 0
                && (_socket->bytesAvailable() + _file.size()) < _fileSize
                && (_socket->bytesAvailable() + _file.size()) < notifyFactor)
            return false;

        _socketContent = _socket->read(_socket->bytesAvailable());

        if (!_file.isOpen())
        {
            receptionDir.mkpath(SettingsManager::getDestinationFolder());
            _file.setFileName(SettingsManager::getDestinationFolder() + "/" + filename);
            if (!_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                LogManager::appendLine("[Service] File ERROR - Can't create the file");
                emit serviceError(CANNOT_CREATE_FILE, false);
                resetService();
                return true;
            }
        }
        _file.write(_socketContent);
        if (_file.size() > notifyFactor * _progressCounter)
        {
            unsigned progress = (_file.size() * 100) / _fileSize;
            sendProgress(progress);
            emit historyElementProgressUpdated(progress);
            ++_progressCounter;
        }

        if (_file.size() < _fileSize)
            return false;

        _file.close();

        decompressFolder(filename);

        emit historyElementProgressUpdated(100);
        serializeHistory();
        LogManager::appendLine("[Service] [FILE] " + filename + " written");

        if (_dataType == TYPE_FILE_OPEN && SettingsManager::isAutoOpenFilesEnabled())
        {
            QFileInfo fileInfo(SettingsManager::getDestinationFolder() + "/" + filename);
            FileHelper::openURL("file:///" + fileInfo.absoluteFilePath());
        }

        _bFileSize = false;
        _bFilename = false;
        _progressCounter = 0;
        _socketContent.clear();

        sendACK();
    }

    return (--_dataSize <= 0);
}

void Service::decompressFolder(QString &filename)
{
    if (filename.endsWith(ZIP_EXTENSION))
    {
        QString filePath = SettingsManager::getDestinationFolder() + "/" + filename;
        QString folderPath = filePath;

        folderPath.remove(ZIP_EXTENSION);
        LogManager::appendLine("[Server] Unzipping directory (" + folderPath + ")");
        FolderZipper::decompressFolder(filePath, folderPath);
        FileHelper::deleteFileFromDisk(filename);

        filename.remove(ZIP_EXTENSION);
    }
}

void Service::addCurrentElementToHistory()
{
    if (_history.size() >= MAX_HISTORY_SIZE)
        _history.removeLast();

    _history.push_front(_currentHistoryElement);
    emit historyChanged(_history);
}

void Service::onDeleteFromHistory(int row)
{
    if (_history.size() >= row)
        _history.removeAt(row);

    serializeHistory();
}

void Service::removeCurrentFile()
{
    removeCurrentFileFromHistory();
    if (_file.isOpen())
    {
        _file.close();
        if (_file.size() < _fileSize)
            FileHelper::deleteFileFromDisk(_file);
    }
}

void Service::removeCurrentFileFromHistory()
{
    _history.removeFirst();
    emit historyChanged(_history);
}

void Service::onClearHistory()
{
    _history.clear();
    serializeHistory();
}

void Service::serializeHistory()
{
    QFile file(HistoryFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QDataStream out(&file);
        out << _history;
        file.close();
    }
}

void Service::deserializeHistory()
{
    QFile file(HistoryFileName);

    // Check the history version
    if (SettingsManager::getHistoryVersion() != APP_HISTORY_VERSION)
    {
        // Delete history file
        if (file.exists())
        {
            FileHelper::deleteFileFromDisk(file);
        }
        // Set the new version of the history protocol
        SettingsManager::setHistoryVersion(APP_HISTORY_VERSION);
    }
    else
    {
        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> _history;
            file.close();

            checkForHistoryExistingFiles();
        }
    }
}

void Service::checkForHistoryExistingFiles()
{
    QList<HistoryElement> toRemove;

    foreach (HistoryElement elt, _history)
    {
        if (elt.isFile() && !FileHelper::exists(elt.getText()))
            toRemove.append(elt);
    }

    if (!toRemove.isEmpty())
    {
        foreach (HistoryElement elt, toRemove)
        {
            _history.removeOne(elt);
        }

        serializeHistory();
    }
}

bool Service::readText()
{
    QDataStream stream(_socket);
    QString text;

    if(_socket->bytesAvailable() < _dataSize)
        return false;

    stream >> text;

    // Notify the controller for a clipboard save
    QApplication::postEvent(_controller, new ClipboardThreadEvent(text));

    if (_dataType == TYPE_URL_OPEN)
    {
        emit receivingUrl(text);
        LogManager::appendLine("[Service] [URL] '" + text + "' opened");
        if (SettingsManager::isAutoOpenFilesEnabled())
            FileHelper::openURL(text);

        _currentHistoryElement = HistoryElement(QDateTime::currentDateTime(), text, HISTORY_URL_TYPE);
    }
    else
    {
        emit receivingText(text);
        LogManager::appendLine("[Service] [TEXT] '" + text + "' saved into clipboard");

        _currentHistoryElement = HistoryElement(QDateTime::currentDateTime(), text, HISTORY_TEXT_TYPE);
    }

    addCurrentElementToHistory();

    sendACK();

    return true;
}

void Service::error(DNSServiceErrorType error)
{
    LogManager::appendLine("[Service] MDNS ERROR (" + QString::number(error) + ") - Is the Bonjour service launched ?");
}
