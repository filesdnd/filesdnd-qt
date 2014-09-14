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

#ifndef BORDERLESSWINDOW_H
#define BORDERLESSWINDOW_H

#include <windows.h>

#include "borderlesspanel.h"
#include "qwinwidget.h"
#include "view.h"

#include <QApplication>

class BorderlessWindow : public QObject
{
    Q_OBJECT

    enum class Style : DWORD
    {
        windowed = ( WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_SYSMENU  ),
        aero_borderless = ( WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN )
    };

public:
    HWND hWnd;
    static HINSTANCE hInstance;

    BorderlessWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view);
    ~BorderlessWindow();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool isVisible();

    void toggleBorderless();
    void toggleShadow();
    void toggleResizeable();
    bool isResizeable();

    void setMinimumSize( const int width, const int height );
    bool isSetMinimumSize();
    void removeMinimumSize();
    int getMinimumHeight();
    int getMinimumWidth();

    void setMaximumSize( const int width, const int height );
    bool isSetMaximumSize();
    int getMaximumHeight();
    int getMaximumWidth();
    void removeMaximumSize();

public slots:
    void show();
    void hide();

private:
    static QApplication *a;
    static BorderlessPanel *mainPanel;
    static View *view;
    static QFocusEvent focusEvent;

    bool closed;
    bool visible;

    bool borderless;
    bool aeroShadow;
    bool borderlessResizeable;

    struct sizeType {
        sizeType() : required(false), width(0), height(0) {}
        bool required;
        int width;
        int height;
    };

    sizeType minimumSize;
    static LONG borderWidth;
    sizeType maximumSize;
};

#endif // BORDERLESSWINDOW_H
