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

#include "helpers/logmanager.h"
#include "settingswidget.h"
#include "helpers/settingsmanager.h"
#include "ui_settingswidget.h"

#include <QTextStream>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    _availableDevice = new DeviceView(tr("Disponible"), "", TYPE_ANDROID, true, NOSTATE, 0, ui->overView);
    _unavailableDevice = new DeviceView(tr("Non Disponible"), "", TYPE_ANDROID, false, NOSTATE, 0, ui->overView);

    // Stylesheet
    loadStyle(SettingsManager::availableDeviceColor(), SettingsManager::unavailableDeviceColor());

    ui->availableDevice->addWidget(_availableDevice);
    ui->unavailableDevice->addWidget(_unavailableDevice);

#if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
    ui->startAtBoot->setVisible(true);
#else
    ui->startAtBoot->setVisible(false);
#endif
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
    delete _availableDevice;
    delete _unavailableDevice;
}

void SettingsWidget::on_trayIconEnabled_toggled(bool checked)
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

void SettingsWidget::on_widgetEnabled_toggled(bool checked)
{
    ui->widgetForeground->setChecked(checked);
    ui->widgetForeground->setEnabled(checked);

    SettingsManager::setWidgetEnabled(checked);
    emit widgetStateChanged();
}

void SettingsWidget::on_logEnabled_toggled(bool checked)
{
    SettingsManager::setLogEnabled(checked);
}

void SettingsWidget::on_startServiceAtLaunch_toggled(bool checked)
{
    SettingsManager::setStartServiceAtLaunch(checked);
}

void SettingsWidget::refreshSettingsFromFile()
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
    ui->serviceDeviceName->setText(SettingsManager::getServiceDeviceName());
    ui->destFolderArea->setText(SettingsManager::getDestinationFolder());

    /// Color settings
    loadStyle(SettingsManager::availableDeviceColor(), SettingsManager::unavailableDeviceColor());

    _unavailableDevice->setAvailable(false);
    _availableDevice->setAvailable(true);

    /// Widget settings
    if (!ui->widgetEnabled->isChecked())
    {
        ui->widgetForeground->setChecked(false);
        ui->widgetForeground->setEnabled(false);
    }
}

void SettingsWidget::showEvent(QShowEvent* event)
{
    refreshSettingsFromFile();

    QWidget::showEvent(event);
}

void SettingsWidget::on_autoOpenFiles_toggled(bool checked)
{
    SettingsManager::setAutoOpenFiles(checked);
}

void SettingsWidget::on_availableColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        SettingsManager::setAvailableDeviceColor(color.name());
        loadStyle(SettingsManager::availableDeviceColor(), SettingsManager::unavailableDeviceColor());

        _availableDevice->setAvailable(true);
        emit refreshDevicesAvailability();
    }
}

void SettingsWidget::on_unavailableColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        SettingsManager::setUnavailableDeviceColor(color.name());
        loadStyle(SettingsManager::availableDeviceColor(), SettingsManager::unavailableDeviceColor());

        _unavailableDevice->setAvailable(false);
        emit refreshDevicesAvailability();
    }
}

void SettingsWidget::on_serviceDeviceName_editingFinished()
{
    ui->serviceDeviceName->setText(ui->serviceDeviceName->text().remove(";"));
    SettingsManager::setServiceDeviceName(ui->serviceDeviceName->text());
    emit serviceNameChanged();
}

void SettingsWidget::on_destFolderButton_clicked()
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

void SettingsWidget::onExistingDirectoryOver(const QString &dirName)
{
    if (!dirName.isEmpty())
    {
        SettingsManager::setDestinationFolder(dirName);
        ui->destFolderArea->setText(dirName);
    }
}

void SettingsWidget::on_widgetForeground_toggled(bool checked)
{
    SettingsManager::setWidgetForeground(checked);
    emit updateWidgetFlags();
}

void SettingsWidget::on_searchUpdateAtLaunch_toggled(bool checked)
{
    SettingsManager::setSearchUpdateAtLaunch(checked);
}

void SettingsWidget::loadStyle(const QString &availableColor, const QString &unavailableColor)
{
    QString css = FileHelper::loadFileContent(SETTINGS_CSS);

    css.replace("#availableColor#", availableColor);
    css.replace("#unavailableColor#", unavailableColor);

    setStyleSheet(css);
}

void SettingsWidget::on_startAtBoot_toggled(bool checked)
{
    SettingsManager::setStartAtBoot(checked);
}

void SettingsWidget::on_openLogButton_clicked()
{
    LogManager::openLogFile();
}

void SettingsWidget::on_startMinimized_toggled(bool checked)
{
    SettingsManager::setStartMinimized(checked);
}
