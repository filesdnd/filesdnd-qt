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

#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>

#include "transparentscrollbar.h"
#include "historyelement.h"
#include "view.h"

namespace Ui {
class ConfigPanel;
}

/**
 * @class ConfigPanel
 *
 * The configuration panel is the main left panel.
 * This panel contains the history and the controls
 */
class ConfigPanel : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit ConfigPanel(QWidget *parent = 0);
    /// Destructor
    ~ConfigPanel();

    /**
     * Set the main view
     * Th main view if the central widget of the application
     *
     * @see View
     * @param view A pointer to the main views
     */
    void setMainView(View *view);
    /**
     * Open the download folder in the operating system window
     */
    void openDownloadFolder();

    /// See QWidget mousePressEvent
    void mousePressEvent(QMouseEvent *event);

private slots:
    /**
     * Refresh button clicked : refresh the devices
     */
    void on_refreshButton_clicked();
    /**
     * Update the spinning loading gif
     */
    void updateRefreshFrame(int frame);

public slots:
    /**
    * SLOT : on history changed
    * @param history New history value
    */
    void onHistoryChanged(const QList<HistoryElement> &history);

signals:
    /// Sent when the settings button is clicked
    void settingsTriggered();
    /// Sent when the about button is clicked
    void aboutTriggered();
    /// Sent when the devices button is clicked
    void devicesTriggered();

private:
    /// GUI
    Ui::ConfigPanel *ui;
    /// Loading gif
    QMovie _refreshMovie;
    /// Main view pointer
    View *_view;
    /// Animated scrollbar
    TransparentScrollBar *_historyScrollBar;

    /**
     * Create the advances history list widget with animated scrollbar
     */
    void createHistoryListWidget();
    /**
    * Clear the history
    */
    void clearHistory();
};

#endif // CONFIGPANEL_H
