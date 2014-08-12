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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QSettings>

enum HistoryDisplayType
{
    ON_SERVICE_ENABLED,
    ALWAYS,
    NEVER
};

/**
  * @class SettingsManager
  *
  * Static class made to handle settings
  */
class SettingsManager
{
public:
    /**
     * Tells if the program should start at the system boot
     *
     * For example, on Windows, it will check the registry
     *
     * @return True if it should start, false otherwise
     */
    static bool shouldStartAtBoot();
    /**
      * Getter : MaxDevices
      */
    static int getMaxDevices();
    /**
      * Getter : HistoryDisplayPolicy
      */
    static int getHistoryDisplayPolicy();
    /**
      * Getter : HistoryVersion
      */
    static int getHistoryVersion();
    /**
      * Getter : MaxFileSize
      */
    static int getMaxFileSize();
    /**
      * Getter : TrayIconEnabled
      */
    static bool isTrayEnabled();
    /**
     * Getter : IgnoredVersion
     */
    static bool isVersionIgnored(const QString &version);
    /**
     * Getter : WidgetForeground
     */
    static bool isWidgetInForeground();
    /**
      * Getter : WidgetEnabled
      */
    static bool isWidgetEnabled();
    /**
      * Getter : LogEnabled
      */
    static bool isLogEnabled();
    /**
     * Getter : SearchUpdateAtLaunch
     */
    static bool isSearchUpdateAtLaunch();
    /**
     * Getter : AutoOpenFiles
     */
    static bool isAutoOpenFilesEnabled();
    /**
     * Getter : WidgetPosition
     */
    static QPoint getWidgetPosition(const QString &uid);
    /**
     * Getter : StartServiceAtLaunch
     */
    static bool isServiceStartedAtlaunch();
    /**
     * Getter : StartMinimized
     */
    static bool isStartMinimizedAtlaunch();
    /**
      * Getter : AvailableDeviceColor
      */
    static QString availableDeviceColor();
    /**
     * Getter : DestinationFolder
     */
    static QString getDestinationFolder();
    /**
      * Getter : ServiceDeviceName
      */
    static QString getServiceDeviceName();
    /**
     * Getter : DeviceUID
     */
    static QString getDeviceUID();
    /**
      * Getter : UnavailableDeviceColor
      */
    static QString unavailableDeviceColor();
    /**
      * Load the settings for the .ini file
      */
    static void loadSettings();
    /**
      * Check the existance of the settings file and load the settings
      */
    static void loadSettingsFile();
    /**
      * Rewrite all settings on the .ini file
      */
    static void rewriteSettings();
    /**
      * Write a new setting in the .ini file
      */
    static void writeSetting(const QString &key, const QVariant &variant);
    /**
      * Setter : MaxDevices
      */
    static void setMaxDevices(int maxDevices);
    /**
      * Setter : MaxSizeFile
      */
    static void setMaxSizeFile(int maxFileSize);
    /**
      * Setter : TrayIconEnableda
      */
    static void setTrayIconEnabled(bool enabled);
    /**
     * Setter : WidgetForeground
     */
    static void setWidgetForeground(bool enabled);
    /**
      * Setter : WidgetEnabled
      */
    static void setWidgetEnabled(bool enabled);
    /**
     * Setter : SearchUpdateAtLaunch
     */
    static void setSearchUpdateAtLaunch(bool enabled);
    /**
      * Setter : AutoOpenFiles
      */
    static void setAutoOpenFiles(bool enabled);
    /**
     * Setter : StartMinimized
     */
    static void setStartMinimized(bool enabled);
    /**
      * Setter : ServiceDeviceName
      */
    static void setServiceDeviceName(const QString &serviceDeviceName);
    /**
      * Setter : HistoryDisplayPolicy
      */
    static void setHistoryDisplayPolicy(int policy);
    /**
      * Setter : AvailableDeviceColor
      */
    static void setAvailableDeviceColor(const QString &color);
    /**
     * Setter : DestinationFolder
     */
    static void setDestinationFolder(const QString &folderName);
    /**
      * Setter : UnavailableDeviceColor
      */
    static void setUnavailableDeviceColor(const QString &color);
    /**
     * Setter : IgnoredVersion
     */
    static void setIgnoredVersion(const QString &version);
    /**
      * Setter : LogEnabled
      */
    static void setLogEnabled(bool enabled);
    /**
      * Setter : HistoryVersion
      */
    static void setHistoryVersion(int version);
    /**
     * Set the application at startup
     *
     * Implementation depends on the system
     */
    static void setStartAtBoot(bool enabled);
    /**
     * Setter : WidgetPosition
     */
    static void setWidgetPosition(const QString &uid, const QPoint &position);
    /**
     * Setter : StartServiceAtLaunch
     */
    static void setStartServiceAtLaunch(bool enabled);
    /**
     * Type correspond to the OS
     * @return The curent operating system
     */
    static QString getType();
    /**
     * Tells if the widget designed by uid should be displayed
     *
     * @param uid Uid of the widget
     * @return True if the widget should be displayed, false otherwise
     */
    static bool shouldDisplayWidget(const QString &uid);
    /**
     * @brief Write in the settings file if a widget (degines by its guid) should be displayed
     * @param uid Device guid
     * @param display Widget visibility
     */
    static void setWidgetVisibility(const QString &uid, bool display);

private:
    /// The maximum number of devices the application will show
    static int MaxDevices;
    /**
      * Is the tray enabled
      * If the tray is desabled, the widget setting will be ignored
      */
    static bool TrayIconEnabled;
    /// Defines if the Widget should stay in foreground mode
    static bool WidgetForeground;
    /// Is the widget enabled
    static bool WidgetEnabled;
    /// Color for available devices
    static QString AvailableDeviceColor;
    /// Service device name
    static QString ServiceDeviceName;
    /// Destination folder
    static QString DestinationFolder;
    /// Color for unavailable devices
    static QString UnavailableDeviceColor;
    /// Settings file name
    static const QString FileName;
    /// Name of the app
    static const QString AppName;
    /// Log enabled
    static bool LogEnabled;
    /// The maximum size of sending file in Mo
    static int MaxFileSize;
    /// Start the Client service at program launch
    static bool StartServiceAtLaunch;
    /// Search software update on launch
    static bool SearchUpdateAtLaunch;
    /// Auto open files on received
    static bool AutoOpenFiles;
    /// History display policy
    static int HistoryDisplayPolicy;
    /// Unique ID
    static QString DeviceUID;
    /// Version ignored
    static QString IgnoredVersion;
    /// Start minimized
    static bool StartMinimized;
    /// HistoryVersion
    static int HistoryVersion;
};

#endif // SETTINGSMANAGER_H
