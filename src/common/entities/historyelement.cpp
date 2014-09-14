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

HistoryElement::HistoryElement(const QDateTime &date, const QString &text, const QString &name, qint64 size, HistoryElementType type, QObject *parent) :
    QObject(parent)
{
    setDate(date);
    setText(text);
    setType(type);
    setName(name);
    setSize(size);
}

HistoryElement& HistoryElement::operator=(const HistoryElement &historyElement)
{
    if (&historyElement != this)
    {
        setDate(historyElement._date);
        setText(historyElement._txt);
        setType(historyElement._type);
        setName(historyElement._name);
        setSize(historyElement._size);
    }

    return *this;
}

bool HistoryElement::isFile()
{
    return (_type == HISTORY_FILE_TYPE);
}

HistoryElementType HistoryElement::getType() const
{
    return _type;
}

bool HistoryElement::operator==(const HistoryElement &historyElement)
{
    return (historyElement._date == _date &&
            historyElement._txt == _txt &&
            historyElement._type == _type &&
            historyElement._name == _name &&
            historyElement._size == _size);
}

HistoryElement::HistoryElement(const HistoryElement &element)
{
    _date = element._date;
    _txt = element._txt;
    _type = element._type;
    _name = element._name;
    _size = element._size;
}

void HistoryElement::setDate(const QDateTime &date)
{
    _date = date;
}

void HistoryElement::setSize(qint64 size)
{
    _size = size;
}

void HistoryElement::setType(HistoryElementType type)
{
    _type = type;
}

void HistoryElement::setText(const QString &text)
{
    _txt = text;
}

void HistoryElement::setName(const QString &name)
{
    _name = name;
}

QString HistoryElement::getName()
{
    return _name;
}

qint64 HistoryElement::getSize()
{
    return _size;
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
    out << element._date << element._txt << element._name << (qint64)element._size << (qint16)element._type;

    return out;
}

QDataStream &operator>>(QDataStream &in, HistoryElement &element)
{
    qint16 type;

    in >> element._date >> element._txt >> element._name >> element._size >> type;

    element._type = HistoryElementType(type);

    return in;
}
