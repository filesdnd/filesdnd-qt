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

#include "helpers/filehelper.h"
#include "helpers/settingsmanager.h"
#include "helpers/logmanager.h"
#include "appconfig.h"

#include <QFileIconProvider>
#include <QIcon>
#include <QDir>
#include <QStandardPaths>
#include <QUrl>
#include <QFileInfo>
#include <QDesktopServices>
#include <QApplication>
#include <QDebug>
#include <QClipboard>

FileHelper::FileHelper()
{
}

QString FileHelper::getFilePath(QString url)
{
    QString path;
    QString filePrefix = "file:///";
    QString networkFilePrefix = "file://";

    if(url.startsWith(filePrefix)) {
        //Normal file
        int length = filePrefix.length();
#if defined (Q_OS_MACX) || defined(Q_OS_LINUX) || defined(__linux__)
            length -= 1;
#endif

        path = url.remove(0, length);
    } else if (url.startsWith(networkFilePrefix)){
        path = url.remove(0, networkFilePrefix.remove("/").length());
    }

    return path;
}

QString FileHelper::getFileSize(const QString &filename)
{
    QFileInfo info(SettingsManager::getDestinationFolder() + "/" + filename);
    QString result;

    if (info.exists() && !info.isDir())
    {
        qint64 bytes = info.size();
        result = getSizeAsString(bytes);
    }
    else
        result = "--";

    return result;
}

QString FileHelper::getSizeAsString(qint64 bytes)
{
    QString result;
    qint64 byte = 1;
    qint64 KB = 1024 * byte;
    qint64 MB = 1024 * KB;
    char doubleFormat = 'f';

    if (bytes > MB)
        result = QString::number((float)bytes/(float)MB, doubleFormat, 1) + " Mo";
    else
        if (bytes > KB)
            result = QString::number((float)bytes/(float)KB, doubleFormat, 1) + " Ko";
        else
            result = QString::number(bytes) + " o";

    return result;
}

void FileHelper::saveToClipboard(const QString &txt)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(txt);
}

QIcon FileHelper::getFileIcon(const QString &filename)
{
    QFileInfo info(SettingsManager::getDestinationFolder() + "/" + filename);
    QIcon icon;

    if(info.exists())
    {
        icon = QFileIconProvider().icon(info);
    }
    else
    {
        icon = QIcon(FILE_ICON);
    }

    return icon;
}

void FileHelper::deleteFileFromDisk(const QString &filename)
{
    QString name = SettingsManager::getDestinationFolder() + "/" + filename;
    QFileInfo fileInfo(name);

    if (fileInfo.exists())
    {
        if (fileInfo.isDir())
        {
            QDir dir(name);
            deleteFileFromDisk(dir);
        }
        else
        {
            QFile file(name);
            deleteFileFromDisk(file);
        }
    }
}

void FileHelper::deleteFileFromDisk(QDir &dir)
{
    if (dir.exists())
    {
        dir.removeRecursively();
    }
}

void FileHelper::deleteFileFromDisk(QFile &file)
{
    if (file.exists())
        file.remove();
}

bool FileHelper::exists(const QString &filename)
{
    QFileInfo info(SettingsManager::getDestinationFolder() + "/" + filename);

    return info.exists();
}

void FileHelper::openURL(const QString &url)
{
    if (!QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode)))
        LogManager::appendLine("[FileHelper] ERROR - Can't open " + url);
}

QString FileHelper::getDownloadLocation()
{
    QString defaultLocation = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (defaultLocation.isEmpty())
        defaultLocation = QDir::homePath();

    defaultLocation = QDir::fromNativeSeparators(defaultLocation);
    defaultLocation.append(DEFAULT_DOWNLOAD_DIR);

    return defaultLocation;
}

QString FileHelper::getSendToFolder()
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
    QSettings settings(QDir::toNativeSeparators("HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Explorer/Shell Folders"),
                       QSettings::NativeFormat);
    return settings.value("SendTo").toString();
#endif
    return "";
}

QString FileHelper::getDefaultStorageLocation()
{
    QString defaultLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    if (defaultLocation.isEmpty())
        defaultLocation = QDir::homePath();

    defaultLocation = QDir::fromNativeSeparators(defaultLocation);
    defaultLocation.append(DEFAULT_DOWNLOAD_DIR);

    return defaultLocation;
}

QString FileHelper::loadFileContent(const QString &path)
{
    QFile file(path);
    QString content = "";

    if (file.open(QFile::ReadOnly)) {
        content = file.readAll();
    }

    return content;
}

QString FileHelper::getFileStorageLocation()
{
    QString defaultLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

    if (defaultLocation.isEmpty())
        defaultLocation = QDir::homePath();

    defaultLocation = QDir::fromNativeSeparators(defaultLocation);
    defaultLocation.append(DEFAULT_STORAGE_DIR);

    return defaultLocation;
}

bool FileHelper::isSpreadsheet(const QString &filename)
{
    return (filename.endsWith("ods") || filename.endsWith("xls") || filename.endsWith("xlsx") || filename.endsWith("xlr"));
}

bool FileHelper::isDoc(const QString &filename)
{
    return (filename.endsWith("doc") || filename.endsWith("docx") || filename.endsWith("odt") || filename.endsWith("rtf"));
}

bool FileHelper::isPdf(const QString &filename)
{
    return filename.endsWith("pdf");
}

bool FileHelper::isZip(const QString &filename)
{
    return (filename.endsWith("zip") || filename.endsWith("tar") || filename.endsWith("tar.gz") || filename.endsWith("rar")
             || filename.endsWith("7z") || filename.endsWith("gz"));
}

bool FileHelper::isMusic(const QString &filename)
{
    return (filename.endsWith("aac") || filename.endsWith("flac") || filename.endsWith("mid") || filename.endsWith("mp3")
             || filename.endsWith("m3u") || filename.endsWith("wav") || filename.endsWith("mpa") || filename.endsWith("m4a"));
}

bool FileHelper::isImage(const QString &filename)
{
    return (filename.endsWith("png") || filename.endsWith("jpg") || filename.endsWith("jpeg") || filename.endsWith("gif")
            || filename.endsWith("bmp") || filename.endsWith("psd") || filename.endsWith("svg") || filename.endsWith("ico"));
}

bool FileHelper::isMovie(const QString &filename)
{
    return (filename.endsWith("3gp") || filename.endsWith("avi") || filename.endsWith("mp4") || filename.endsWith("mpeg")
            || filename.endsWith("flv") || filename.endsWith("m4v") || filename.endsWith("mov") || filename.endsWith("mpg")
            || filename.endsWith("rts") || filename.endsWith("wmv") || filename.endsWith("xvid") || filename.endsWith("swf"));
}
