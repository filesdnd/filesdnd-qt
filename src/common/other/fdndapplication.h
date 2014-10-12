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

#ifndef FDNDAPPLICATION_H
#define FDNDAPPLICATION_H

#include <QObject>

#include "qtsingleapplication/singleapplication/qtsingleapplication.h"

/**
 * Application class that will allow to control system event
 * as the dock events on the mac
 */
class FDNDApplication : public QtSingleApplication
{
    Q_OBJECT
public:
    /// Constructor
    explicit FDNDApplication(int argc, char *argv[]);

#ifdef Q_OS_MACX
    /// Lookup close event
    bool event(QEvent *);
#endif

signals:
    /**
     * Notify that the dock was clicked
     */
    void dockClicked();
    
public slots:
    /**
     * On mac dock clicked (use the low level objc API)
     */
    void onClickOnDock();
    
};

#endif // FDNDAPPLICATION_H
