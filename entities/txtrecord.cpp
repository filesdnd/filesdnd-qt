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

#include "txtrecord.h"

TxtRecord::TxtRecord()
{
}

void TxtRecord::append(const QString &kv)
{
    QString keyValue(kv);
    keyValue.insert(0, ' ');
    QByteArray final = keyValue.toLatin1();
    char *cdata = final.data();
    cdata[0] = strlen(cdata) - 1;

    _txtRecord.append(final.constData()); // need const data to copy the value
}

void TxtRecord::append(const QString &key, const QString &value)
{
    QString record(key);
    record.append('=').append(value);
    append(record);
}

const char * TxtRecord::getData()
{
    return _txtRecord.constData();
}

int TxtRecord::length()
{
    return _size;
}

QMap<QString, QString> TxtRecord::parse(quint16 txtLen, const char *txtRecord)
{
    QMap<QString, QString> records;
    QString keyvalue;
    QStringList record;
    int size = 0;
    int read = 0;

    if(txtLen > 1)
    {
        while( read < txtLen)
        {
            size = txtRecord[0];
            ++txtRecord; // We skip the size to get the string

            keyvalue = QString::fromLatin1(txtRecord, size);
            record = keyvalue.split('=');
            records.insert(record.at(0), record.at(1));

            txtRecord += size; // we go to the next string (length byte + data)
            read += size + 1; // To length byte + data
        }
    }

    return records;
}
