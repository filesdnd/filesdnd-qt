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

#ifndef HISTORYELEMENTVIEW_H
#define HISTORYELEMENTVIEW_H

#include <QWidget>
#include <QDateTime>

#include "entities/historyelement.h"

namespace Ui {
class HistoryElementView;
}

/**
 * Element of the view representing an history element
 */
class HistoryElementView : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * Default constructor
     */
    explicit HistoryElementView(QWidget *parent = 0);
    /**
     * Constructor
     *
     * @param date Date of the file reception
     * @param text Text of the history element
     */
    HistoryElementView(const QString &date, const QString &text, HistoryElementType type, QWidget *parent = 0);
    /**
     * Destructor
     */
    ~HistoryElementView();

    /**
     * Update the progress bar with the proper value
     *
     * @param progress Download progress
     */
    void setProgress(unsigned progress);
    /**
     * Change the progress bar visibility state
     *
     * @param enabled True to enable the progress bar, false otherwise
     */
    void setProgressBarEnabled(bool enabled);
    /**
     * Refresh all the values of the view
     */
    void refresh();
    /**
      * Get the url
      */
    QString getUrl() const;
    /**
      * Get the the text of the element
      */
    QString getText() const;
    /**
     * Get the size of the file as a string
     */
    QString getFileSize() const;
    /**
      * Shows if the element is in download
      *
      * @return True if the element is downloading, false otherwise
      */
    bool isDownloading() const;
    /**
      * Getter : _type
      */
    HistoryElementType getType() const;

signals:
    /**
     * Notify that a transfert is canceled
     */
    void cancelIncomingTransfert();
    
private slots:
    /**
     * On cancel button clicked
     */
    void on_cancelButton_clicked();

private:
    /// GUI
    Ui::HistoryElementView *ui;
    /// Text of the element, can be a filename, an url, or a simple string
    QString _text;
    /// Type of the element
    HistoryElementType _type;
    /// Date of the history element
    QString _date;

    /**
      * Find the text to display corresponding to the display type
      *
      * @see HistoryElementType
      * @param type The display type
      * @param text The original text
      * @return The truncated text
      */
    QString textForType(HistoryElementType type, QString text);

    /**
     * Trucate the user name to fit the label
     *
     * @param fullDataName Full size name
     * @return Trucated name
     */
    QString trucateName(QString fullDataName);
};

#endif // HISTORYELEMENTVIEW_H
