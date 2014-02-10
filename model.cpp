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

#include "model.h"

#define DEVICE_NOT_FOUND -1

Model::Model()
{
}

Model::~Model()
{
    clearDevices();
}

void Model::clearDevices()
{
    foreach (Device *device, _devices)
    {
        delete device;
    }
    _devices.clear();
}

void Model::addDevice(Device *device)
{
    _devices.push_back(device);
}

QList<Device*> Model::getDevices() const
{
    return _devices;
}

QList<Device*> Model::getSortedDevices()
{
    QList<Device*> sortedDevices;
    bool added;

    foreach(Device *device, _devices)
    {
        added = false;
        if (sortedDevices.isEmpty())
            sortedDevices.append(device);
        else
        {
            for (int i = 0; i < sortedDevices.size() && !added; ++i)
            {
                if (device->getName() <= sortedDevices.at(i)->getName())
                {
                    sortedDevices.insert(i, device);
                    added = true;
                }
            }
            if (!added)
                sortedDevices.push_back(device);
        }
    }

    return sortedDevices;
}

Device* Model::getDeviceByUID(const QString& uid) const
{
    foreach (Device *device, _devices) {
        if(device->getUID() == uid) {
            return device;
        }
    }
    return NULL;
}

void Model::addDeviceToList(Device *newDevice)
{
    int recordIndex = index(_newDevices, newDevice);

    if(newDevice->getUID() == SettingsManager::getDeviceUID() || recordIndex != DEVICE_NOT_FOUND) {
        if (recordIndex != DEVICE_NOT_FOUND) {
             _newDevices.at(recordIndex)->mergeAddresses(newDevice->getHostInfo());
             _newDevices.at(recordIndex)->setPort(newDevice->getPort());
             _newDevices.at(recordIndex)->setVersion(newDevice->getVersion());
        }

        delete newDevice;
        return;
    }

    recordIndex = index(_devices, newDevice);
    if (recordIndex != DEVICE_NOT_FOUND)
    {
        Device *device = _devices.takeAt(recordIndex);
        // We update the current record

        device->setName(newDevice->getName());
        device->setHostInfo(newDevice->getHostInfo());
        device->setPort(newDevice->getPort());
        device->setVersion(newDevice->getVersion());
        device->setDetectedBy(newDevice->getDetectedBy() | device->getDetectedBy());
        device->mergeAddresses(newDevice->getHostInfo());
        _newDevices.push_back(device);

        delete newDevice;
    }
    else
    {
        _newDevices.push_back(newDevice);

        emit newDeviceCreated(newDevice);
    }
}

void Model::onDeviceNotResponding(Device *device)
{
    if (device)
    {
        _devices.removeOne(device);
        delete device;

        emit deviceRemoved();
    }
}

void Model::cleanDetectedBy(int type)
{
    foreach (Device *device, _devices)
    {
        if (device->getDetectedBy() & type)
            device->setDetectedBy(device->getDetectedBy() ^ type);
    }
}

void Model::updateDevices()
{
    // In case of a record disappear, look for the connected devices
    foreach (Device *device, _devices)
    {
        if (!_newDevices.contains(device) && device->getDetectedBy() > 0 || device->isConnected())
            _newDevices.push_back(_devices.takeAt(_devices.indexOf(device)));
    }

    clearDevices();
    _devices = _newDevices;
    _newDevices.clear();
}

int Model::index(QList<Device *> &lst, Device *device) const
{
    int deviceIndex = DEVICE_NOT_FOUND;

    for (int i = 0; i < lst.size() && deviceIndex == DEVICE_NOT_FOUND; ++i)
    {
        if (lst.at(i)->getUID() == device->getUID())
        {
            deviceIndex = i;
        }
    }

    return deviceIndex;
}
