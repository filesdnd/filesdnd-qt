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

#include "../helpers/logmanager.h"
#include "settingsdialog.h"
#include "../helpers/settingsmanager.h"
#include "ui_settingsdialog.h"

#include <QTextStream>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    _availableDevice = new DeviceView(tr("Disponible"), "", TYPE_ANDROID, true, NOSTATE, 0, ui->overView);
    _unavailableDevice = new DeviceView(tr("Non Disponible"), "", TYPE_ANDROID, false, NOSTATE, 0, ui->overView);

    ui->availableDevice->addWidget(_availableDevice);
    ui->unavailableDevice->addWidget(_unavailableDevice);

#if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
    ui->startAtBoot->setVisible(true);
#else
    ui->startAtBoot->setVisible(false);
#endif
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete _availableDevice;
    delete _unavailableDevice;
}

void SettingsDialog::on_trayIconEnabled_toggled(bool checked)
{
    ui->widgetForeground->setEnabled(checked);
    ui->widgetEnabled->setEnabled(checked);
    ui->errorMessage->setVisible(!checked);

    if (!checked)
        emit trayDisabled();
    else
        emit trayEnabled();

    SettingsManager::setTrayIconEnabled(checked);
}

void SettingsDialog::on_widgetEnabled_toggled(bool checked)
{
    ui->widgetForeground->setChecked(checked);
    ui->widgetForeground->setEnabled(checked);

    SettingsManager::setWidgetEnabled(checked);
    emit widgetStateChanged();
}

void SettingsDialog::on_logEnabled_toggled(bool checked)
{
    SettingsManager::setLogEnabled(checked);
}

void SettingsDialog::on_apply_clicked()
{
    close();
}

void SettingsDialog::on_startServiceAtLaunch_toggled(bool checked)
{
    SettingsManager::setStartServiceAtLaunch(checked);
}

void SettingsDialog::refreshSettingsFromFile()
{
    /// Global settings
    ui->startMinimized->setChecked(SettingsManager::isStartMinimizedAtlaunch());
    ui->startAtBoot->setChecked(SettingsManager::shouldStartAtBoot());
    ui->widgetEnabled->setChecked(SettingsManager::isWidgetEnabled());
    ui->widgetForeground->setChecked(SettingsManager::isWidgetInForeground());
    ui->searchUpdateAtLaunch->setChecked(SettingsManager::isSearchUpdateAtLaunch());
    ui->trayIconEnabled->setChecked(SettingsManager::isTrayEnabled());
    on_trayIconEnabled_toggled(SettingsManager::isTrayEnabled());
    ui->logEnabled->setChecked(SettingsManager::isLogEnabled());
    ui->startServiceAtLaunch->setChecked(SettingsManager::isServiceStartedAtlaunch());
    ui->autoOpenFiles->setChecked(SettingsManager::isAutoOpenFilesEnabled());
    ui->historyDisplay->setCurrentIndex(SettingsManager::getHistoryDisplayPolicy());
    ui->serviceDeviceName->setText(SettingsManager::getServiceDeviceName());
    ui->destFolderArea->setText(SettingsManager::getDestinationFolder());

    /// Color settings
    ui->availableColor->setStyleSheet("QPushButton {\n"
                                          "border-radius: 4px;\n"
                                          "background-color: " + SettingsManager::availableDeviceColor() + ";\n"
                                      "}");
    _availableDevice->setAvailable(true);
    ui->unavailableColor->setStyleSheet("QPushButton {\n"
                                          "border-radius: 4px;\n"
                                          "background-color: " + SettingsManager::unavailableDeviceColor() + ";\n"
                                      "}");
    _unavailableDevice->setAvailable(false);

    /// Widget settings
    if (!ui->widgetEnabled->isChecked())
    {
        ui->widgetForeground->setChecked(false);
        ui->widgetForeground->setEnabled(false);
    }
}

void SettingsDialog::showEvent(QShowEvent* event)
{
    refreshSettingsFromFile();

    QWidget::showEvent(event);
}

void SettingsDialog::on_autoOpenFiles_toggled(bool checked)
{
    SettingsManager::setAutoOpenFiles(checked);
}

void SettingsDialog::on_historyDisplay_currentIndexChanged(int index)
{
    SettingsManager::setHistoryDisplayPolicy(index);
    emit historyPolicyChanged();
}

void SettingsDialog::on_availableColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        SettingsManager::setAvailableDeviceColor(color.name());
        ui->availableColor->setStyleSheet("QPushButton {\n"
                                              "border-radius: 4px;\n"
                                              "background-color: " + color.name() + ";\n"
                                          "}");
        _availableDevice->setAvailable(true);
        emit refreshDevicesAvailability();
    }
}

void SettingsDialog::on_unavailableColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        SettingsManager::setUnavailableDeviceColor(color.name());
        ui->unavailableColor->setStyleSheet("QPushButton {\n"
                                              "border-radius: 4px;\n"
                                              "background-color: " + color.name() + ";\n"
                                          "}");
        _unavailableDevice->setAvailable(false);
        emit refreshDevicesAvailability();
    }
}

void SettingsDialog::on_serviceDeviceName_editingFinished()
{
    ui->serviceDeviceName->setText(ui->serviceDeviceName->text().remove(";"));
    SettingsManager::setServiceDeviceName(ui->serviceDeviceName->text());
    emit serviceNameChanged();
}

void SettingsDialog::on_destFolderButton_clicked()
{
//    QFileDialog dialog(this);

//    dialog.setFileMode(QFileDialog::Directory);
//    dialog.setOption(QFileDialog::ShowDirsOnly, true);
//    dialog.setDirectory(QDir::currentPath());

//    dialog.open(this, SLOT(onExistingDirectoryOver(const QString &)));
//    dialog.exec();

    QString dirName = QFileDialog::getExistingDirectory(this, tr("Dossier de destination"), SettingsManager::getDestinationFolder());

    if (!dirName.isEmpty())
    {
        SettingsManager::setDestinationFolder(dirName);
        ui->destFolderArea->setText(dirName);
    }
}

void SettingsDialog::onExistingDirectoryOver(const QString &dirName)
{
    if (!dirName.isEmpty())
    {
        SettingsManager::setDestinationFolder(dirName);
        ui->destFolderArea->setText(dirName);
    }
}

void SettingsDialog::on_reset_clicked()
{
    QString localHostName = QHostInfo::localHostName();

#if defined (Q_OS_MACX)
    localHostName.remove(".local");
#endif

    switch (ui->stackedWidget->currentIndex())
    {
    case GENERAL_INDEX:
        ui->startMinimized->setChecked(false);
        SettingsManager::setStartMinimized(false);
        ui->startAtBoot->setChecked(false);
        SettingsManager::setStartAtBoot(false);
        ui->trayIconEnabled->setChecked(true);
        SettingsManager::setTrayIconEnabled(true);
        ui->logEnabled->setChecked(true);
        SettingsManager::setLogEnabled(true);
        ui->searchUpdateAtLaunch->setChecked(true);
        SettingsManager::setSearchUpdateAtLaunch(true);
        ui->startServiceAtLaunch->setChecked(true);
        SettingsManager::setStartServiceAtLaunch(true);
        ui->autoOpenFiles->setChecked(true);
        SettingsManager::setAutoOpenFiles(true);
        ui->destFolderArea->setText(FileHelper::getDownloadLocation());
        SettingsManager::setDestinationFolder(FileHelper::getDownloadLocation());
        ui->serviceDeviceName->setText(localHostName);
        SettingsManager::setServiceDeviceName(localHostName);
        ui->historyDisplay->setCurrentIndex(ON_SERVICE_ENABLED);
        SettingsManager::setHistoryDisplayPolicy(ON_SERVICE_ENABLED);
        break;

    case WIDGET_INDEX:
        ui->errorMessage->setVisible(false);
        ui->widgetEnabled->setChecked(true);
        SettingsManager::setWidgetEnabled(true);
        ui->widgetForeground->setChecked(true);
        SettingsManager::setWidgetForeground(true);
        break;

    case VIEW_INDEX:
        SettingsManager::setUnavailableDeviceColor("#ef0800");
        ui->unavailableColor->setStyleSheet("QPushButton {\n"
                                              "border-radius: 4px;\n"
                                              "background-color: #ef0800;\n"
                                          "}");
        _unavailableDevice->setAvailable(false);
        SettingsManager::setAvailableDeviceColor("#008000");
        ui->availableColor->setStyleSheet("QPushButton {\n"
                                              "border-radius: 4px;\n"
                                              "background-color: #008000;\n"
                                          "}");
        _availableDevice->setAvailable(true);
        emit refreshDevicesAvailability();
        break;
    }
}

void SettingsDialog::on_widgetForeground_toggled(bool checked)
{
    SettingsManager::setWidgetForeground(checked);
    emit updateWidgetFlags();
}

void SettingsDialog::on_searchUpdateAtLaunch_toggled(bool checked)
{
    SettingsManager::setSearchUpdateAtLaunch(checked);
}

void SettingsDialog::on_startAtBoot_toggled(bool checked)
{
    SettingsManager::setStartAtBoot(checked);
}

void SettingsDialog::on_openLogButton_clicked()
{
    LogManager::openLogFile();
}

void SettingsDialog::on_startMinimized_toggled(bool checked)
{
    SettingsManager::setStartMinimized(checked);
}
