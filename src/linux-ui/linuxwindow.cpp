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

#include "linuxwindow.h"
#include "ui_linuxwindow.h"

#include <QShortcut>

LinuxWindow::LinuxWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view, QWidget *parent) :
    QMainWindow(parent),
    _app(app),
    _view(view),
    ui(new Ui::LinuxWindow)
{
    ui->setupUi(this);

    connect(view, SIGNAL(showWindow()), this, SLOT(show()));
    connect(view, SIGNAL(hideWindow()), this, SLOT(hide()));

    //setWindowTitle("Files Drag & Drop");
    setGeometry(x, y, width, height);
    setStyleSheet(FileHelper::loadFileContent(LINUX_APP_CSS));

    ui->centralwidget->layout()->addWidget(view);

    _cmdWShortcut = new QShortcut(QKeySequence("Ctrl+w"), this);
    _cmdQShortcut = new QShortcut(QKeySequence("Ctrl+q"), this);

    connect(_cmdWShortcut, SIGNAL(activated()), this, SLOT(close()));
    connect(_cmdQShortcut, SIGNAL(activated()), qApp, SLOT(quit()));

    ui->menubar->setVisible(false);
    //createLinuxMenu();
}

LinuxWindow::~LinuxWindow()
{
    delete ui;
    delete _cmdWShortcut;
    delete _cmdQShortcut;
}

void LinuxWindow::createLinuxMenu() {
    ui->actionSettings->setMenuRole(QAction::PreferencesRole);
}

void LinuxWindow::closeEvent(QCloseEvent *event)
{
    _view->closeEvent(&_closeEvent);
    event->ignore();
}

void LinuxWindow::on_actionSettings_triggered()
{
    _view->onSettingsActionTriggered();
}
