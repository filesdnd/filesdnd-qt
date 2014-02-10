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

#ifndef TXTRECORD_H
#define TXTRECORD_H

#include <QString>
#include <QStringList>
#include <QMap>

/**
 * @class TxtRecord
 *
 * Contains the bonjour bonjour data
 */
class TxtRecord
{
public:
    /**
     * Constructor
     */
    TxtRecord();
    /**
     * Append a <key, value>
     *
     * @param kv Formated "key value"
     */
    void append(const QString &kv);
    /**
     * Append a <key, value>
     *
     * @param key Key to insert
     * @param value Value corresponding to the key
     */
    void append(const QString &key, const QString &value);
    /**
     * Getter : _txtRecord
     */
    const char * getData();
    /**
     * Getter : _size
     */
    int length();
    /**
     * Parse the TxtRecord to get a map of <key, value>
     */
    static QMap<QString, QString> parse(quint16 txtLen, const char *txtRecord);
private:
    /// TxtRecord data
    QByteArray _txtRecord;
    /// Size of the record
    int _size;
};

#endif // TXTRECORD_H
