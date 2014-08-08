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

#include "logmanager.h"
#include "settingsmanager.h"
#include "appconfig.h"
#include "helpers/filehelper.h"

#include <QDateTime>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>

const QString LogManager::LogFileName = FileHelper::getFileStorageLocation() + "/" + LOG_FILE;
QFile LogManager::LogFile(LogFileName);
QMutex LogManager::Mutex;

LogManager::LogManager()
{
}

const QString LogManager::getLogFileName()
{
    return LogFileName;
}

void LogManager::openLogFile()
{
    FileHelper::openURL("file:///" + LogFileName);
}

void LogManager::resetLog()
{
    if (LogFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        LogFile.close();
    }
}

void LogManager::appendLine(const QString &logLine)
{
    QString dateTimeString = QDateTime::currentDateTime().toString("[dd/MM/yyyy - hh:mm:ss] ");
    if (SettingsManager::isLogEnabled())
    {
        if (!LogFile.isOpen())
            LogFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        if (LogFile.isOpen() && Mutex.tryLock())
        {
            QTextStream out(&LogFile);

            if (LogFile.size() > 1000 * 512)
            {
                LogFile.reset();
                LogFile.seek(LogFile.size() - 1000 * 8);
                QString content = LogFile.readAll();
                LogFile.resize(0);

                content = content.mid(content.indexOf("\n") + 1);
                out << content;
            }

            out << dateTimeString;
            out << logLine << endl;

            qDebug() << dateTimeString + logLine;

            Mutex.unlock();
        }
    }
}
