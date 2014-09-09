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

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "deviceview.h"

#include <QWidget>

/**
  * @enum SettingsIndex
  *
  * Index of the tab in the settings view
  */
enum SettingsIndex
{
    GENERAL_INDEX,
    WIDGET_INDEX,
    VIEW_INDEX
};

namespace Ui {
class SettingsWidget;
}

/**
 * @class SettingsWidget
 *
 * Let the user manage the settings
 */
class SettingsWidget : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * Constructor
     */
    explicit SettingsWidget(QWidget *parent = 0);
    /**
     * Destructor
     */
    ~SettingsWidget();

    /**
     * @overload QWidget::showEvent
     */
    void showEvent(QShowEvent* event);
    /**
     * Refresh the settings from the file with the settings manager
     */
    void refreshSettingsFromFile();
    
private slots:
    /**
     * Tray icon state changed
     */
    void on_trayIconEnabled_toggled(bool checked);
    /**
     * Widget state changed
     */
    void on_widgetEnabled_toggled(bool checked);
    /**
     * Log state changed
     */
    void on_logEnabled_toggled(bool checked);
    /**
     * Apply button clicked
     */
    void on_apply_clicked();
    /**
     * Start service at launch state changed
     */
    void on_startServiceAtLaunch_toggled(bool checked);
    /**
     * Auto open files on receive state changed
     */
    void on_autoOpenFiles_toggled(bool checked);
    /**
     * History display state changed
     */
    void on_historyDisplay_currentIndexChanged(int index);
    /**
      * Available color triggered
      */
    void on_availableColor_clicked();
    /**
      * Unavailable color triggered
      */
    void on_unavailableColor_clicked();
    /**
      * Device name changed
      */
    void on_serviceDeviceName_editingFinished();
    /**
     * Destination folder button clicked
     */
    void on_destFolderButton_clicked();
    /**
     * Reset button clicked
     */
    void on_reset_clicked();
    /**
     * Widget at foreground changed
     */
    void on_widgetForeground_toggled(bool checked);
    /**
     * Search update at launch changed
     */
    void on_searchUpdateAtLaunch_toggled(bool checked);
    /*!
     * Start at boot state changed
     */
    void on_startAtBoot_toggled(bool checked);
    /**
     * Open log button clicked
     */
    void on_openLogButton_clicked();
    /**
     * On start minimized state changed
     */
    void on_startMinimized_toggled(bool checked);

public slots:
    /**
     * SLOT : Directory chosen in the dialog
     *
     * @param dirName Name of the chosen directory or empty string
     */
    void onExistingDirectoryOver(const QString &dirName);

signals:
    /**
     * Notify the view to disable the tray
     */
    void trayDisabled();
    /**
     * Notify the view to enable the tray
     */
    void trayEnabled();
    /**
      * Notify the view for a history policy change
      */
    void historyPolicyChanged();
    /**
      * Notify the view for a border color changed
      */
    void refreshDevicesAvailability();
    /**
      * Notify the controller for a service name change
      */
    void serviceNameChanged();
    /**
     * Notify the view for widget reset
     */
    void resetWidgetPosition();
    /**
     * Notify the view for a widget update
     */
    void updateWidgetFlags();
    /**
     * Notify the view that the widget state changed
     */
    void widgetStateChanged();

private:
    /// GUI
    Ui::SettingsWidget *ui;
    /// Exemple device available
    DeviceView *_availableDevice;
    /// Exemple device unavailable
    DeviceView *_unavailableDevice;

    /**
     * Load the css style and replace the proper values (available colors)
     */
    void loadStyle(const QString &availableColor, const QString &unavailableColor);
};

#endif // SETTINGSWIDGET_H
