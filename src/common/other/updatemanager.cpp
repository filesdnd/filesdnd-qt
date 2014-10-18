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

#include "updatemanager.h"
#include "appconfig.h"
#include "helpers/logmanager.h"
#include "helpers/settingsmanager.h"

#include <QStringList>
#include <QDebug>

UpdateManager::UpdateManager()
{
    QString version = CURRENT_VERSION;
    QStringList numbers = version.split(".");

    if (!isValidVersionNumber(version))
        LogManager::appendLine("[UpdateManager] ERROR : " + QString(CURRENT_VERSION) + " isn't a correct local (appconfig.h) version number");
    else
    {
        _first = numbers.at(0).toInt();
        _second = numbers.at(1).toInt();
        _third = numbers.at(2).toInt();
    }

    _updateNeeded = false;
}

void UpdateManager::checkForUpdate()
{
    QString website;

    if (QLocale::system().language() == QLocale::French)
        website = WEB_SITE;
    else
        website = EN_WEB_SITE;

    QUrl url(QString(website).append("/version"));
    QNetworkRequest request(url);

    _versionReply = _server.get(request);

    connect(_versionReply, SIGNAL(finished()), this, SLOT(handleServerResponseForVersion()));
    connect(_versionReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onServerError(QNetworkReply::NetworkError)));
}

void UpdateManager::handleServerResponseForVersion()
{
    char response[4096];
    _updateNeeded = false;

    if (_versionReply->error() == QNetworkReply::NoError)
    {
        if (_versionReply->readLine(response, 4096) != -1)
        {
            _serverVersion.append(QString::fromUtf8(response)).remove("\n");
            if (isValidVersionNumber(response))
            {
                LogManager::appendLine("[UpdateManager] Server version is "
                                       + _serverVersion
                                       + " (current is "
                                       + QString(CURRENT_VERSION)
                                       + ")"
                                       + (SettingsManager::isVersionIgnored(_serverVersion) ? " Version ignored by user" : ""));
                QStringList numbers = _serverVersion.split(".");

                unsigned serverFirst = numbers.at(0).toInt();
                unsigned serverSecond = numbers.at(1).toInt();
                unsigned serverThird = numbers.at(2).toInt();

                if (serverFirst > _first)
                    _updateNeeded = true;
                else
                    if (serverSecond > _second && serverFirst == _first)
                        _updateNeeded = true;
                    else
                        if (serverThird > _third && serverSecond == _second && serverFirst == _first)
                            _updateNeeded = true;
            }



            if (_updateNeeded && !SettingsManager::isVersionIgnored(_serverVersion)) {
                QString website;
                if (QLocale::system().language() == QLocale::French)
                    website = QString(WEB_SITE).append("/releasenote_fr");
                else
                    website = QString(EN_WEB_SITE).append("/releasenote_en");

                QUrl url(website);
                QNetworkRequest request(url);

                _releasenoteReply = _server.get(request);

                connect(_releasenoteReply, SIGNAL(finished()), this, SLOT(handleServerResponseForReleasenote()));
                connect(_releasenoteReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onServerError(QNetworkReply::NetworkError)));
            }
        }
    }
}

void UpdateManager::handleServerResponseForReleasenote()
{
    char response[4096];
    QString releaseNote;

    if (_releasenoteReply->error() == QNetworkReply::NoError) {
        while (_releasenoteReply->readLine(response, 4096) > 0)
            releaseNote.append(QString::fromUtf8(response));

        emit updateNeeded(_serverVersion, releaseNote);
    }
}

void UpdateManager::onServerError(QNetworkReply::NetworkError error)
{
    LogManager::appendLine("[UpdateManager] Network error : " + QString::number(error));
}

bool UpdateManager::isValidVersionNumber(QString version)
{
    return (version.split(".").size() == 3);
}
