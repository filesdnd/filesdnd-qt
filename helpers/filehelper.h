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

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <QIcon>
#include <QFile>
#include <QDir>

/**
  * @class FileHelper
  *
  * Manage files
  */
class FileHelper
{
public:
    /**
      * Default constructor
      */
    FileHelper();
    /**
      * Get the real path for a file (depending on local or network file)
      *
      * @param url The initial url of the file
      * @return Formatted URL of the file
      */
    static QString getFilePath(QString url);
    /**
      * Get the icon for a file
      *
      * @param filename The file of the name
      * @return The icon corresponding to the file explorer
      */
    static QIcon getFileIcon(const QString &filename);
    /**
      * Get the size as a string for a file
      *
      * @param filename The file of the name
      * @return The file size as a string if the file exists
      */
    static QString getFileSize(const QString &filename);
    /**
      * Open the specified file of url with the proper application
      *
      * @param url Filename or url
      */
    static void openURL(const QString &url);
    /**
      * Delete a file from the disk
      *
      * @param filename The name of the file
      */
    static void deleteFileFromDisk(const QString &filename);
    /**
      * Delete a file from the disk
      *
      * @param file File to delete
      */
    static void deleteFileFromDisk(QFile &file);
    /**
     * Delete a folder from the disk
     *
     * @param dir Directory to delete
     */
    static void deleteFileFromDisk(QDir &dir);
    /**
      * Define if a file exists or not
      *
      * @return True if the file exists, false otherwise
      */
    static bool exists(const QString &filename);
    /**
      * Try to retrieve the default storage location
      *
      * @return The default storage location as a string
      */
    static QString getDefaultStorageLocation();
    /**
      * Try to retrieve the default storage location for data (settings)
      *
      * @return The default storage location as a string
      */
    static QString getFileStorageLocation();
    /**
      * Try to retrieve the default download location
      *
      * @return The default download location as a string
      */
    static QString getDownloadLocation();
    /**
     * Save text to clipboard
     *
     * @param txt text to save
     */
    static void saveToClipboard(const QString &txt);
    /**
     * Return the bytes as a formatted string
     *
     * @param bytes Bytes to convert
     * @return Formatted string
     */
    static QString getSizeAsString(qint64 bytes);
    /**
     * @brief Retrieve the sendTo folder of the windows registry
     *
     * @return The path of the sent to folder or an empty string
     */
    static QString getSendToFolder();
};

#endif // FILEHELPER_H
