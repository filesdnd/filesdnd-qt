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

#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>

namespace Ui {
class AboutWidget;
}

/**
  * @class AboutDialog
  *
  * About this application
  */
class AboutWidget : public QWidget
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      */
    explicit AboutWidget(QWidget *parent = 0);
    /**
      * Destructor
      */
    ~AboutWidget();

    /**
     * Show the about dialog and set the proper size (depending on the OS)
     */
    void showAbout();
    
private slots:
    void on_playLinkButton_clicked();

    void on_facebookLinkButton_clicked();

    void on_googleLinkButton_clicked();

    void on_twitterLinkButton_clicked();

    void on_githubLinkButton_clicked();

private:
    /// GUI
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
