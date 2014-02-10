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

#include "widget.h"
#include "../helpers/settingsmanager.h"
#include "appconfig.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QScrollBar>

Widget::Widget(QWidget *parent) :
    _parent(parent)
{
}

Widget::~Widget()
{
    clearDevices();
}

void Widget::canBeShown()
{
    foreach (SmallDeviceView *device, _devices)
    {
        device->canBeShown();
    }
}

void Widget::onWidgetDoubleClicked()
{
    emit doubleClicked();

    hideWidgets();
}

void Widget::hideWidgets()
{
    foreach (SmallDeviceView *device, _devices)
    {
        device->startFadeOut();
    }
}

void Widget::updateWindowFlags()
{
    foreach (SmallDeviceView *device, _devices)
    {
        device->updateWindowFlags();
    }
}

void Widget::addDevice(const QString &name, const QString &uid, DeviceType type, bool available)
{
    SmallDeviceView *smallDevice = new SmallDeviceView(name, uid, type, available, _parent);

    connect(smallDevice, SIGNAL(sendFileSignal(const QString&, const QList<QUrl>&, DataType)),
            this, SLOT(onSendFile(const QString&, const QList<QUrl>&, DataType)));
    connect(smallDevice, SIGNAL(sendTextSignal(const QString&, const QString&, DataType)),
            this, SLOT(onSendText(const QString&, const QString&, DataType)));
    connect(smallDevice, SIGNAL(doubleClicked()),
            this, SLOT(onWidgetDoubleClicked()));
    connect(smallDevice, SIGNAL(hideAllWidgets()),
            this, SLOT(onHideAllWidgets()));

    _devices.append(smallDevice);
}

void Widget::onHideAllWidgets()
{
    foreach (SmallDeviceView *device, _devices)
    {
        device->startFadeOut();
        device->cannotBeShown();
    }
}

void Widget::clearDevices()
{
    foreach(SmallDeviceView *device, _devices)
    {
        device->hide();
        delete device;
    }
    _devices.clear();
}

SmallDeviceView* Widget::getDeviceByUID(const QString& uid) const
{
    foreach (SmallDeviceView *device, _devices)
    {
        if(device->getDeviceUID() == uid)
        {
            return device;
        }
    }
    return NULL;
}

void Widget::setDeviceUnavailable(const QString &uid)
{
    SmallDeviceView *device = getDeviceByUID(uid);

    if (device)
    {
        device->setAvailable(false);
    }
}

void Widget::updateDevices(QList<Device*> devices)
{
    SmallDeviceView *tmp;
    QList<SmallDeviceView*> alreadyDisplayed;

    // Search already displayed devices
    foreach (Device* device, devices)
    {
        tmp = getDeviceByUID(device->getUID());
        if (tmp)
        {
            alreadyDisplayed.append(tmp);
        }
    }

    // Clear all other devices
    foreach (SmallDeviceView *device, _devices)
    {
        if (!alreadyDisplayed.contains(device))
        {
            device->hide();
            _devices.removeOne(device);

            delete device;
        }
    }

    // Add missing devices
    foreach (Device* device, devices)
    {
        tmp = getDeviceByUID(device->getUID());
        if (!tmp)
        {
            addDevice(device->getName(), device->getUID(), device->getType(), device->isAvailable());
        }
    }
}

void Widget::setDeviceAvailable(const QString &uid, bool available)
{
    if (available)
        setDeviceAvailable(uid);
    else
        setDeviceUnavailable(uid);
}

void Widget::setDeviceAvailable(const QString &uid)
{
    SmallDeviceView *device = getDeviceByUID(uid);

    if (device)
    {
        device->setAvailable(true);
    }
}

void Widget::onSendFile(const QString &name, const QList<QUrl> &urls, DataType type)
{
    emit sendFile(name, urls, type);
}

void Widget::onSendText(const QString &name, const QString &string, DataType type)
{
    emit sendText(name, string, type);
}

void Widget::showWidgets()
{
    foreach (SmallDeviceView *device, _devices)
    {
        device->initPosition();
        device->startFadeIn();
    }
}
