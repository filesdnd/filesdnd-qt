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

#include "view.h"
#include "ui_view.h"
#include "helpers/logmanager.h"
#include "helpers/settingsmanager.h"
#include "historyelement.h"
#include "historyelementview.h"
#include "appconfig.h"
#include "progressindicator.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QBitmap>
#include <QDir>
#include <QScrollBar>

View::View(Model *model) :
    ui(new Ui::View),
    _model(model),
    _aboutDialog(this),
    _settingsDialog(0),
    _updateDialog(this),
    _infoWidget(0),
    _lastBonjourState(BONJOUR_SERVICE_OK),
    _trayTimer(this)
{
    ui->setupUi(this);

    _trayTimer.setSingleShot(true);

    QApplication::setActiveWindow(this);
    activateWindow();
    setFocus(Qt::ActiveWindowFocusReason);

    setWindowIcon(QIcon(CONFIG_APP_ICON));
    // Manage widget
    _widget = new Widget(this);
    connect(_widget, SIGNAL(normalSizeRequested()),
            this, SLOT(onShow()));
    connect(_widget, SIGNAL(sendFile(const QString&, const QList<QUrl>&, DataType)),
            this, SLOT(onSendFile(const QString&, const QList<QUrl>&, DataType)));
    connect(_widget, SIGNAL(sendText(const QString&, const QString&, DataType)),
            this, SLOT(onSendText(const QString&, const QString&, DataType)));
    connect(_widget, SIGNAL(doubleClicked()),
            this, SLOT(onShow()));

    // Manage tray icon
    createTrayActions();
    createTrayIcon();

    // Manage settings dialog
    connect(&_settingsDialog, SIGNAL(refreshDevicesAvailability()),
            this, SLOT(onRefreshDevicesAvailability()));
    connect(&_settingsDialog, SIGNAL(serviceNameChanged()),
            this, SLOT(onServiceNameChanged()));
    connect(&_settingsDialog, SIGNAL(updateWidgetFlags()),
            _widget, SLOT(updateWindowFlags()));

    _overlayMessageDisplay = new OverlayMessageDisplay(ui->devicesView);

    // Config panel
    ui->configPanel->setMainView(this);
}

View::~View()
{
    clearGrid();

    delete _overlayMessageDisplay;

    delete _openAction;
    delete _quitAction;
    delete _infoAction;
    delete _settingsAction;
    delete _serviceAction;
    delete _trayIconMenu;
    delete _trayIcon;

    delete _widget;
    delete ui;
}

void View::resizeEvent(QResizeEvent *event)
{
     QWidget::resizeEvent(event);
     if (_overlayMessageDisplay->isVisible())
        _overlayMessageDisplay->refreshGeometry();
}

void View::clearGrid()
{
    foreach (DeviceView *device, _devices) {
        delete device;
    }
    clearCenterInfoWidget();
    _devices.clear();
}

void View::clearCenterInfoWidget()
{
    if (_infoWidget)
    {
        delete _infoWidget;
        _infoWidget = 0;
    }
}

QList<QPair<unsigned, unsigned> > View::getPosition(unsigned size)
{
    QList<QPair<unsigned, unsigned> > positions;
    switch (size)
    {
        case 0:
        case 1:
            positions.push_back(QPair<unsigned, unsigned>(2, 2));
            break;
        case 2:
            positions.push_back(QPair<unsigned, unsigned>(2, 1));
            positions.push_back(QPair<unsigned, unsigned>(2, 3));
            break;
        case 3:
            positions.push_back(QPair<unsigned, unsigned>(1, 1));
            positions.push_back(QPair<unsigned, unsigned>(1, 3));
            positions.push_back(QPair<unsigned, unsigned>(2, 2));
            break;
        case 4:
            positions.push_back(QPair<unsigned, unsigned>(1, 1));
            positions.push_back(QPair<unsigned, unsigned>(1, 3));
            positions.push_back(QPair<unsigned, unsigned>(3, 1));
            positions.push_back(QPair<unsigned, unsigned>(3, 3));
            break;
        case 5:
            positions.push_back(QPair<unsigned, unsigned>(1, 1));
            positions.push_back(QPair<unsigned, unsigned>(1, 3));
            positions.push_back(QPair<unsigned, unsigned>(2, 2));
            positions.push_back(QPair<unsigned, unsigned>(3, 1));
            positions.push_back(QPair<unsigned, unsigned>(3, 3));
            break;
        default:
            unsigned x = 1, y = 0;
            for (unsigned i = 0; i < size; ++i)
            {
                positions.push_back(QPair<unsigned, unsigned>(y, x));
                if (++x == 4)
                {
                    x = 1;
                    ++y;
                }
            }
    }

    return positions;
}

void View::updateDevices()
{
    QList<Device*> devices = _model->getSortedDevices();
    QList<QPair<unsigned, unsigned> > positions = getPosition(devices.size());
    unsigned count = 0;

    clearGrid();
    _widget->clearDevices();

    if (devices.size() != 0)
    {
        foreach(Device *device, devices)
        {
            QPair<unsigned, unsigned> currentPosition = positions.at(count++);
            DeviceView *deviceWidget = new DeviceView(device->getName(), device->getUID(), device->getType(), device->isAvailable(), device->getLastTransfertState(), device->getProgress(), this);

            connect(deviceWidget, SIGNAL(sendFileSignal(QString,const QList<QUrl>&, DataType)),
                    this, SLOT(onSendFile(const QString&, const QList<QUrl>&, DataType)));
            connect(deviceWidget, SIGNAL(sendTextSignal(const QString&, const QString&, DataType)),
                    this, SLOT(onSendText(const QString&, const QString&, DataType)));
            connect(deviceWidget, SIGNAL(cancelTransfert(const QString&)),
                    this, SLOT(onCancelTransfert(const QString&)));

            _devices.push_back(deviceWidget);
            ui->gridLayout->addWidget(deviceWidget, currentPosition.first, currentPosition.second);
        }
        _widget->updateDevices(devices);
    }
    else
    {
        QPair<unsigned, unsigned> currentPosition = positions.at(0);

        if (_lastBonjourState == BONJOUR_SERVICE_OK)
        {
            if (!_infoWidget)
            {
                _infoWidget = new CenterInfoWidget(this);
            }
            _infoWidget->setNoDeviceMode();
            ui->gridLayout->addWidget(_infoWidget, currentPosition.first, currentPosition.second);
        }
        else
            displayBonjourServiceError();
    }

    manageWidgetVisibility();
    updateTrayTooltip();
    updateTrayIcon();
}

void View::updateTrayIcon()
{
    QIcon icon;

    // Gray icons
    if (_devices.isEmpty())
    {
#if defined(Q_OS_MACX)
        icon = QIcon(CONFIG_GREY_WHITE_TRAY_ICON);
#else
        icon = QIcon(CONFIG_GREY_TRAY_ICON);
#endif
    }
    else // Colored icons
    {
#if defined(Q_OS_MACX)
        icon = QIcon(CONFIG_BLACK_WHITE_TRAY_ICON);
#else
        icon = QIcon(CONFIG_TRAY_ICON);
#endif
    }

    _trayIcon->setIcon(icon);
}

void View::onCancelTransfert(const QString &uid)
{
    emit cancelTransfert(uid);
}

void View::setBonjourState(BonjourServiceState state)
{
    _lastBonjourState = state;
}

void View::displayBonjourServiceError()
{
    QString message;
    QPair<unsigned, unsigned> currentPosition = getPosition(1).at(0);

    switch (_lastBonjourState)
    {
    case BONJOUR_SERVICE_FAILED:
        message.append(tr("Une erreur s'est produite dans le logiciel Bonjour de Apple\nLa détection des périphériques risque de ne pas fonctionner"));
        break;
    case BONJOUR_SERVICE_NOT_FOUND:
        message.append(tr("Le logiciel Bonjour de Apple n'est pas installé\nLa détection des périphériques risque de ne pas fonctionner"));
        break;
    case BONJOUR_SERVICE_NOT_STARTED:
        message.append(tr("Le logiciel Bonjour de Apple n'est pas lancé\nLa détection des périphériques risque de ne pas fonctionner"));
        break;
    default:
        break;
    }

    if (!_infoWidget) {
        _infoWidget = new CenterInfoWidget(this);
    }

    _infoWidget->setBonjourErrorMode(message);
    ui->gridLayout->addWidget(_infoWidget, currentPosition.first, currentPosition.second);
}

void View::manageWidgetVisibility()
{
    if (_devices.size() > 0)
    {
        if (!isVisible() && SettingsManager::isTrayEnabled() && SettingsManager::isWidgetEnabled())
            _widget->showWidgets();
        else
            _widget->hideWidgets();
    }
    else
        _widget->hideWidgets();
}

void View::updateTrayTooltip()
{
    QString tooltip;
    QString devicesText;

#if defined(Q_OS_MACX)
    tooltip = "Files Drag & Drop\n";
#else
    tooltip = "Files Drag &&& Drop\n";
#endif

    if (_devices.size() > 0)
        devicesText = tr("Des périphérique sont visibles (") + QString::number(_devices.size()) + ")";
    else
        devicesText = tr("Aucun périphérique visible");

    _infoAction->setText(devicesText);
    tooltip.append(devicesText);

     _trayIcon->setToolTip(tooltip);
}

void View::onSendFile(const QString &uid, const QList<QUrl> &urls, DataType type)
{
    emit sendFile(uid, urls, type);
}

void View::onSendText(const QString &uid, const QString &string, DataType type)
{
    emit sendText(uid, string, type);
}

void View::onDeviceUnavailable(const QString &uid, TransfertState state)
{
    DeviceView *device = getDeviceByUID(uid);

    if (device)
    {
        device->setAvailable(false, state);
    }
    _widget->setDeviceUnavailable(uid);
}

void View::onServiceNameChanged()
{
    emit serviceNameChanged();
}

void View::onRefreshDevicesAvailability()
{
    bool deviceAvailability;
    foreach(DeviceView *device, _devices)
    {
        deviceAvailability = device->isAvailable();

        device->setAvailable(deviceAvailability);
        _widget->setDeviceAvailable(device->getDeviceUID(), deviceAvailability);
    }
}

void View::onDisplayMessage(MessageType messageType, const QString &message)
{
    switch (messageType)
    {
    case MESSAGE_OVERLAY:
        _overlayMessageDisplay->setText(message);
        break;
    case MESSAGE_POPUP:
        QMessageBox::information(this, tr("Information"), message);
        break;
    }
}

void View::onDeviceAvailable(const QString &uid, TransfertState state)
{
    DeviceView *device = getDeviceByUID(uid);

    if (device)
    {
        device->setAvailable(true, state);
    }

    _widget->setDeviceAvailable(uid);
}

void View::onProgressUpdated(const QString &message, const QString &uid, unsigned progress)
{
    DeviceView *device = getDeviceByUID(uid);

    if (device)
    {
        device->updateProgress(message, progress);
    }
}

DeviceView* View::getDeviceByUID(const QString& uid) const
{
    foreach (DeviceView *device, _devices)
    {
        if(device->getDeviceUID() == uid)
        {
            return device;
        }
    }
    return NULL;
}

void View::createTrayActions()
{
    QFont font;

    _trayIconMenu = new QMenu(this);

    _openAction = new QAction(tr("Ouvrir Files Drag &&& Drop"), _trayIconMenu);
    font = _openAction->font();
    font.setBold(true);
    _openAction->setFont(font);
    connect(_openAction, SIGNAL(triggered()), this, SLOT(onShow()));
    _openAction->setMenuRole(QAction::NoRole);

    _quitAction = new QAction(tr("Quitter"), _trayIconMenu);
    connect(_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    _quitAction->setMenuRole(QAction::NoRole);

    _settingsAction = new QAction(tr("Paramètres"), _trayIconMenu);
    connect(_settingsAction, SIGNAL(triggered()), this, SLOT(onSettingsActionTriggered()));
    _settingsAction->setMenuRole(QAction::NoRole);

    _serviceAction = new QAction(tr("Réception"), _trayIconMenu);
    _serviceAction->setCheckable(true);
    connect(_serviceAction, SIGNAL(triggered()), this, SLOT(onServiceTriggered()));
    _serviceAction->setMenuRole(QAction::NoRole);

    _infoAction = new QAction(tr("Aucun périphérique visible"), _trayIconMenu);
    _infoAction->setEnabled(false);
    _infoAction->setMenuRole(QAction::NoRole);

    _trayIconMenu->addAction(_openAction);
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(_serviceAction);
    _trayIconMenu->addAction(_settingsAction);
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(_infoAction);
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(_quitAction);
}

void View::onSettingsActionTriggered()
{
    _settingsDialog.show();
}

void View::closeEvent(QCloseEvent *event)
{
    if (_trayIcon->isVisible())
    {
        emit hideWindow();
        hide();
        manageWidgetVisibility();
        event->ignore();
    }
    else
        qApp->quit();
}

void View::createTrayIcon()
{
    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setContextMenu(_trayIconMenu);

    updateTrayIcon();

#if defined(Q_OS_MACX)
    _trayIcon->setToolTip(tr("Files Drag & Drop\nAucun périphérique visible"));
#else
    _trayIcon->setToolTip(tr("Files Drag &&& Drop\nAucun périphérique visible"));
#endif

    // Manage Tray icon
    connect(&_settingsDialog, SIGNAL(trayDisabled()),
            this, SLOT(onTrayDisabled()));
    connect(&_settingsDialog, SIGNAL(trayEnabled()),
            this, SLOT(onTrayEnabled()));
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(_trayIcon, SIGNAL(messageClicked()),
            this, SLOT(onShow()));
    connect(&_settingsDialog, SIGNAL(widgetStateChanged()),
            this, SLOT(manageWidgetVisibility()));

    if (SettingsManager::isTrayEnabled())
        _trayIcon->show();
}

void View::onTrayDisabled()
{
    _trayIcon->hide();
    qApp->setQuitOnLastWindowClosed(true);
}

void View::onTrayEnabled()
{
    _trayIcon->show();
    qApp->setQuitOnLastWindowClosed(false);
}

void View::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            onShow();
            break;
        case QSystemTrayIcon::MiddleClick:
            break;
    }
}

void View::onShow()
{
    if (SettingsManager::isWidgetEnabled())
        _widget->hideWidgets();

    _widget->canBeShown();
    activateWindow();
    QApplication::alert(this);

    show();
    emit showWindow();
}

void View::onServiceTriggered()
{
    if (!_serviceAction->isChecked())
    {
        stopService();
    }
    else
    {
        startService();
    }
}

void View::onServiceError(ServiceErrorState error, bool isCritical)
{
    QString message;

    switch (error)
    {
    case CANNOT_CREATE_FILE:
        message.append(tr("Impossible de créer le fichier reçu.\n"));
        message.append(tr("Vérifiez que vous avez les droits d'écrire dans le dossier de destination."));
        break;
    case CANNOT_LAUNCH_SERVICE:
        message.append(tr("Impossible de démarrer le client."));
        break;
    }

    QMessageBox::warning(this, tr("Le client à rencontré une erreur"), message);

    if (isCritical)
        stopService();
}

void View::stopService()
{
    _serviceAction->setToolTip(tr("Activer la réception"));
    _serviceAction->setChecked(false);

    updateTrayTooltip();

    emit unregisterService();
}

void View::startService()
{
    showTrayMessage(tr("Files Drag & Drop est actif"));

    _serviceAction->setToolTip(tr("Arrêter la réception"));
    _serviceAction->setChecked(true);

    updateTrayTooltip();

    emit registerService();
}

void View::onFileTooBig()
{
    QMessageBox::warning(this, tr("Echec de l'envoi"), tr("Le fichier est trop volumineux pour le périphérique."));
}

void View::onUpdateNeeded(const QString &version, const QString &note)
{
    _updateDialog.updateAndShow(version, note);
}

void View::onCancelIncomingTransfert()
{
    emit cancelIncomingTransfert();
}

void View::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    emit focused();
}

void View::onHistoryChanged(const QList<HistoryElement> &history)
{
    ui->configPanel->onHistoryChanged(history);
}

void View::showTrayMessage(const QString &message)
{
    int timerInterval = 1000;

    if (!_trayTimer.isActive())
    {
        _trayIcon->showMessage("Files Drag & Drop",
                               message,
                               QSystemTrayIcon::Information,
                               timerInterval);
        _trayTimer.start(timerInterval);
    }
}

void View::onReceivingFile(const QString &fileName, int fileSize)
{
    if (!isVisible() || isMinimized())
        showTrayMessage(tr("Réception du fichier : ") + fileName + " (" + FileHelper::getSizeAsString(fileSize) + ")");
}

void View::onReceivingFolder(const QString &folderName, int folderSize)
{
    if (!isVisible() || isMinimized())
        showTrayMessage(tr("Réception du dossier : ") + folderName + " (" + FileHelper::getSizeAsString(folderSize) + ")");
}

void View::onReceivingUrl(const QString &url)
{
    if (!isVisible() || isMinimized())
        showTrayMessage(tr("Réception d'une URL : ") + url);
}

void View::onReceivingText(const QString &text)
{
    if (!isVisible() || isMinimized())
        showTrayMessage(tr("Réception d'un texte : ") + text);
}
