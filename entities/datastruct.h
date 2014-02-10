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

#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QFile>
#include <QString>
#include <QUrl>

/**
  * @enum DataType
  *
  * Describe the type of data to send
  */
enum DataType
{
    TYPE_FILE_OPEN,
    TYPE_TEXT,
    TYPE_ACK,
    TYPE_DOWNLOAD_PROGRESS,
    TYPE_FILE_TOO_BIG,
    TYPE_FILE_SAVE,
    TYPE_URL_OPEN,
    TYPE_MESSAGE
};

/**
  * @struct DataStruct
  *
  * Structure of the data
  */
struct DataStruct
{
    /**
      * Constructor
      */
    DataStruct();

    /// Type of data
    DataType _type;
    /// String to send
    QString _string;
    /// List of urls to send
    QList<QUrl> _urls;

    /**
     * Is the specified type a file type
     */
    static bool isFileType(DataType type);
};

#endif // DATASTRUCT_H
