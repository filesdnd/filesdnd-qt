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

#ifndef MODEL_H
#define MODEL_H

#include <QList>
#include <QObject>
#include <QDebug>
#include <QStringList>

#include "zeroconf/bonjourrecord.h"
#include "device.h"
#include "helpers/settingsmanager.h"
#include "helpers/logmanager.h"

/**
  * @class Model
  *
  * MVC Model
  * Contains the devices
  */
class Model : public QObject
{
    Q_OBJECT
public:
    /**
      * Constructor
      */
    Model();
    /**
      * Destructor
      */
    ~Model();

    /**
      * Add a device to the device list
      *
      * @param device Device to add
      */
    void addDevice(Device *device);
    /**
      * Getter : _devices
      *
      * @return Device list
      */
    QList<Device*> getDevices() const;
    /**
      * Getter : _devices
      *
      * @return Device list sorted
      */
    QList<Device*> getSortedDevices();
    /**
      * Find a device by its name
      *
      * @param uid Device UID
      * @return Device searched or NULL if not found
      */
    Device* getDeviceByUID(const QString& uid) const;
    /**
     * Add a new device by record. Update the old one if it exists.
     * @param record The device to add
     */
    void addDeviceToList(Device *newDevice);
    /**
     * Reset the detection type
     *
     * @param type Device type
     */
    void cleanDetectedBy(int type);
    /**
      * Clear the device list
      */
    void clearDevices();
    /**
      * Search a device index from a device
      *
      * @param device The device to find
      * @return The device index or -1 if not found
      */
    int index(QList<Device *> &lst, Device *device) const;
    /**
     * Check for old entry and copy the new list to the old one
     */
    void updateDevices();

signals:
    /**
      * Notify the controller for a new device
      *
      * @param device Device created
      */
    void newDeviceCreated(Device *device);
    /**
     * Notify that a device has been removed
     */
    void deviceRemoved();

public slots:
    /**
     * Notify that a device is not responding
     *
     * @param device Device not responding
     */
    void onDeviceNotResponding(Device *device);

private:
    /// Device list
    QList<Device*> _devices;
    /// Tmp device list
    QList<Device*> _newDevices;
};

#endif // MODEL_H
