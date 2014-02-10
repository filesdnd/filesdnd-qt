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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

/**
  * @class AboutDialog
  *
  * About this application
  */
class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      */
    explicit AboutDialog(QWidget *parent = 0);
    /**
      * Destructor
      */
    ~AboutDialog();

    /**
     * Show the about dialog and set the proper size (depending on the OS)
     */
    void showAbout();
    
private:
    /// GUI
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
