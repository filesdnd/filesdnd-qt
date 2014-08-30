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

#ifndef HISTORYELEMENT_H
#define HISTORYELEMENT_H

#include <QObject>
#include <QDateTime>

enum HistoryElementType
{
    HISTORY_FILE_TYPE,
    HISTORY_FOLDER_TYPE,
    HISTORY_TEXT_TYPE,
    HISTORY_URL_TYPE
};

/**
 * @class History Element
 *
 * Element of the history
 * Decribes the received file
 */
class HistoryElement : public QObject
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    explicit HistoryElement(QObject *parent = 0);
    /**
     * Constructor
     *
     * @param date Date time for the file receiption
     * @param text Text of the element
     * @param name Name of the device
     * @param type Type of the element
     */
    HistoryElement(const QDateTime &date, const QString &text, const QString &name, qint64 size, HistoryElementType type, QObject *parent = 0);
    /**
      * Operator =
      */
    HistoryElement& operator=(const HistoryElement &historyElement);
    /**
      * Operator ==
      */
    bool operator==(const HistoryElement &historyElement);
    /**
     * Copy constructor
     */
    HistoryElement(const HistoryElement &element);

    /**
     * Setter : _date
     */
    void setDate(const QDateTime &date);
    /**
     * Setter : _size
     */
    void setSize(qint64 size);
    /**
     * Setter : _txt
     */
    void setText(const QString &text);
    /**
     * Setter : _name
     */
    void setName(const QString &name);
    /**
      * Setter : _type
      */
    void setType(HistoryElementType type);
    /**
      * Getter : _name
      */
    QString getName();
    /**
      * Getter : _size
      */
    qint64 getSize();
    /**
     * Getter : _date
     *
     * @param format Format of the date time
     * @return Formatted string
     */
    QString getDateTime(const QString &format) const;
    /**
     * Getter : _filename
     */
    QString getText() const;
    /**
      * Getter : _type
      */
    HistoryElementType getType() const;
    /**
     * Check if the element is a file (_type == FILE_FOLDER_TYPE)
     * @return
     */
    bool isFile();

    /**
     * Serialization function
     */
    friend QDataStream &operator<<(QDataStream &out, const HistoryElement &element);
    /**
     * Deserialization function
     */
    friend QDataStream &operator>>(QDataStream &in, HistoryElement &element);
    
signals:
    
public slots:

private:
    /// Reception date
    QDateTime _date;
    /// Text corresponding to a file name, an url, or a simple string
    QString _txt;
    /// Type of the element
    HistoryElementType _type;
    /// Name of the sender/receiver
    QString _name;
    /// Size of the element (file / text)
    qint64 _size;
};

#endif // HISTORYELEMENT_H
