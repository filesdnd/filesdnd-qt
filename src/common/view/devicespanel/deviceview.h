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

#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QTimer>
#include <QMimeData>

#include "device.h"
#include "helpers/filehelper.h"
#include "abstractdeviceview.h"

namespace Ui {
class DeviceView;
}

/**
  * @class DeviceView
  *
  * A device as a widget, on the view
  */
class DeviceView : public AbstractDeviceView
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      *
      * @param name Label to display
      * @param uid The uid of the device
      * @param type Device type for the device icon
      * @param available Defines if the device is available or not
      * @param state Last state of the device
      * @param progress The progress of the upload (percentage)
      * @param parent Device's parent widget
      */
    explicit DeviceView(const QString &name,
                        const QString &uid,
                        DeviceType type,
                        bool available,
                        TransfertState state,
                        unsigned progress,
                        QWidget *parent);
    /**
      * Destructor
      */
    ~DeviceView();

    /**
      * Set the device name on the view
      *
      * @param name Name to display
      */
    void setDeviceName(const QString &name);

    /**
      * Defines if the device is available or not
      *
      * @param available Availability
      * @param state State of the last transfert
      */
    void setAvailable(bool available, TransfertState state = NOSTATE);
    /**
     * Update the progress bar
     *
     * @param message Message to display
     * @param progress Upload progress as a percentage
     */
    void updateProgress(const QString &message, unsigned progress);
    /**
      * @overload QWidget::dragEnterEvent(QDragEnterEvent *event)
      */
    virtual void dragEnterEvent(QDragEnterEvent *event);
    /**
      * @overload QWidget::dropEvent(QDropEvent *event)
      */
    virtual void dropEvent(QDropEvent *event);
    /**
      * @overload QWidget::dragMoveEvent(QDragMoveEvent *event)
      */
    virtual void dragMoveEvent(QDragMoveEvent *event);
    /**
      * @overload QWidget::dragLeaveEvent(QDragLeaveEvent *event)
      */
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    /**
     * Retrieve the last stage of the device
     */
    TransfertState lastState();
    /**
     * Display the right icon color depending on the widget visibility
     */
    void manageWidgetVisibility();

private slots:
    /**
     * Set the progressWidget invisible
     */
    void onProgressTimerOut();
    /**
     * Cancel the current transfert
     */
    void on_cancelButton_clicked();
    /**
     * On widget visibility clicked
     */
    void on_widgetButton_clicked();

private:
    /// Device GUI
    Ui::DeviceView *ui;
    /// ProgressBar visibility timer
    QTimer _progressTimer;
    /// State of the transfert
    TransfertState _state;

    /**
     * Manage the device on availability
     *
     * @param state Transfert state
     */
    void deviceAvailable(TransfertState state);
    /**
     * Manage the device on unavailability
     *
     * @param state Transfert state
     */
    void deviceUnavailable(TransfertState state);
    /**
     * Drag enter in the left panel
     */
    void dragInLeftPanel();
    /**
     * Drag move goes out of the left pannel
     */
    void dragOutLeftPanel();
    /**
     * Drag enter in the right pannel
     */
    void dragInRightPanel();
    /**
     * Drag move goes out of the right pannel
     */
    void dragOutRightPanel();
    /**
     * Change the panels visibility
     *
     * @param visible New panels visibility
     */
    void setPanelsVisible(bool visible);
    /**
     * Refresh the panels choises for two panels
     *
     * @param left Left label
     * @param right Right label
     */
    void refreshPanels(const QString &left, const QString &right);
    /**
     * Refresh the panel choise (only one label)
     *
     * @param left Left label
     * @param right Right label
     */
    void refreshPanels(const QString &left);
    /**
     * Refresh the labels correspondings to the drag data type
     */
    void updateLabels();
    /**
     * Check if the mouse if on the device
     */
    bool isMouseOnDevice();
    /**
     * Check if the mouse if on the left panel
     */
    bool isMouseOnLeftPanel();
    /**
     * Check if the mouse if on the right panel
     */
    bool isMouseOnRightPanel();
    /**
     * Send the proper data to the device
     *
     * @param mimeData Data
     */
    void sendProperData(const QMimeData *mimeData);
};

#endif // DEVICEVIEW_H
