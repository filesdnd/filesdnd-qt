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

#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QEvent>
#include <QString>

#define CURRENT_VERSION "2.0.0"
#define PROTOCOL_VERSION "2"
#define APP_HISTORY_VERSION 2
#define WEB_SITE "http://www.filesdnd.fr"
#define EN_WEB_SITE "http://www.filesdnd.com"
#define SEND_TO_NAME "Files Drag and Drop.lnk"

#define CONFIG_APP_ICON ":/ic_launcher"
#define CONFIG_TRAY_ICON ":/ic_launcher"
#define CONFIG_GREY_TRAY_ICON ":/ic_launcher_grey"
#define CONFIG_BLACK_WHITE_TRAY_ICON ":/ic_launcher_black_white"
#define CONFIG_WHITE_BLACK_TRAY_ICON ":/ic_launcher_white_black"
#define CONFIG_GREY_WHITE_TRAY_ICON ":/ic_launcher_grey_white"
#define HIDE_ALL_WIDGETS_ICON ":/images/icons/crosses-small.png"
#define HIDE_WIDGET_ICON ":/images/icons/cross_red.png"
#define RESTORE_MAIN_VIEW_ICON ":/images/icons/arrow-out.png"
#define FOLDER_ICON ":/images/icons/folder-horizontal-open.png"
#define NODEVICE_LOGO ":/images/other/nodevice_logo.png"
#define BONJOUR_LOGO ":/images/other/bonjour_logo.png"
#define HISTORY_CLEAR_HISTORY_ICON ":/images/icons/clear_history.png"
#define HISTORY_DELETE_FILE_ICON ":/images/icons/cross_red.png"
#define HISTORY_REMOVE_ICON ":/images/icons/remove_history.png"
#define HISTORY_LAUNCH_ICON ":/images/icons/launch_file.png"
#define TEXT_ICON ":/images/icons/script-text.png"
#define LINK_ICON ":/images/icons/link.png"
#define CLIPBOARD_ICON ":/images/icons/clipboard-paste.png"
#define ANIMATED_REFRESH_ICON ":/images/flat/refresh-circle-flat-animated.gif"
#define ANIMATED_WAITING_LOGO ":/images/logo/waiting-logo.gif"
#define ANIMATED_BONJOUR_LOGO ":/images/other/bonjour-logo.gif"

#define CONFIG_PANEL_CSS ":/css/common/css/configpanel.css"
#define WINDOWS_APP_CSS ":/css/windows-ui/application.css"
#define HISTORY_LIST_CSS ":/css/common/css/history.css"
#define DEVICE_VIEW_CSS ":/css/common/css/deviceview.css"

#define HISTORY_ICON_SIZE 16
#define FILE_ICON ":/fileIconFlat"
#define XLSX_ICON ":/xlsxIconFlat"
#define DOC_ICON ":/docIconFlat"
#define PDF_ICON ":/pdfIconFlat"
#define ZIP_ICON ":/zipIconFlat"
#define MUSIC_ICON ":/musicIconFlat"
#define IMG_ICON ":/imageIconFlat"
#define MOVIE_ICON ":/movieIconFlat"

#define OVERLAY_TIMEOUT (1000 * 10)
#define BONJOUR_TIMEOUT (1000 * 25)
#define READ_FILE_BUFFER 5000000
#define MAX_HISTORY_SIZE 20
#define RESTART_REGISTER_TIMER (60000 * 10)
#define FOCUSED_NETWORK_REFRESH 60 * 2 // 2 minutes
#define NOTIFY_FACTOR (256 * 1024)
#define HISTORY_ANIMATION_TIMER 750
#define OPACITY_ANIMATION_TIMER 1500
#define WIDGET_ANIMATION_TIMER 600
#define WIDGET_OFFSET 15
#define WIDGET_OPACITY 0.85
#define PREFIX "filesdnd "

#define KEY_UID "uid"
#define KEY_TYPE "type"
#define KEY_VERSION "version"

#define TYPE_STRING_ANDROID "A"
#define TYPE_STRING_MAC "M"
#define TYPE_STRING_LINUX "L"
#define TYPE_STRING_WINDOWS "W"

#define SERVICE_TYPE "_fdnd._tcp."
#define BONJOUR_DOMAIN "local."

#define LOG_FILE "filesdnd.log"
#define SETTINGS_FILE "settings.ini"
#define HISTORY_FILE "history"

#define DETECTED_BY_BONJOUR 1
#define DETECTED_BY_UDP 2

#define DEFAULT_DOWNLOAD_DIR "/Files Drag & Drop"
#define DEFAULT_STORAGE_DIR "/Files Drag & Drop"
#define ZIP_EXTENSION ".fdndzip"

#define MULTICAST_ADDR "227.113.113.0"
#define UDP_DISCOVERY_MULTICAST_PORT 60111
#define UDP_DISCOVERY_BROADCAST_PORT 60112
#define GET_RECORD_INTERVAL 1500
#define PING_INTERVAL 1500
#define UDP_DISCOVERY_INTERVAL 1000 * 60 * 2  // 2 minutes

#define ACTION_GET_RECORD "getrecords"
#define ACTION_RECORD "record"
#define ACTION_ANNOUNCE "announce"
#define ACTION_LEAVE "leave"
#define ACTION_PING "ping "
#define ACTION_PONG "pong "

enum EventType
{
    EVENT_TYPE_CLIPBOARD = QEvent::User + 1,
    EVENT_TYPE_PING = QEvent::User + 2,
    EVENT_TYPE_CONNECT = QEvent::User + 3,
    EVENT_TYPE_PONG = QEvent::User + 4,
    EVENT_TYPE_CANCEL_TRANSFERT = QEvent::User + 5
};

#endif // APPCONFIG_H
