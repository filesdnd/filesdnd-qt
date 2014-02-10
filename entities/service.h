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

#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QFile>
#include <QTcpSocket>

#include "../zeroconf/bonjourserviceregister.h"
#include "../zeroconf/bonjourrecord.h"
#include "historyelement.h"
#include "../config/appconfig.h"
#include "../udp/udpdiscovery.h"

class Controller;

/**
  * @enum ServiceErrorState
  *
  * Define the possible errors for the service
  */
enum ServiceErrorState
{
    CANNOT_CREATE_FILE,
    CANNOT_LAUNCH_SERVICE
};

/**
  * @class Service
  *
  * Service bonjour
  */
class Service : public QObject
{
    Q_OBJECT
public:
    /**
      * Constructor
      */
    Service(UdpDiscovery *discovery, Controller *controller);
    /**
      * Destructor
      */
    ~Service();

    /**
      * Read a file sent by the server
      *
      * @return True if the file is fully read, false else
      */
    bool readFile();
    /**
      * Read a text sent by the server
      *
      * @return True if the text is fully read, false else
      */
    bool readText();
    /**
      * Send ACK to the server
      */
    void sendACK();
    /**
     * Send the progress of the download
     *
     * @param percentage Progress in percent
     */
    void sendProgress(unsigned percentage);
    /**
      * Tells if the service is registered or not
      *
      * @return True is the service is registered, false otherwise
      **/
    bool isRegistered();
    /**
     * Add a element to the history
     */
    void addCurrentElementToHistory();
    /**
     * On error, remove the incorrect file from the history
     */
    void removeCurrentFileFromHistory();
    /**
     * Serialize history as a binary file
     */
    void serializeHistory();
    /**
     * Deserialize history from a binary file
     */
    void deserializeHistory();
    /**
     * Send message to the current opened socket
     *
     * @param type Message type, overlay or popup
     * @param message Message to send
     */
    void sendMessage(MessageType type, const QString &message);
    /**
     * Getter : _history
     */
    QList<HistoryElement> getHistory();
    /**
      * Set the default values for the service
      */
    void resetService();
    /**
     * On socket error, remove the current file if it is not properly written
     */
    void removeCurrentFile();
    /**
     * Decompress folders and all subfolders
     *
     * @param filename Zipped file
     */
    void decompressFolder(QString &filename);
    /**
     * Manage a new connection on a tcp server
     *
     * @param server Tcp server that need to handle connection
     */
    void manageNewConnection(QTcpServer &server);

public slots:
    /**
      * SLOT : Register the service
      */
    void serviceRegister();
    /**
      * SLOT : Unregiter the service
      */
    void serviceUnregister();
    /**
      * SLOT : On data received (File or text)
      */
    void onDataReceived();
    /**
      * SLOT : On new connexion (callback from accept state of the TcpServer)
      */
    void onNewConnection();
    /**
      * SLOT : On device disconnected
      */
    void onDeviceDisconnected();
    /**
     * Timer out, have to register again
     */
    void onTimerOut();
    /**
      * On socket error
      */
    void socketError(QAbstractSocket::SocketError);
    /**
      * On history entry deleted
      */
    void onDeleteFromHistory(int row);
    /**
      * On clear history requested
      */
    void onClearHistory();
    /**
     * SLOT : Log the MDNS error
     *
     * @param error MDNS error description
     */
    void error(DNSServiceErrorType error);
    /**
     * Send the device record on the multicast
     */
    void sendRecord(QHostAddress *address);
    /**
     * Interrupt the current download, delete the file, change history
     */
    void deleteFileReset();

signals:
    /**
     * Notify an history change
     */
    void historyChanged(const QList<HistoryElement> &history);
    /**
     * Notify the view of the download progress
     *
     * @param progress Download progress percentage
     */
    void historyElementProgressUpdated(unsigned progress);
    /**
      * Notify the view for an error in the service
      * This error could be critical, in this case, it means that the service should stop.
      *
      * @param error The error occured in the service
      * @param isCritical True mean that the service will be stopped
      */
    void serviceError(ServiceErrorState error, bool isCritical);
    /**
     * Notify that a file is about to be received
     *
     * @param fileName The name of the file
     * @param fileSize The size of the file
     */
    void receivingFile(const QString &fileName, int fileSize);
    /**
     * Notify that a folder is about to be received
     *
     * @param folderName The name of the folder
     * @param folderSize The size of the folder
     */
    void receivingFolder(const QString &folderName, int folderSize);
    /**
     * Notify that a text is received
     *
     * @param text The text received
     */
    void receivingText(const QString &text);
    /**
     * Notify that a url is received
     *
     * @param text The url received
     */
    void receivingUrl(const QString &texe);

private:
    /// Bonjour service register
    BonjourServiceRegister *_bonjourRegister;
    /// Server for connection to the service
    QTcpServer _tcpServer;
    /// Socket for connectins
    QTcpSocket *_socket;
    /// Boolean for the data size reception
    bool _bDataSize;
    /// Boolean for the data tye reception
    bool _bDataType;
    /// Size of the data to read on the socket
    unsigned _dataSize;
    /// Size of the file
    qint64 _fileSize;
    /// Uid of the server
    QString _dataUid;
    /// Name of the server
    QString _dataName;
    /// Type of the server
    QString _serverType;
    /// Boolean for the uid
    bool _bUid;
    /// Boolean for the name
    bool _bName;
    /// Boolean for the type
    bool _bType;
    /// Boolean for the file size
    bool _bFileSize;
    /// Type of the data
    unsigned _dataType;
    /// Timer for register again each 10 mins
    QTimer _timer;
    /// Received file history
    QList<HistoryElement> _history;
    /// Current history Element
    HistoryElement _currentHistoryElement;
    /// Boolean for file name reception
    bool _bFilename;
    /// Progress modification counter
    unsigned _progressCounter;
    /// Data read from the file
    QByteArray _socketContent;
    /// File to write
    QFile _file;
    /// Udp discovery module
    UdpDiscovery *_udpDiscovery;
    /// Link to the controller (used for events sending)
    Controller *_controller;
    /// History file name
    static const QString HistoryFileName;

    /**
     * Check if the files of the history exist
     * If some file has been removed, the view will be notified
     */
    void checkForHistoryExistingFiles();
};

#endif // SERVICE_H
