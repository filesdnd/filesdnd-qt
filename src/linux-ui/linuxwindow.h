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

#ifndef LinuxWindow_H
#define LinuxWindow_H

#include <QMainWindow>
#include <QShortcut>

#include "view.h"

namespace Ui {
class LinuxWindow;
}

class LinuxWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LinuxWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view, QWidget *parent = 0);
    ~LinuxWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionSettings_triggered();

private:
    Ui::LinuxWindow *ui;
    QApplication *_app;
    View *_view;
    QCloseEvent _closeEvent;
    QShortcut *_cmdWShortcut;
    QShortcut *_cmdQShortcut;

    void createLinuxMenu();
};

#endif // LinuxWindow_H
