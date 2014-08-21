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

#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>
#include <QDesktopWidget>

#include "controller.h"
#include "device.h"
#include "appconfig.h"
#include "helpers/logmanager.h"
#include "helpers/servicehelper.h"
#include "helpers/settingsmanager.h"
#include "threads/clipboardthreadevent.h"
#include "threads/deviceconnectionthreadevent.h"
#include "threads/devicepingthreadevent.h"
#include "threads/devicepongthreadevent.h"
#include "threads/devicecanceltransfertthreadevent.h"

#if defined(Q_OS_WIN)
#include "borderlesswindow.h"
#elif defined(Q_OS_LINUX)

#elif defined(Q_OS_OSX)

#endif

Controller::Controller() :
    _deviceNeedResolve(0),
    _lastTimeFocused(0),
    _service(&_udpDiscovery, this),
    _udpDiscoveryTimer(this)
{
    _service.moveToThread(&_serviceThread);
    _serviceThread.start();

    _view = new View(&_model);
    _bonjourBrowser = new BonjourServiceBrowser(this);

    connect(&_udpDiscovery, SIGNAL(devicesFound(const QList<Device *> &)), this, SLOT(updateDevices(const QList<Device *> &)));
    connect(&_udpDiscovery, SIGNAL(pongReceived(const QString&)), this, SLOT(onPong(const QString &)));

    connect(_bonjourBrowser, SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
            this, SLOT(updateRecords(const QList<BonjourRecord> &)));
    connect(_bonjourBrowser, SIGNAL(error(DNSServiceErrorType)),
            this, SLOT(error(DNSServiceErrorType)));

    connect(_view, SIGNAL(sendFile(const QString&, const QList<QUrl>&, DataType)),
            this, SLOT(onSendFile(const QString&, const QList<QUrl>&, DataType)));
    connect(_view, SIGNAL(sendText(const QString&, const QString&, DataType)),
            this, SLOT(onSendText(const QString&, const QString&, DataType)));
    connect(_view, SIGNAL(cancelTransfert(const QString&)),
            this, SLOT(onCancelTransfert(const QString&)));

    connect(_view, SIGNAL(registerService()),
            &_service, SLOT(serviceRegister()));
    connect(_view, SIGNAL(unregisterService()),
            &_service, SLOT(serviceUnregister()));
    connect(_view, SIGNAL(deleteFromHistory(int)),
            &_service, SLOT(onDeleteFromHistory(int)));
    connect(_view, SIGNAL(clearHistoryTriggered()),
            &_service, SLOT(onClearHistory()));
    connect(_view, SIGNAL(serviceNameChanged()),
            &_service, SLOT(onTimerOut()));
    connect(_view, SIGNAL(cancelIncomingTransfert()),
            &_service, SLOT(deleteFileReset()));

    connect(&_service, SIGNAL(serviceError(ServiceErrorState,bool)),
            _view, SLOT(onServiceError(ServiceErrorState,bool)));
    connect(&_service, SIGNAL(receivingFile(const QString&,int)),
             _view, SLOT(onReceivingFile(const QString&,int)));
    connect(&_service, SIGNAL(receivingFolder(const QString&,int)),
            _view, SLOT(onReceivingFolder(const QString&,int)));
    connect(&_service, SIGNAL(receivingText(const QString&)),
            _view, SLOT(onReceivingText(const QString&)));
    connect(&_service, SIGNAL(receivingUrl(const QString&)),
            _view, SLOT(onReceivingUrl(const QString&)));

    connect(&_model, SIGNAL(newDeviceCreated(Device*)),
            this, SLOT(onNewDeviceCreated(Device*)));
    connect(&_model, SIGNAL(deviceRemoved()),
            _view, SLOT(updateDevices()));

    connect(&_updater, SIGNAL(updateNeeded(const QString&,const QString&)),
            _view, SLOT(onUpdateNeeded(const QString&,const QString&)));

    checkForBonjourState();
    _view->updateDevices();
    _view->onHistoryChanged(_service.getHistory());

    _bonjourBrowser->browseForServiceType(QLatin1String("_fdnd._tcp."));

    connect(_view, SIGNAL(focused()), this, SLOT(onWindowFocused()));
    connect(&_udpDiscoveryTimer, SIGNAL(timeout()), &_udpDiscovery, SLOT(startDiscovery()));
    _udpDiscoveryTimer.start(UDP_DISCOVERY_INTERVAL);
    _udpDiscovery.startDiscovery();

    createSendTo();
}

Controller::~Controller()
{
    _serviceThread.quit();
    _serviceThread.wait();

    clearSendToFolder();

    delete _bonjourBrowser;
    delete _view;
}

void Controller::onPong(const QString &uid)
{
    Device *device = _model.getDeviceByUID(uid);

    if (device)
        QCoreApplication::postEvent(device, new DevicePongThreadEvent());
}

void Controller::checkForBonjourState()
{
    _bonjourState = ServiceHelper::getBonjourServiceState();

    _view->setBonjourState(_bonjourState);
}

void Controller::startView(QApplication *application)
{
    QRect geometry = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(_view));
    QSize size(775, 425);

    if (SettingsManager::isSearchUpdateAtLaunch())
        _updater.checkForUpdate();
    if (SettingsManager::isServiceStartedAtlaunch())
        _view->startService();

#if defined(Q_OS_WIN)
    // Stylesheet
    QFile stylesheetFile(WINDOWS_APP_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        application->setStyleSheet(stylesheet);
    }

    // Create window
    BorderlessWindow *window = new BorderlessWindow(application,
                                                    (geometry.width() - size.width()) / 2,
                                                    (geometry.height() - size.height()) / 2,
                                                    size.width(), size.height(), _view);
    window->setMinimumSize(size.width(), size.height());

    if(!SettingsManager::isStartMinimizedAtlaunch())
        window->show();

#elif defined(Q_OS_LINUX)

#elif defined(Q_OS_OSX)

#endif

}

void Controller::updateRecords(const QList<BonjourRecord> &list)
{
    _model.cleanDetectedBy(DETECTED_BY_BONJOUR);

    foreach(BonjourRecord record, list)
    {
        BonjourServiceResolver *resolver = new BonjourServiceResolver(this);

        ++_deviceNeedResolve;

        connect(resolver, SIGNAL(bonjourRecordResolved(Device*)),
                this, SLOT(updateBonjourDevices(Device*)));
        connect(resolver, SIGNAL(error(DNSServiceErrorType)),
                this, SLOT(error(DNSServiceErrorType)));
        connect(resolver, SIGNAL(finish(BonjourServiceResolver*)),
                this, SLOT(onRevolveEnded(BonjourServiceResolver*)));

        resolver->resolveBonjourRecord(record);
    }
}

void Controller::updateBonjourDevices(Device *device)
{
    if (_deviceNeedResolve < SettingsManager::getMaxDevices())
        _model.addDeviceToList(device);
}

void Controller::onRevolveEnded(BonjourServiceResolver *resolver)
{
    if(--_deviceNeedResolve == 0)
    {
        LogManager::appendLine("[Server] Devices updated by bonjour");

        _model.updateDevices();
        _view->updateDevices();
    }

    resolver->deleteLater();
}

void Controller::updateDevices(const QList<Device *> &list)
{
    LogManager::appendLine("[Controller] Devices updated by UDP (" + QString::number(list.size()) + ")");
    _model.cleanDetectedBy(DETECTED_BY_UDP);
    foreach(Device *device, list) {
        _model.addDeviceToList(device);
    }
    _model.updateDevices();
    _view->updateDevices();
}

void Controller::clearSendToFolder()
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
    QString sendToFolder = FileHelper::getSendToFolder();
    FileHelper::deleteFileFromDisk(sendToFolder + "/"SEND_TO_NAME);
#endif
}

void Controller::createSendTo()
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
    QString sendToFolder = FileHelper::getSendToFolder();
    QString sendToFilePath = sendToFolder + "/"SEND_TO_NAME;

    clearSendToFolder();
    QFile(QApplication::applicationFilePath()).link(sendToFilePath);
#endif
}

QList<BonjourRecord> Controller::sanitizeRecords(const QList<BonjourRecord> &list)
{
    QList<BonjourRecord> final;

    // Remove our service & Make them unique
    foreach(BonjourRecord record, list)
    {
        if (record.serviceName.split(';').size() >= 3)
        {
            if (!(record.serviceName.split(';').at(2).split(' ').at(0) == SettingsManager::getDeviceUID())
                    && !listContainsRecord(final, record))
                final.push_back(record);
        }
    }

    return final;
}

bool Controller::listContainsRecord(const QList<BonjourRecord> &list, const BonjourRecord &record)
{
    foreach(BonjourRecord rec, list)
    {
        if (rec.serviceName.split(';').at(2).split(' ').at(0) == record.serviceName.split(';').at(2).split(' ').at(0))
            return true;
    }

    return false;
}

void Controller::onNewDeviceCreated(Device *device)
{
    LogManager::appendLine(QString("[Server] New device created " + device->getName()));

    connect(device, SIGNAL(deviceDoNotRespond(Device*)),
            this, SLOT(onDeviceNotResponding(Device*)));

    connect(device, SIGNAL(deviceUnavailable(const QString&, TransfertState)),
            _view, SLOT(onDeviceUnavailable(const QString&, TransfertState)));

    connect(device, SIGNAL(deviceAvailable(const QString&, TransfertState)),
            _view, SLOT(onDeviceAvailable(const QString&, TransfertState)));

    connect(device, SIGNAL(progressUpdated(const QString&, const QString&, unsigned)),
            _view, SLOT(onProgressUpdated(const QString&, const QString&, unsigned)));

    connect(device, SIGNAL(fileTooBig()),
            _view, SLOT(onFileTooBig()));

    connect(device, SIGNAL(displayMessage(MessageType, const QString&)),
            _view, SLOT(onDisplayMessage(MessageType, const QString&)));
}

void Controller::onDeviceNotResponding(Device *device)
{
    if (device->isDetectedBy(DETECTED_BY_BONJOUR))
    {
        BonjourServiceReconfirmer *reconfirmer = new BonjourServiceReconfirmer(this);
        reconfirmer->reconfirmBonjourRecord(device->getBonjourRecord());
    }

    _model.onDeviceNotResponding(device);
}

void Controller::connectToDevice(Device *device)
{
    if(device != NULL)
    {
        device->tryConnect();
        QCoreApplication::postEvent(device, new DeviceConnectionThreadEvent());
        QCoreApplication::postEvent(device, new DevicePingThreadEvent(&_udpDiscovery));
    }
}

unsigned Controller::getDevicesNumbers()
{
    return _model.getDevices().size();
}

void Controller::onSendFile(const QString &uid, const QList<QUrl> &urls, DataType type)
{
    Device *device = _model.getDeviceByUID(uid);
    if (device != NULL)
    {
        DataStruct dataStruct;

        dataStruct._type = type;
        dataStruct._urls = urls;

        device->setDataStruct(dataStruct);
        connectToDevice(device);
    }
}

void Controller::onSendText(const QString &uid, const QString &string, DataType type)
{
    Device *device = _model.getDeviceByUID(uid);
    if (device != NULL)
    {
        DataStruct dataStruct;
        dataStruct._type = type;
        dataStruct._string = string;

        device->setDataStruct(dataStruct);
        connectToDevice(device);
    }
}

void Controller::error(DNSServiceErrorType error)
{
    --_deviceNeedResolve;
    LogManager::appendLine("[Server] MDNS ERROR (" + QString::number(error) + ") - Is the Bonjour service launched ?");
}

bool Controller::event(QEvent *event)
{
    if (event->type() == EVENT_TYPE_CLIPBOARD)
    {
        ClipboardThreadEvent *threadEvent = static_cast<ClipboardThreadEvent*>(event);

        FileHelper::saveToClipboard(threadEvent->Text);

        return true;
    }
    else
        return QObject::event(event);
}

void Controller::onOtherInstance(QString message)
{
    QStringList paramsList = message.split(";");

    LogManager::appendLine("[SingleApplication] Other instance launched " + message);
    _view->onShow();
}

void Controller::onCancelTransfert(const QString &uid)
{
    Device *device = _model.getDeviceByUID(uid);

    if (device)
    {
        QCoreApplication::postEvent(device, new DeviceCancelTransfertThreadEvent());
    }
}

void Controller::onWindowFocused()
{
    uint current = QDateTime::currentDateTime().toTime_t();
    if((current - _lastTimeFocused) > FOCUSED_NETWORK_REFRESH)
    {
        _lastTimeFocused = current;
        _udpDiscovery.startDiscovery();
    }
}

void Controller::onDockClicked()
{
    _view->onShow();
}
