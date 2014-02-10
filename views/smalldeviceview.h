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

#ifndef SMALLDEVICEVIEW_H
#define SMALLDEVICEVIEW_H

#include "abstractdeviceview.h"

#include <QMenu>
#include <QMouseEvent>
#include <QApplication>
#include <QPropertyAnimation>

namespace Ui {
class SmallDeviceView;
}

/**
  * @class SmallDeviceView
  *
  * View for a device in the Widget
  */
class SmallDeviceView : public AbstractDeviceView
{
    Q_OBJECT
    
public:
    /**
      * Contructor
      *
      * @param name Label to display
      * @param uid The uid of the device
      * @param type Device type for the device icon
      * @param available Defines if the device is available or not
      * @param parent Device's parent widget
      */
    explicit SmallDeviceView(const QString &name, const QString &uid, DeviceType type, bool available, QWidget *parent);
    /**
      * Destructor
      */
    ~SmallDeviceView();

    /**
     * Tells that the widget can be shown
     */
    void canBeShown();
    /**
     * Tells that the widget cannot be shown anymore
     */
    void cannotBeShown();
    /**
      * Create actions for the history context menu
      */
    void createContextMenuActions();
    /**
     * Update the flags of the widget (depending on options)
     */
    void updateWindowFlags();
    /**
     * Init animation
     */
    void initAnimation();
    /**
     * Fade out the widget with animation (if visible)
     */
    void startFadeOut();
    /**
     * Fade in the widget with animation (if hidden)
     */
    void startFadeIn();

    /**
      * Init the position of the widget
      **/
    void initPosition();

    /**
      * Set the device name on the view
      *
      * @param name Name to display
      */
    virtual void setDeviceName(const QString &name);
    /**
      * Defines if the device is available or not
      *
      * @param available Availability
      * @param state Last transfert state
      */
    virtual void setAvailable(bool available, TransfertState state = NOSTATE);
    /**
      * @overload QWidget::dragEnterEvent(QDragEnterEvent *event)
      */
    virtual void dragEnterEvent(QDragEnterEvent *event);
    /**
      * @overload QWidget::dropEvent(QDropEvent *event)
      */
    virtual void dropEvent(QDropEvent *event);
    /**
     * @overload QWidget::mouseDoubleClickEvent
     */
    void mouseDoubleClickEvent(QMouseEvent *);

public slots:
    /**
     * Show the history menu
     */
    void onHistoryViewContextMenuRequested(const QPoint &pos);
    /**
     * On hide all widgtes action triggered
     */
    void onHideAllWidgets();
    /**
     * On hide this widget triggered
     */
    void onHideWidget();
    /**
     * On restore main view triggered
     */
    void onRestoreMainViewAction();

signals:
    /**
     * Notify the widget manager that a double clicked happen
     */
    void doubleClicked();
    /**
     * Notify the widget manager that all widgets should be closed
     */
    void hideAllWidgets();

private:
    /// GUI
    Ui::SmallDeviceView *ui;
    /// Memory for the drag position
    QPoint _dragPosition;
    /// Memory for the drag position relative on the widget
    QPoint _mousePositionOnWidget;
    /// Mouse cursor memory
    QPoint _cursorPos;
    /// Vertical snap
    bool _verticalSnap;
    /// Horizontal snap
    bool _horizontalSnap;
    /// Fade of the widget
    QPropertyAnimation *_widgetAnimation;
    /// Indicate if the widget was closed by clicking on the top right red cross
    bool _wasClosed;
    /// Type of the device
    DeviceType _type;
    /// Context menu
    QMenu _contextMenu;
    /// Context menu action : hide current widget
    QAction *_hideWidgetAction;
    /// Hide all widgets
    QAction *_hideAllWidgetsAction;
    /// Restore the main window
    QAction *_restoreMainViewAction;

    /**
      * @overload QWidget::mousePressEvent
      *
      *  On mouse pressed
      */
    void mousePressEvent(QMouseEvent *event);
    /**
      * @overload QWidget::mousePressEvent
      *
      *  On mouse released
      */
    void mouseReleaseEvent(QMouseEvent *event);
    /**
      * @overload QWidget::mouseMoveEvent
      *
      *  On mwidget moved
      */
    void mouseMoveEvent(QMouseEvent *event);
    /**
      * @overload QWidget::moveEvent
      */
    void moveEvent(QMoveEvent *event);
};

#endif // SMALLDEVICEVIEW_H
