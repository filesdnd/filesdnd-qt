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

#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QHostInfo>
#include <QTimer>
#include <QTcpSocket>
#include <QThread>
#include <QHostAddress>
#include <QEvent>

#include "bonjourrecord.h"
#include "../entities/datastruct.h"
#include "../helpers/settingsmanager.h"
#include "../threads/devicethread.h"

class UdpDiscovery;

/**
  * @enum DeviceType
  *
  * Define the type of device
  */
enum DeviceType
{
    TYPE_ANDROID,
    TYPE_WINDOWS,
    TYPE_MAC,
    TYPE_LINUX
};

/**
 * @enum MessageType
 *
 * Type of message to display
 */
enum MessageType
{
    MESSAGE_OVERLAY,
    MESSAGE_POPUP
};

/**
  * @enum TransfertState
  *
  * State of the transfert
  */
enum TransfertState
{
    SUCCESS,
    FAIL,
    CONNECTING,
    CONNECTED,
    CANCELED,
    NOSTATE,
    DIFVERSION
};

/**
  * @class Device
  *
  * Device near the server.
  * Could be a computer, a mobile or a tablet.
  */
class Device : public QObject
{
    Q_OBJECT
public:

    /**
      * Constructor
      *
      * @param name The name of the device
      * @param stype The type of the device as a string
      * @param info Connection informations of the device
      * @param port Port to join the device
      */
    Device(QString name, QString stype, QString uid, QHostInfo info, int port, QString version);

    /**
      * Constructor by copy
      *
      * @param device Device to copy
      */
    Device(const Device &device);

    /**
     * Destructor
     */
    ~Device();

    /**
      * Setter : _name
      *
      * @param name The label of the device
      */
    void setName(const QString &name);

    /**
     * Setter : _hostAddress
     * @param address
     */
    void setHostInfo(const QHostInfo &info);

    /**
     * Setter : _port
     * @param port
     */
    void setPort(const int &port);
    /**
      * Getter : _name
      *
      * @return The label of the device
      */
    QString getName() const;
    /**
     * Getter : _uis
     */
    QString getUID() const;
    /**
      * Getter : _type
      *
      * @return The type of the device
      * @see DeviceType
      */
    DeviceType getType() const;
    /**
      * Getter : _stype
      *
      * @return The type of the device in string
      */
    QString getStringType() const;
    /**
      * Getter : _available
      *
      * @return True if the device is available, false otherwise
      */
    bool isAvailable();
    /**
     * Getter : _progress
     *
     * @return The progress of the upload
     */
    unsigned getProgress();
    /**
     * Notify the view of the new connecting state
     */
    void tryConnect();
    /**
      * Check if the device is connected
      *
      * @return True if the device is connected, false otherwise
      */
    bool isConnected();
    /**
     * Getter : _lastState
     */
    TransfertState getLastTransfertState();
    /**
      * Send the next file in the queue
      *
      * @return False is the queue was empty (any file is sent), True, otherwise
      */
    bool sendNextFile();
    /**
      * Try to send the next file. If no file can be sent, emit the end signal
      */
    void trySendNextFile();
    /**
      * Send a list of files
      */
    void sendFiles();
    /**
      * Reset the state of the device in case of failure
      */
    void onTransfertFail();
    /**
     * Getter : _hostInfo
     */
    QHostInfo getHostInfo() const;
    /**
     * Getter : _port
     */
    int getPort() const;
    /**
     * Getter : _version
     */
    QString getVersion() const;
    /**
     * Setter : _version
     *
     * @param
     */
    void setVersion(QString version);
    /**
     * Getter : _detectedBy
     *
     * @return A binary number representing the device detection mode (01 | 10 | 11)
     */
    int getDetectedBy();
    /**
     * Setter : _detectedBy
     *
     * @param det A binary number representing the device detection mode (01 | 10 | 11)
     */
    void setDetectedBy(int det);
    /**
     * Defines if a device is detected by a specified type
     *
     * @param value Detection type
     * @return True if the device is detected by the type, false otherwise
     */
    bool isDetectedBy(int value);
    /**
     * Create the corresponding message for the view display
     *
     * @return Message to display
     */
    QString getDisplayMessage();
    /**
     * Send a ping query to the specified device
     *
     * @param udpDiscovery UdpDiscovery module that can send a ping query
     */
    void ping(UdpDiscovery *udpDiscovery = 0);
    /**
     * React to a pong (Response from ping)
     *
     * @see void ping(UdpDiscovery *udpDiscovery = 0);
     */
    void pong();
    /**
     * Merge current addresses with the addresses of another device
     *
     * @param hostInfo Addressed to merge
     */
    void mergeAddresses(const QHostInfo &hostInfo);
    /**
     * Thread events handler
     */
    bool event(QEvent *event);
    /**
     * Cancel the current transfert
     */
    void cancelTransfert();
    /**
     * Transfert Succeded, set the device available and close de socket
     */
    void transfertSucceded();
    /**
     * Setter : _bonjourRecord
     */
    void setBonjourRecord(const BonjourRecord &record);
    /**
     * Getter : _bonjourRecord
     */
    const BonjourRecord getBonjourRecord() const;

public slots:
    /**
      * On bonjour record resolved
      *
      * @param info Informations about the device (addresses, etc.)
      * @param port Port of the service
      */
    void connectTo();
    /**
      * On connection failed
      *
      * @param socketError Error type
      */
    void socketError(QAbstractSocket::SocketError );
    /**
      * On device connected
      * Send data (file, text)
      */
    void onDeviceConnected();
    /**
      * On device disconnected
      * Close the socket
      */
    void onDeviceDisconnected();
    /**
      * Send the file described in _data on the network by the socket
      */
    void sendFile();
    /**
      * Send the string described in _data on the network by the socket
      * Could be plain, html or even url
      */
    void sendString();
    /**
      * Setter : _data
      *
      * @param dataStruct New data structure
      */
    void setDataStruct(const DataStruct &dataStruct);
    /**
      * On data received
      * Means that the device is available
      */
    void onDataReceived();
    /**
     * Send the uid of the device through the socket
     */
    void sendUid();
    /**
     * Send the name of the device through the socket
     */
    void sendName();

    /**
     * Send the type of the device through the socket
     */
    void sendType();
    /**
     * SLOT : on ping timer out, request another ping or delete the device (if 3 ping failed)
     */
    void onPingTimerOut();

    /**
     * On file bytes written on the socket
     */
    void onBytesWritten(qint64 bytes);

signals:
    /**
      * Notify the view of the unavailability of the device
      *
      * @param name Device name
      * @param state State of the last transfert
      */
    void deviceUnavailable(const QString &name, TransfertState state);
    /**
      * Notify the view of the availability of the device
      *
      * @param name Device name
      * @param state State of the last transfert
      */
    void deviceAvailable(const QString &name, TransfertState state);
    /**
      * Notify the view for the upload state
      *
      * @param uid Device uid
      * @param progress Progress of the download (Percentage)
      */
    void progressUpdated(const QString &message, const QString &uid, unsigned progress);
    /**
     * Notify the view for a file too big
     */
    void fileTooBig();
    /**
     * Tells the model that the device is not responding (he will be deleted soon)
     */
    void deviceDoNotRespond(Device *device);
    /**
     * Notify the view for a message display
     */
    void displayMessage(MessageType messageType, const QString &message);

private:
    /// Label of the device
    QString _name;
    /// Unique UID
    QString _uid;
    /// Device type in string
    QString _stype;
    /// Device type in string
    QString _version;
    /// Device type
    DeviceType _type;
    /// Address by udp
    QHostInfo _hostInfo;
    /// Port by udp
    int _port;
    /// Socket for the real device connection
    QTcpSocket _tcpSocket;
    /// Defines the data to send
    DataStruct _data;
    /// Defines if the device is available
    bool _available;
    /// Percentage of the upload progress
    unsigned _progress;
    /// Next data will be the download progress
    bool _downloadProgress;
    /// Last transfert State
    TransfertState _lastState;
    /// Number of files to be sent
    qint64 _filesToSend;
    /// Binary detection
    int _detectedBy;
    /// Number of pending ping try
    unsigned _pingTry;
    /// Timer for ping request
    QTimer _pingTimer;
    /// Udp discovery module
    UdpDiscovery *_udpDiscovery;
    /// Associated thread
    DeviceThread _thread;
    /// Bonjour record associated to the bonjour detection, may not be setted
    BonjourRecord _bonjourRecord;

    /// Current file size
    qint64 _fileSize;
    /// Bytes sent to the socket for the current file
    qint64 _bytesSent;
    /// Current file that is uploaded
    QFile _currentFile;

    /**
     * Handle the device construction, initialize it
     */
    void handleDeviceConstruction();
    /**
     * Set the device available and notify the view
     */
    void setDeviceAvailable();
    /**
     * Set the device unavailable and notify the view
     */
    void setDeviceUnavailable();
};

#endif // DEVICE_H
