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

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QFile>
#include <QMutex>

/**
  * @class LogManager
  *
  * Manage the log file from static class
  */
class LogManager
{
public:
    /**
      * Constructor
      */
    LogManager();

    /**
      * Append a line on the log file
      * Prepend the line by the timedate
      *
      * @param logLine The line to log
      */
    static void appendLine(const QString &logLine);
    /**
      * Getter : LogFileName
      *
      * @return The log file name
      */
    static const QString getLogFileName();
    /**
      * Reset the log, clear the file
      */
    static void resetLog();
    /**
     * Open the log file
     */
    static void openLogFile();

private:
    /// Log file
    static QFile LogFile;
    /// Log file name
    static const QString LogFileName;
    /// Mutex for log printing
    static QMutex Mutex;
};

#endif // LOGMANAGER_H
