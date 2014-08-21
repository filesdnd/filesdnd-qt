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

#include "deviceview.h"
#include "helpers/settingsmanager.h"
#include "ui_deviceview.h"
#include "helpers/filehelper.h"

#include <QFileInfo>

DeviceView::DeviceView(const QString &name, const QString &uid, DeviceType type, bool available, TransfertState state, unsigned progress, QWidget *parent) :
    AbstractDeviceView(name, uid, available, parent),
    ui(new Ui::DeviceView),
    _state(state)
{
    ui->setupUi(this);
    ui->leftPanel->setVisible(false);
    ui->rightPanel->setVisible(false);
    ui->transfertWidget->setVisible(false);

    _progressTimer.setSingleShot(true);
    connect(&_progressTimer, SIGNAL(timeout()),
            this, SLOT(onProgressTimerOut()));

    switch (type)
    {
    case TYPE_ANDROID:
        ui->backgroundContainer->setStyleSheet("#backgroundContainer"
                                               "{\n"
                                                  "background: url(:/images/devices/android_icon_large.png) no-repeat right top;\n"
                                               "}\n");
        break;
    case TYPE_WINDOWS:
        ui->backgroundContainer->setStyleSheet("#backgroundContainer"
                                               "{\n"
                                                  "background: url(:/images/devices/windows_icon_large.png) no-repeat right top;\n"
                                               "}\n");
        break;
    case TYPE_MAC:
        ui->backgroundContainer->setStyleSheet("#backgroundContainer"
                                               "{\n"
                                                  "background: url(:/images/devices/apple_icon_large_2.png) no-repeat right top;\n"
                                               "}\n");
        break;
    case TYPE_LINUX:
        ui->backgroundContainer->setStyleSheet("#backgroundContainer"
                                               "{\n"
                                                  "background: url(:/images/devices/linux_icon_large.png) no-repeat right top;\n"
                                               "}\n");
        break;
    }

    setAvailable(available, state);
    setDeviceName(name);
    manageWidgetVisibility();

    if (!available)
    {
        ui->progressBar->setValue(progress);
    }
}

DeviceView::~DeviceView()
{
    delete ui;
}

void DeviceView::manageWidgetVisibility()
{
    if (SettingsManager::shouldDisplayWidget(_uid))
    {
        ui->widgetButton->setToolTip(tr("Widget activé"));
        ui->widgetButton->setStyleSheet("QPushButton#widgetButton"
                                        "{"
                                            "background: url(:/images/icons/arrow-out.png) no-repeat right bottom;"
                                        "}");
    }
    else
    {
        ui->widgetButton->setToolTip(tr("Widget désactivé"));
        ui->widgetButton->setStyleSheet("QPushButton#widgetButton"
                                        "{"
                                            "background: url(:/images/icons/arrow-out_white.png) no-repeat right bottom;"
                                        "}");
    }
}

void DeviceView::setAvailable(bool available, TransfertState state)
{
    _available = available;
    setAcceptDrops(available);

    if (available)
        deviceAvailable(state);
    else
        deviceUnavailable(state);
}

TransfertState DeviceView::lastState()
{
    return _state;
}

void DeviceView::deviceAvailable(TransfertState state)
{
    ui->borderContainer->setStyleSheet("#borderContainer"
                                       "{\n"
                                          "border: 3px solid " + SettingsManager::availableDeviceColor() + ";\n"
                                          "border-radius: 10px;\n"
                                       "}\n");

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);
    switch (state)
    {
        case SUCCESS:
            ui->progressBar->setStyleSheet("QProgressBar:horizontal {\n"
                                               "border: 1px solid gray;\n"
                                               "border-radius: 3px;\nbackground: white;\n"
                                           "}\n"
                                           "QProgressBar::chunk:horizontal {\n"
                                                "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 green, stop: 1 white);\n"
                                           "}");
            ui->progressBar->setFormat(tr("Transfert terminé"));
            _progressTimer.start(3000);
            break;
        case CANCELED:
            ui->progressBar->setStyleSheet("QProgressBar:horizontal {\n"
                                               "border: 1px solid gray;\n"
                                               "border-radius: 3px;\nbackground: white;\n"
                                           "}\n"
                                           "QProgressBar::chunk:horizontal {\n"
                                                "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 darkred, stop: 1 white);\n"
                                           "}");
            ui->progressBar->setFormat(tr("Transfert annulé"));
            break;
        default:
            ui->progressBar->setStyleSheet("QProgressBar:horizontal {\n"
                                               "border: 1px solid gray;\n"
                                               "border-radius: 3px;\nbackground: white;\n"
                                           "}\n"
                                           "QProgressBar::chunk:horizontal {\n"
                                                "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 darkred, stop: 1 white);\n"
                                           "}");
            ui->progressBar->setFormat(tr("Echec du transfert"));
    }
    ui->cancelButton->setVisible(false);
}

void DeviceView::deviceUnavailable(TransfertState state)
{
    ui->borderContainer->setStyleSheet("#borderContainer"
                                       "{\n"
                                          "border: 3px solid " + SettingsManager::unavailableDeviceColor() + ";\n"
                                          "border-radius: 10px;\n"
                                       "}\n");
    _progressTimer.stop();
    ui->cancelButton->setVisible(true);

    if (state == CONNECTED)
    {
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);
        ui->progressBar->setStyleSheet("QProgressBar:horizontal {\n"
                                           "border: 1px solid gray;\n"
                                           "border-radius: 3px;\nbackground: white;\n"
                                       "}\n"
                                       "QProgressBar::chunk:horizontal {\n"
                                            "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 green, stop: 1 white);\n"
                                       "}");
        ui->progressBar->setFormat(tr("Authentification ..."));
    } else
    {
        if(state == DIFVERSION)
        {
            ui->cancelButton->setVisible(false);
            ui->progressBar->setMaximum(100);
            ui->progressBar->setValue(0);
            ui->progressBar->setStyleSheet("");
            ui->progressBar->setFormat(tr("Version différente"));
        }
        else
        {
            ui->progressBar->setMaximum(0);
            ui->progressBar->setStyleSheet("");
            ui->progressBar->setFormat(tr("Connexion en cours ..."));
        }
    }
    ui->transfertWidget->setVisible(true);
}

void DeviceView::onProgressTimerOut()
{
    ui->transfertWidget->setVisible(false);
}

void DeviceView::updateProgress(const QString &message, unsigned progress)
{
    ui->progressBar->setValue(progress);
    ui->progressBar->setFormat(message + " - %p%");
}

void DeviceView::setDeviceName(const QString &name)
{
    ui->deviceName->setText(name);
    _name = name;
}

void DeviceView::dragEnterEvent(QDragEnterEvent *event)
{
    AbstractDeviceView::dragEnterEvent(event);

    ui->transfertWidget->setVisible(false);
    updateLabels();
}

void DeviceView::updateLabels()
{
    switch(_dragType)
    {
    case DRAG_FILE:
        refreshPanels(tr("Enregistrer"), tr("Ouvrir"));
        break;
    case DRAG_TEXT:
        refreshPanels(tr("Presse papier"));
        break;
    case DRAG_HTML:
        refreshPanels(tr("Presse papier"));
        break;
    case DRAG_DIR_FILES:
        refreshPanels(tr("Enregistrer"));
        break;
    case DRAG_URL:
        refreshPanels(tr("Presse papier"), tr("Ouvrir"));
        break;
    }
}

void DeviceView::refreshPanels(const QString &left, const QString &right)
{
    setPanelsVisible(true);
    ui->leftLabel->setText(left);
    ui->rightLabel->setText(right);
}

void DeviceView::refreshPanels(const QString &left)
{
    ui->rightPanel->setVisible(false);
    ui->leftPanel->setVisible(true);
    ui->leftLabel->setText(left);
}

bool DeviceView::isMouseOnDevice()
{
    QPoint topLeftDevice = mapToGlobal(QPoint(0,0));
    QPoint cursor = QCursor::pos();
    int offset = 2;

    return ((cursor.x() - offset) >= topLeftDevice.x()
                && (cursor.x() + offset) <= topLeftDevice.x() + width()
                && (cursor.y() - offset) >= topLeftDevice.y()
                && (cursor.y() + offset) <= topLeftDevice.y() + height());
}

bool DeviceView::isMouseOnLeftPanel()
{
    QPoint cursor = QCursor::pos();
    QPoint leftPanelPos = mapToGlobal(ui->leftPanel->pos());

    return !((cursor.x()) < leftPanelPos.x()
            || (cursor.x()) > leftPanelPos.x() + ui->leftPanel->width()
            || (cursor.y()) < leftPanelPos.y()
            || (cursor.y()) > leftPanelPos.y() + ui->leftPanel->height() + ui->bottomFrame->height());
}

bool DeviceView::isMouseOnRightPanel()
{
    QPoint cursor = QCursor::pos();
    QPoint rightPanelPos = mapToGlobal(ui->rightPanel->pos());

    return !((cursor.x()) < rightPanelPos.x()
             || (cursor.x()) > rightPanelPos.x() + ui->rightPanel->width()
             || (cursor.y()) < rightPanelPos.y()
             || (cursor.y()) > rightPanelPos.y() + ui->rightPanel->height() + ui->bottomFrame->height());
}

void DeviceView::dragMoveEvent(QDragMoveEvent *event)
{  
    if (isMouseOnDevice())
    {
        if (isMouseOnLeftPanel())
            dragInLeftPanel();
        else
           dragOutLeftPanel();

        if (isMouseOnRightPanel())
            dragInRightPanel();
        else
            dragOutRightPanel();

        updateLabels();
    }
    else
        setPanelsVisible(false);

    AbstractDeviceView::dragMoveEvent(event);
}

void DeviceView::setPanelsVisible(bool visible)
{
    ui->leftPanel->setVisible(visible);
    ui->rightPanel->setVisible(visible);
}

void DeviceView::dragLeaveEvent(QDragLeaveEvent *event)
{
    setPanelsVisible(false);

    AbstractDeviceView::dragLeaveEvent(event);
}

void DeviceView::dropEvent(QDropEvent *event)
{
    setPanelsVisible(false);
    sendProperData(event->mimeData());

    AbstractDeviceView::dropEvent(event);
}

void DeviceView::sendProperData(const QMimeData *mimeData)
{
    DataType type;
    bool isOnLeftPanel = isMouseOnLeftPanel();
    bool isOnRightPanel = isMouseOnRightPanel();

    switch(_dragType)
    {
    case DRAG_FILE:
        if (isOnLeftPanel)
            type = TYPE_FILE_SAVE;
        else
            if (isOnRightPanel)
                type = TYPE_FILE_OPEN;
            else
                type = AbstractDeviceView::defaultActionFor(_dragType);
        AbstractDeviceView::sendFiles(mimeData->urls(), type);
        break;
    case DRAG_HTML:
        type = AbstractDeviceView::defaultActionFor(_dragType);
//        AbstractDeviceView::sendHtml(mimeData->html(), type);
        AbstractDeviceView::sendHtml(mimeData->text(), type);
        break;
    case DRAG_TEXT:
        type = AbstractDeviceView::defaultActionFor(_dragType);
        AbstractDeviceView::sendText(mimeData->text(), type);
        break;
    case DRAG_DIR_FILES:
        type = AbstractDeviceView::defaultActionFor(_dragType);
        AbstractDeviceView::sendFiles(mimeData->urls(), type);
        break;
    case DRAG_URL:
        if (isOnLeftPanel)
            type = TYPE_TEXT;
        else
            if (isOnRightPanel)
                type = TYPE_URL_OPEN;
            else
                type = AbstractDeviceView::defaultActionFor(_dragType);
        AbstractDeviceView::sendUrl(mimeData->text(), type);
        break;
    }
}

void DeviceView::dragInLeftPanel()
{
    ui->leftPanel->setStyleSheet(
                "#leftPanel"
                "{"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(210, 210, 210,65%), stop:0.5 rgba(0, 40, 0, 65%));"
                    "opacity: 0.5;"
                    "border-top-left-radius: 6px;"
                "}");

    ui->leftLabel->setStyleSheet(
                "font-weight: bold;"
                "color: white;");
}

void DeviceView::dragOutLeftPanel()
{
    ui->leftPanel->setStyleSheet(
                "#leftPanel"
                "{"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(210, 210, 210, 65%), stop:0.5 rgba(0, 0, 0, 65%));"
                    "opacity: 0.5;"
                    "border-top-left-radius: 6px;"
                 "}");

    ui->leftLabel->setStyleSheet(
                "font-weight: bold;"
                "color: white;");
}

void DeviceView::dragInRightPanel()
{
    ui->rightPanel->setStyleSheet(
                "#rightPanel"
                "{"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(210, 210, 210,65%), stop:0.5 rgba(0, 40, 0, 65%));"
                    "opacity: 0.5;"
                    "border-top-right-radius: 6px;"
                "}");

    ui->rightLabel->setStyleSheet(
                "font-weight: bold;"
                "color: white;");
}

void DeviceView::dragOutRightPanel()
{
    ui->rightPanel->setStyleSheet(
                "#rightPanel"
                "{"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(210, 210, 210, 65%), stop:0.5 rgba(0, 0, 0, 65%));"
                    "opacity: 0.5;"
                    "border-top-right-radius: 6px;"
                "}");

    ui->rightLabel->setStyleSheet(
                "font-weight: bold;"
                "color: white;");
}

void DeviceView::on_cancelButton_clicked()
{
    AbstractDeviceView::cancelCurrentTransfert();
}

void DeviceView::on_widgetButton_clicked()
{
    SettingsManager::setWidgetVisibility(_uid, !SettingsManager::shouldDisplayWidget(_uid));
    manageWidgetVisibility();
}
