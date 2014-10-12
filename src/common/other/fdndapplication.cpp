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

#include "fdndapplication.h"
#include "helpers/logmanager.h"

#include <QDebug>

#ifdef Q_OS_MACX

#include <objc/objc.h>
#include <objc/message.h>

bool dockClickHandler(id self,SEL _cmd,...)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    ((FDNDApplication*)qApp)->onClickOnDock();
    return true;
}

bool FDNDApplication::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
        quit();
    }

    return QApplication::event(event);
}

#endif

FDNDApplication::FDNDApplication(int argc, char *argv[]) :
    QtSingleApplication(argc, argv)
{
#ifdef Q_OS_MAC
    Class cls = objc_getClass("NSApplication");
    SEL sharedApplication = sel_registerName("sharedApplication");
    objc_object *appInst = objc_msgSend((objc_object*)cls,sharedApplication);

    if(appInst != NULL)
    {
        objc_object* delegate = objc_msgSend(appInst, sel_registerName("delegate"));
        objc_object* delClass = objc_msgSend(delegate,  sel_registerName("class"));
        //const char* tst = class_getName(delClass->isa);
        bool test = class_addMethod((Class)delClass, sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:"), (IMP)dockClickHandler,"B@:");

        if (!test)
        {
            LogManager::appendLine("[FDNDApplication] Cannot get dock handler");
        }
    }

#endif
}

void FDNDApplication::onClickOnDock()
{
    LogManager::appendLine("[FDNDApplication] Dock clicked");
    emit dockClicked();
}
