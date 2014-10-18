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

#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

/**
 * @class UpdateManager
 *
 * Retrieve software last version on server
 */
class UpdateManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor
     */
    UpdateManager();

    /**
     * Check for the software update
     */
    void checkForUpdate();

private:
    /// Request server
    QNetworkAccessManager _server;
    /// Reply from server for version
    QNetworkReply *_versionReply;
    /// Reply from server for release note
    QNetworkReply *_releasenoteReply;
    /// First number of the version
    unsigned _first;
    /// Second number of the version
    unsigned _second;
    /// Last number of the version
    unsigned _third;
    /// Update needed
    bool _updateNeeded;
    /// Server version
    QString _serverVersion;

    /// Check if the version has a valid format
    bool isValidVersionNumber(QString version);

private slots:
    /// On server reply for version
    void handleServerResponseForVersion();
    /// On server reply for release note
    void handleServerResponseForReleasenote();
    /// On server error
    void onServerError(QNetworkReply::NetworkError error);

signals:
    /// Notify the view for an update
    void updateNeeded(const QString &version, const QString &note);
};

#endif // UPDATEMANAGER_H
