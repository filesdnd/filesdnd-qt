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

#include "historyelement.h"

#include <QDataStream>
#include <QDebug>

HistoryElement::HistoryElement(QObject *parent) :
    QObject(parent)
{
}

HistoryElement::HistoryElement(const QDateTime &date, const QString &text, HistoryElementType type, QObject *parent) :
    QObject(parent)
{
    setDate(date);
    setText(text);
    setType(type);
}

HistoryElement& HistoryElement::operator=(const HistoryElement &historyElement)
{
    if (&historyElement != this)
    {
        setDate(historyElement._date);
        setText(historyElement._txt);
        setType(historyElement._type);
    }

    return *this;
}

bool HistoryElement::isFile()
{
    return (_type == HISTORY_FILE_FOLDER_TYPE);
}

HistoryElementType HistoryElement::getType() const
{
    return _type;
}

bool HistoryElement::operator==(const HistoryElement &historyElement)
{
    return (historyElement._date == _date &&
            historyElement._txt == _txt &&
            historyElement._type == _type);
}

HistoryElement::HistoryElement(const HistoryElement &element)
{
    _date = element._date;
    _txt = element._txt;
    _type = element._type;
}

void HistoryElement::setDate(const QDateTime &date)
{
    _date = date;
}

void HistoryElement::setType(HistoryElementType type)
{
    _type = type;
}

void HistoryElement::setText(const QString &text)
{
    _txt = text;
}

QString HistoryElement::getDateTime(const QString &format) const
{
    return _date.toString(format);
}

QString HistoryElement::getText() const
{
    return _txt;
}

QDataStream &operator<<(QDataStream &out, const HistoryElement &element)
{
    out << element._date << element._txt << (qint16)element._type;

    return out;
}

QDataStream &operator>>(QDataStream &in, HistoryElement &element)
{
    qint16 type;

    in >> element._date >> element._txt >> type;

    element._type = HistoryElementType(type);

    return in;
}
