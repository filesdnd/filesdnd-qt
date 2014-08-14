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

#ifndef SERVICEHELPER_H
#define SERVICEHELPER_H

#include <QString>

#if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
#include "Windows.h"
#include "WinSvc.h"
#elif defined(Q_OS_LINUX) || defined(__linux__)
#include <QProcess>
#endif

/**
  * @enum BonjourServiceState
  *
  * Represents the state of the Bonjour service
  */
enum BonjourServiceState
{
    BONJOUR_SERVICE_FAILED,
    BONJOUR_SERVICE_NOT_FOUND,
    BONJOUR_SERVICE_NOT_STARTED,
    BONJOUR_SERVICE_OK
};

/**
  * Handle services for windows, and deamons for Linux
  */
class ServiceHelper
{
public:
    /**
      * Constructor
      */
    ServiceHelper();

    /**
      * Search the Bonjour service
      *
      * @return Service Bonjour state
      */
    static BonjourServiceState getBonjourServiceState();
    /**
      * Tells if the bonjour service is ready to works
      *
      * @return True if the bonjour service is ready, false otherwise
      */
    static bool isBonjourServiceAvailable();

private:
#if defined(Q_OS_LINUX) || defined(__linux__)
    /**
     * Check the avahi daemon state
     *
     * @return The Bonjour service state
     */
    static BonjourServiceState linuxServiceBonjourState();
#endif
#if defined(Q_OS_WIN)
    /**
      * Find the bonjour service in a complete list of windows services
      *
      * @param service List of services
      * @param size Size of the list
      * @param servicePosition Integer pointer, will be fill with the service index or -1, in case of error
      * @return The service name if it was found, empty string else
      */
    static QString findBonjourService(ENUM_SERVICE_STATUS *services, DWORD size, int *servicePosition);
    /**
      * Check the bonjour service in the windows services
      *
      * @return The Bonjour service state
      */
    static BonjourServiceState windowsServiceBonjourState();
#endif
};

#endif // SERVICEHELPER_H
