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

#ifndef BORDERLESSPANEL_H
#define BORDERLESSPANEL_H

#include <QMouseEvent>

#include "qwinwidget.h"
#include "view.h"

class BorderlessPanel : public QWinWidget {
    Q_OBJECT

public:
    BorderlessPanel(HWND hWnd, View *view);
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

    void maximizeButtonState();
    void restoreButtonState();

private:
    HWND _windowHandle;
    View *_view;
    QCloseEvent _closeEvent;
    QPushButton *_pushButtonMaximize;
};

#endif // BORDERLESSPANEL_H
