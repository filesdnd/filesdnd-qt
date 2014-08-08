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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QSharedMemory>
#include <QDebug>

#include "controller.h"
#include "settingsmanager.h"
#include "appconfig.h"
#include "autotest.h"
#include "fdndapplication.h"

int main(int argc, char *argv[])
{
#ifdef RUN_TESTS
    int failures = AutoTest::run(argc, argv);

    if (failures == 0)
        qDebug() << "ALL TESTS PASSED";
    else
        qDebug() << failures << " TESTS FAILED!";
#endif

    FDNDApplication app(argc, argv);
    QTranslator translator;

    if (QLocale::system().language() == QLocale::French)
        translator.load("filesdnd_fr", QCoreApplication::applicationDirPath());
    else
        translator.load("filesdnd_en", QCoreApplication::applicationDirPath());
    app.installTranslator(&translator);

    if(app.isRunning())
    {
        QString message;

        message.append(app.id());
        message.append(";");
        message.append(QString::number(argc));
        for (int i = 0; i < argc; ++i)
        {
             message.append(";");
             message.append(argv[i]);
        }

        app.sendMessage(message);

        return 0;
    }

    SettingsManager::loadSettingsFile();

    Controller controller;

    QApplication::setOrganizationName("Files Drag & Drop");

    QObject::connect(&app, SIGNAL(messageReceived(QString)), &controller, SLOT(onOtherInstance(QString)));
    QObject::connect(&app, SIGNAL(dockClicked()), &controller, SLOT(onDockClicked()));

    controller.startView();

    return app.exec();
}
