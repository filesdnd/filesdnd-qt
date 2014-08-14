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

#include "servicehelper.h"
#include "helpers/logmanager.h"

#include <QDebug>

ServiceHelper::ServiceHelper()
{
}

BonjourServiceState ServiceHelper::getBonjourServiceState()
{
    BonjourServiceState serviceState = BONJOUR_SERVICE_OK;

#if defined(Q_OS_WIN)
    serviceState = windowsServiceBonjourState();
#elif defined(Q_OS_LINUX) || defined(__linux__)
    serviceState = linuxServiceBonjourState();
#endif

    return serviceState;
}

bool ServiceHelper::isBonjourServiceAvailable()
{
    return (getBonjourServiceState() == BONJOUR_SERVICE_OK);
}
#if defined(Q_OS_LINUX) || defined(__linux__)
BonjourServiceState ServiceHelper::linuxServiceBonjourState()
{
    BonjourServiceState serviceState = BONJOUR_SERVICE_OK;

    QFile avahi("/var/run/avahi-daemon/pid");
    if (avahi.exists())
        serviceState = BONJOUR_SERVICE_OK;
    else
        serviceState = BONJOUR_SERVICE_NOT_STARTED;

    return serviceState;
}

#endif

#if defined(Q_OS_WIN)
BonjourServiceState ServiceHelper::windowsServiceBonjourState()
{
    BonjourServiceState serviceState = BONJOUR_SERVICE_OK;
    int servicePosition;
    SC_HANDLE schSCManager;
    QString bonjourServiceName;

    schSCManager = OpenSCManager(
        NULL,                    // Local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ENUMERATE_SERVICE);  // Minimum right (no need admin rights)

    if (schSCManager == NULL)
    {
        LogManager::appendLine("[ServiceHelper - Windows] OpenSCManager ERROR : " + QString::number(GetLastError()));
        serviceState = BONJOUR_SERVICE_FAILED;
    }
    else
    {
        ENUM_SERVICE_STATUS *winServices = new ENUM_SERVICE_STATUS[1000];

        DWORD pcbBytesNeeded;
        DWORD lpResumeHandle = 0;
        DWORD m_nServices;
        bool res = EnumServicesStatus(schSCManager,
                          SERVICE_WIN32,
                          SERVICE_STATE_ALL,
                          winServices,
                          1000 * sizeof(ENUM_SERVICE_STATUS),
                          &pcbBytesNeeded,
                          &m_nServices,
                          &lpResumeHandle );

        if(!res)
        {
            LogManager::appendLine("[ServiceHelper - Windows] EnumServicesStatus ERROR : " + QString::number(GetLastError()));
        }
        else
        {
            bonjourServiceName = findBonjourService(winServices, m_nServices, &servicePosition);
            if (servicePosition == -1)
            {
                serviceState = BONJOUR_SERVICE_NOT_FOUND;
            }
            else
            {
                switch (winServices[servicePosition].ServiceStatus.dwCurrentState)
                {
                case SERVICE_RUNNING:
                    serviceState = BONJOUR_SERVICE_OK;
                    break;
                case SERVICE_STOPPED:
                    serviceState = BONJOUR_SERVICE_NOT_STARTED;
                    break;
                }
            }
        }
        delete[] winServices;
    }
    return serviceState;
}

QString ServiceHelper::findBonjourService(ENUM_SERVICE_STATUS *services, DWORD size, int *servicePosition)
{
    QString fail = "";
    QString tmp;
    for (DWORD i = 0; i < size; ++i)
    {
        tmp = QString::fromUtf16((const ushort*)services[i].lpServiceName);
        if (tmp.contains("bonjour", Qt::CaseInsensitive))
        {
            *servicePosition = i;
            return tmp;
        }
    }

    *servicePosition = -1;
    return fail;
}
#endif
