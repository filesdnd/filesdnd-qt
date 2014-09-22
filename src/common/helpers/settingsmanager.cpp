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

#include "settingsmanager.h"
#include "filehelper.h"
#include "appconfig.h"

#include <QFile>
#include <QHostInfo>
#include <QDebug>
#include <QUuid>
#include <QDir>
#include <QApplication>
#include <QRect>
#include <QPoint>
#include <QDesktopWidget>
#include <QDesktopServices>

#define MAX_DEVICES "MaxDevices"
#define TRAY_ICON_ENABLED "TrayIconEnabled"
#define WIDGET_ENABLED "WidgetEnabled"
#define AVAILABLE_DEVICE_COLOR "AvailableDeviceColor"
#define UNAVAILABLE_DEVICE_COLOR "UnavailableDeviceColor"
#define LOG_ENABLED "LogEnabled"
#define START_SERVICE_AT_LAUNCH "StartServiceAtLaunch"
#define AUTO_OPEN_FILES "AutoOpenFiles"
#define SERVICE_DEVICE_NAME "ServiceDeviceName"
#define DESTINATION_FOLDER "DestinationFolder"
#define DEVICE_UID "UID"
#define WIDGET_FOREGROUND "WidgetForeground"
#define IGNORED_VERSION "IgnoredVersion"
#define SEARCH_UPDATE_ON_LAUNCH "SearchUpdateAtLaunch"
#define START_MINIMIZED "StartMinimized"
#define HISTORY_VERSION "HistoryVersion"
#define FIRST_LAUNCH "FirstLaunch"

#define WIDGET_DISPLAY "_DISPLAY"
#define WIDGET_POS "_POS"

int SettingsManager::HistoryVersion = -1;
bool SettingsManager::StartMinimized = false;
bool SettingsManager::LogEnabled = true;
bool SettingsManager::WidgetForeground = true;
int SettingsManager::MaxDevices = 10;
bool SettingsManager::TrayIconEnabled = true;
bool SettingsManager::StartServiceAtLaunch = true;
bool SettingsManager::AutoOpenFiles = true;
bool SettingsManager::WidgetEnabled = true;
bool SettingsManager::FirstLaunch = true;
bool SettingsManager::SearchUpdateAtLaunch = true;
QString SettingsManager::AvailableDeviceColor = "#008000";
QString SettingsManager::DeviceUID = QString::number(qHash(QUuid::createUuid().toString()));
#if defined (Q_OS_MACX)
QString SettingsManager::ServiceDeviceName = QHostInfo::localHostName().remove(".local");
#else
QString SettingsManager::ServiceDeviceName = QHostInfo::localHostName();
#endif
QString SettingsManager::DestinationFolder = FileHelper::getDownloadLocation();
QString SettingsManager::UnavailableDeviceColor = "#ef0800";
QString SettingsManager::IgnoredVersion = "Any";
const QString SettingsManager::FileName = FileHelper::getFileStorageLocation() + "/" + SETTINGS_FILE;
const QString SettingsManager::AppName = "Files Drag & Drop";

int SettingsManager::getMaxDevices()
{
    return MaxDevices;
}

bool SettingsManager::shouldStartAtBoot()
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
    QSettings settings(QDir::toNativeSeparators("HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run"),
                       QSettings::NativeFormat);

    return !(settings.value(AppName).toString().isEmpty());
#else
    return false;
#endif
}

QPoint SettingsManager::getWidgetPosition(const QString &uid)
{
    QSettings settings(FileName, QSettings::IniFormat);

    return settings.value(uid + WIDGET_POS, QPoint(0, 0)).toPoint();
}

bool SettingsManager::isVersionIgnored(const QString &version)
{
    return (IgnoredVersion == version);
}

bool SettingsManager::isSearchUpdateAtLaunch()
{
    return SearchUpdateAtLaunch;
}

bool SettingsManager::isTrayEnabled()
{
    return TrayIconEnabled;
}

bool SettingsManager::isWidgetInForeground()
{
    return WidgetForeground;
}

QString SettingsManager::getDestinationFolder()
{
    return DestinationFolder;
}

QString SettingsManager::getDeviceUID()
{
    return DeviceUID;
}

QString SettingsManager::getServiceDeviceName()
{
    return ServiceDeviceName;
}

int SettingsManager::getHistoryVersion()
{
    return HistoryVersion;
}

bool SettingsManager::isAutoOpenFilesEnabled()
{
    return AutoOpenFiles;
}

bool SettingsManager::isServiceStartedAtlaunch()
{
    return StartServiceAtLaunch;
}

bool SettingsManager::isStartMinimizedAtlaunch()
{
    return StartMinimized;
}

bool SettingsManager::isLogEnabled()
{
    return LogEnabled;
}

bool SettingsManager::isWidgetEnabled()
{
    return WidgetEnabled;
}

QString SettingsManager::availableDeviceColor()
{
    return AvailableDeviceColor;
}

QString SettingsManager::unavailableDeviceColor()
{
    return UnavailableDeviceColor;
}

bool SettingsManager::shouldDisplayWidget(const QString &uid)
{
    QSettings settings(FileName, QSettings::IniFormat);

    return settings.value(uid + WIDGET_DISPLAY, true).toBool();
}

bool SettingsManager::isFirstLaunch()
{
    return FirstLaunch;
}

void SettingsManager::loadSettingsFile()
{
    QFile file(FileName);

    if (!file.exists())
    {
        rewriteSettings();
    }
    else
    {
        loadSettings();
    }
}

void SettingsManager::rewriteSettings()
{
    QSettings settings(FileName, QSettings::IniFormat);

    settings.setValue(HISTORY_VERSION, HistoryVersion);
    settings.setValue(FIRST_LAUNCH, FirstLaunch);
    settings.setValue(START_MINIMIZED, StartMinimized);
    settings.setValue(MAX_DEVICES, MaxDevices);
    settings.setValue(TRAY_ICON_ENABLED, TrayIconEnabled);
    settings.setValue(WIDGET_ENABLED, WidgetEnabled);
    settings.setValue(AVAILABLE_DEVICE_COLOR, AvailableDeviceColor);
    settings.setValue(UNAVAILABLE_DEVICE_COLOR, UnavailableDeviceColor);
    settings.setValue(LOG_ENABLED, LogEnabled);
    settings.setValue(START_SERVICE_AT_LAUNCH, StartServiceAtLaunch);
    settings.setValue(AUTO_OPEN_FILES, AutoOpenFiles);
    settings.setValue(SERVICE_DEVICE_NAME, ServiceDeviceName);
    settings.setValue(DESTINATION_FOLDER, DestinationFolder);
    settings.setValue(DEVICE_UID, DeviceUID);
    settings.setValue(WIDGET_FOREGROUND, WidgetForeground);
    settings.setValue(IGNORED_VERSION, IgnoredVersion);
    settings.setValue(SEARCH_UPDATE_ON_LAUNCH, SearchUpdateAtLaunch);
}

void SettingsManager::loadSettings()
{
    QSettings settings(FileName, QSettings::IniFormat);

    HistoryVersion  = settings.value(HISTORY_VERSION, HistoryVersion).toInt();
    StartMinimized = settings.value(START_MINIMIZED, StartMinimized).toBool();
    MaxDevices = settings.value(MAX_DEVICES, MaxDevices).toInt();
    TrayIconEnabled = settings.value(TRAY_ICON_ENABLED, TrayIconEnabled).toBool();
    StartServiceAtLaunch = settings.value(START_SERVICE_AT_LAUNCH, StartServiceAtLaunch).toBool();
    WidgetForeground = settings.value(WIDGET_FOREGROUND, WidgetForeground).toBool();
    LogEnabled = settings.value(LOG_ENABLED, LogEnabled).toBool();
    AutoOpenFiles = settings.value(AUTO_OPEN_FILES, AutoOpenFiles).toBool();
    WidgetEnabled = settings.value(WIDGET_ENABLED, WidgetEnabled).toBool();
    AvailableDeviceColor = settings.value(AVAILABLE_DEVICE_COLOR, AvailableDeviceColor).toString();
    UnavailableDeviceColor = settings.value(UNAVAILABLE_DEVICE_COLOR, UnavailableDeviceColor).toString();
    ServiceDeviceName = settings.value(SERVICE_DEVICE_NAME, ServiceDeviceName).toString();
    DestinationFolder = settings.value(DESTINATION_FOLDER, DestinationFolder).toString();
    DeviceUID = settings.value(DEVICE_UID, DeviceUID).toString();
    IgnoredVersion = settings.value(IGNORED_VERSION, IgnoredVersion).toString();
    SearchUpdateAtLaunch = settings.value(SEARCH_UPDATE_ON_LAUNCH, SearchUpdateAtLaunch).toBool();
    FirstLaunch = settings.value(FIRST_LAUNCH, FirstLaunch).toBool();
}

void SettingsManager::setHistoryVersion(int version)
{
    HistoryVersion = version;
    writeSetting(HISTORY_VERSION, HistoryVersion);
}

void SettingsManager::setWidgetVisibility(const QString &uid, bool display)
{
    writeSetting(uid + WIDGET_DISPLAY, display);
}

void SettingsManager::setStartMinimized(bool enabled)
{
    StartMinimized = enabled;
    writeSetting(START_MINIMIZED, StartMinimized);
}

void SettingsManager::setStartAtBoot(bool enabled)
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
    QSettings settings(QDir::toNativeSeparators("HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run"),
                       QSettings::NativeFormat);

    if (enabled)
        settings.setValue(AppName, "\"" + QDir::toNativeSeparators(QCoreApplication::applicationFilePath()) + "\"");
    else
        settings.remove(AppName);
#endif
}

void SettingsManager::setIgnoredVersion(const QString &version)
{
    IgnoredVersion = version;
    writeSetting(IGNORED_VERSION, IgnoredVersion);
}

void SettingsManager::setWidgetPosition(const QString &uid, const QPoint &position)
{
    writeSetting(uid + WIDGET_POS, position);
}

void SettingsManager::setSearchUpdateAtLaunch(bool enabled)
{
    SearchUpdateAtLaunch = enabled;
    writeSetting(SEARCH_UPDATE_ON_LAUNCH, SearchUpdateAtLaunch);
}

void SettingsManager::setAutoOpenFiles(bool enabled)
{
    AutoOpenFiles = enabled;
    writeSetting(AUTO_OPEN_FILES, AutoOpenFiles);
}

void SettingsManager::setWidgetForeground(bool enabled)
{
    WidgetForeground = enabled;
    writeSetting(WIDGET_FOREGROUND, WidgetForeground);
}

void SettingsManager::setStartServiceAtLaunch(bool enabled)
{
    StartServiceAtLaunch = enabled;
    writeSetting(START_SERVICE_AT_LAUNCH, StartServiceAtLaunch);
}

void SettingsManager::setMaxDevices(int maxDevices)
{
    MaxDevices = maxDevices;
    writeSetting(MAX_DEVICES, MaxDevices);
}

void SettingsManager::setFirstLaunch(bool firstLaunch)
{
    FirstLaunch = firstLaunch;
    writeSetting(FIRST_LAUNCH, FirstLaunch);
}

void SettingsManager::setLogEnabled(bool enabled)
{
    LogEnabled = enabled;
    writeSetting(LOG_ENABLED, LogEnabled);
}

void SettingsManager::setTrayIconEnabled(bool enabled)
{
    TrayIconEnabled = enabled;
    writeSetting(TRAY_ICON_ENABLED, TrayIconEnabled);
}

void SettingsManager::setWidgetEnabled(bool enabled)
{
    WidgetEnabled = enabled;
    writeSetting(WIDGET_ENABLED, WidgetEnabled);
}

void SettingsManager::setAvailableDeviceColor(const QString &color)
{
    AvailableDeviceColor = color;
    writeSetting(AVAILABLE_DEVICE_COLOR, AvailableDeviceColor);
}

void SettingsManager::setDestinationFolder(const QString &folderName)
{
    DestinationFolder = folderName;
    writeSetting(DESTINATION_FOLDER, DestinationFolder);
}

void SettingsManager::setServiceDeviceName(const QString &serviceDeviceName)
{
    ServiceDeviceName = serviceDeviceName;
    writeSetting(SERVICE_DEVICE_NAME, ServiceDeviceName);
}

void SettingsManager::setUnavailableDeviceColor(const QString &color)
{
    UnavailableDeviceColor = color;
    writeSetting(UNAVAILABLE_DEVICE_COLOR, UnavailableDeviceColor);
}

void SettingsManager::writeSetting(const QString &key, const QVariant &variant)
{
    QSettings settings(FileName, QSettings::IniFormat);

    settings.setValue(key, variant);
}

QString SettingsManager::getType()
{
    QString type = TYPE_STRING_WINDOWS;
    #if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
        type = TYPE_STRING_WINDOWS;
    #elif defined(Q_OS_LINUX) || defined(__linux__)
        type = TYPE_STRING_LINUX;
    #elif defined(Q_OS_MACX)
        type = TYPE_STRING_MAC;
    #endif

    return type;
}
