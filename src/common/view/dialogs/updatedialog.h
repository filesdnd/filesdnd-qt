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

#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class UpdateDialog;
}

/**
 * @class UpdateDialog
 *
 * Dialog made to update the software
 */
class UpdateDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * Constructor
     */
    explicit UpdateDialog(QWidget *parent = 0);
    /**
      * Destructor
      */
    ~UpdateDialog();

    /**
     * Update the text on the dialog and display the dialog
     *
     * @param version New version as a String
     * @param note Release note
     */
    void updateAndShow(const QString &version, const QString &note);
    
private slots:
    /**
     * On update button clicked
     */
    void on_updateButton_clicked();
    /**
     * On update later button clicked
     */
    void on_laterButton_clicked();
    /**
     * On ignored button clicked
     */
    void on_ignoreButton_clicked();

private:
    /// Gui
    Ui::UpdateDialog *ui;

    /// Version to update
    QString _version;
};

#endif // UPDATEDIALOG_H
