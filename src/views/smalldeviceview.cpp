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

#include "smalldeviceview.h"
#include "helpers/settingsmanager.h"
#include "ui_smalldeviceview.h"

#include <QDesktopWidget>

#if defined(Q_OS_WIN) || defined (Q_OS_WIN32)
#include <windows.h>
#endif

SmallDeviceView::SmallDeviceView(const QString &name, const QString &uid, DeviceType type, bool available, QWidget *parent) :
    AbstractDeviceView(name, uid, available, parent),
    ui(new Ui::SmallDeviceView),
    _horizontalSnap(false),
    _verticalSnap(false),
    _wasClosed(false),
    _type(type)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowOpacity(WIDGET_OPACITY);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->backgroundContainer->setStyleSheet("#backgroundContainer"
                                           "{"
                                               "border-radius: 13px;"
                                               "border: 0px;"
                                               "background-color: rgba(220, 220, 220," + QString::number(WIDGET_OPACITY * 255) + ");"
                                           "}");

    // Context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onHistoryViewContextMenuRequested(const QPoint&)));

    setAvailable(available);
    setDeviceName(name);

    initAnimation();
    updateWindowFlags();
    createContextMenuActions();
}

SmallDeviceView::~SmallDeviceView()
{
    delete _restoreMainViewAction;
    delete _widgetAnimation;
    delete _hideWidgetAction;
    delete _hideAllWidgetsAction;

    delete ui;
}

void SmallDeviceView::setAvailable(bool available, TransfertState)
{
    QString style;

    _available = available;
    setAcceptDrops(available);

    switch (_type)
    {
    case TYPE_ANDROID:
        style.append("background: url(:/images/android_icon_small.png) no-repeat right top;\n");
        break;
    case TYPE_WINDOWS:
        style.append("background: url(:/images/windows_icon_small.png) no-repeat right top;\n");
        break;
    case TYPE_MAC:
         style.append("background: url(:/images/apple_icon_small.png) no-repeat right top;\n");
        break;
    case TYPE_LINUX:
         style.append("background: url(:/images/linux_icon_small.png) no-repeat right top;\n");
        break;
    }

    if (available)
    {
        ui->borderContainer->setStyleSheet("#borderContainer"
                                           "{\n"
                                              "border: 2px solid " + SettingsManager::availableDeviceColor() + ";\n"
                                              "border-radius: 7px;\n"
                                              + style +
                                           "}\n");
    }
    else
    {
        ui->borderContainer->setStyleSheet("#borderContainer"
                                           "{\n"
                                              "border: 2px solid " + SettingsManager::unavailableDeviceColor() + ";\n"
                                              "border-radius: 7px;\n"
                                              + style +
                                           "}\n");
    }
}

void SmallDeviceView::setDeviceName(const QString &name)
{
    ui->deviceName->setText(name);
    _name = name;
}

void SmallDeviceView::dragEnterEvent(QDragEnterEvent *event)
{
    AbstractDeviceView::dragEnterEvent(event);
}

void SmallDeviceView::dropEvent(QDropEvent *event)
{
    AbstractDeviceView::sendDataForDefault(event->mimeData());
    AbstractDeviceView::dropEvent(event);
}

void SmallDeviceView::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton)
     {
         _mousePositionOnWidget = event->pos();
         _dragPosition = _mousePositionOnWidget;
         event->accept();
     }
 }

void SmallDeviceView::mouseReleaseEvent(QMouseEvent *)
{
    SettingsManager::setWidgetPosition(_uid, pos());
}

void SmallDeviceView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPoint posi = event->screenPos().toPoint() - _mousePositionOnWidget;
        if(_horizontalSnap && event->localPos().x() > -WIDGET_OFFSET && event->localPos().x() < width() + WIDGET_OFFSET) {
            posi.setX(x());
        }
        if(_verticalSnap && event->localPos().y() > -WIDGET_OFFSET && event->localPos().y() < height() + WIDGET_OFFSET) {
            posi.setY(y());
        }

        move(posi);
        event->accept();
    }
}

void SmallDeviceView::moveEvent(QMoveEvent * event)
{
    QRect geometry = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this));

    _horizontalSnap = false;
    _verticalSnap = false;

    // Left
    if(event->pos().x() < geometry.x() + WIDGET_OFFSET && QCursor::pos().x() > geometry.x() + WIDGET_OFFSET) {
        _horizontalSnap = true;
        move(geometry.x(), pos().y());
    }

    // Right
    if(event->pos().x() > geometry.x() + geometry.width() - width() - WIDGET_OFFSET && QCursor::pos().x() < geometry.x() + geometry.width() - WIDGET_OFFSET) {
        _horizontalSnap = true;
        move(geometry.x() + geometry.width() - width(), pos().y());
    }

    // Top
    if(event->pos().y() < geometry.y() + WIDGET_OFFSET && QCursor::pos().y() > geometry.y() + WIDGET_OFFSET) {
        _verticalSnap = true;
        move(pos().x(), geometry.y());
    }

    // Bottom
    if(event->pos().y() > geometry.y() + geometry.height() - height() - WIDGET_OFFSET && QCursor::pos().y() < geometry.y() + geometry.height() - WIDGET_OFFSET) {
        _verticalSnap = true;
        move(pos().x(), geometry.y() + geometry.height() - height());
    }

    event->accept();
}

void SmallDeviceView::updateWindowFlags()
{
    bool visibility = isVisible();

    if (SettingsManager::isWidgetInForeground())
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
    else if((windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint)
    {
        setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
#if defined(Q_OS_WIN) || defined (Q_OS_WIN32)
        SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED |  SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
#endif
    }

    if (visibility)
    {
        show();
    }
}

void SmallDeviceView::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}

void SmallDeviceView::startFadeOut()
{
    if (!isHidden())
    {
        connect(_widgetAnimation, SIGNAL(finished()), this, SLOT(hide()));
        _widgetAnimation->setEasingCurve(QEasingCurve::InQuint);
        _widgetAnimation->setDirection(QAbstractAnimation::Backward);
        _widgetAnimation->start();
    }
}

void SmallDeviceView::initAnimation()
{
    _widgetAnimation = new QPropertyAnimation(this, "windowOpacity");
    _widgetAnimation->setDuration(WIDGET_ANIMATION_TIMER);

    _widgetAnimation->setStartValue(0);
    _widgetAnimation->setEndValue(WIDGET_OPACITY);
}

void SmallDeviceView::canBeShown()
{
    _wasClosed = false;
}

void SmallDeviceView::cannotBeShown()
{
    _wasClosed = true;
}

void SmallDeviceView::initPosition()
{
    QRect geometry = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this));
    QPoint position = SettingsManager::getWidgetPosition(_uid);

    if ( (QApplication::desktop()->screenCount() == 1) && (position.x() < 0
            || position.y() < 0
            || position.x() + width() > geometry.width()
            || position.y() + height() > geometry.height()))
    {
        position = QPoint(0, 0);
        SettingsManager::setWidgetPosition(_uid, position);
    }

    move(position);
}

void SmallDeviceView::startFadeIn()
{
    if (isHidden() && !_wasClosed && SettingsManager::shouldDisplayWidget(_uid))
    {
        _widgetAnimation->disconnect(this);
        _widgetAnimation->setEasingCurve(QEasingCurve::OutQuint);
        _widgetAnimation->setDirection(QAbstractAnimation::Forward);

        show();
        _widgetAnimation->start();
    }
}

void SmallDeviceView::onRestoreMainViewAction()
{
    emit doubleClicked();
}

void SmallDeviceView::onHistoryViewContextMenuRequested(const QPoint &pos)
{
    _contextMenu.exec(mapToGlobal(pos));
}

void SmallDeviceView::onHideWidget()
{
    _wasClosed = true;
    startFadeOut();
}

void SmallDeviceView::onHideAllWidgets()
{
    emit hideAllWidgets();
}

void SmallDeviceView::createContextMenuActions()
{
    _restoreMainViewAction = new QAction(QIcon(RESTORE_MAIN_VIEW_ICON), tr("Restaurer la fenêtre principale"), &_contextMenu);
    connect(_restoreMainViewAction, SIGNAL(triggered()), this, SLOT(onRestoreMainViewAction()));

    _hideWidgetAction = new QAction(QIcon(HIDE_WIDGET_ICON), tr("Fermer ce Widget"), &_contextMenu);
    connect(_hideWidgetAction, SIGNAL(triggered()), this, SLOT(onHideWidget()));

    _hideAllWidgetsAction = new QAction(QIcon(HIDE_ALL_WIDGETS_ICON), tr("Fermer tous les Widgets"), &_contextMenu);
    connect(_hideAllWidgetsAction, SIGNAL(triggered()), this, SLOT(onHideAllWidgets()));

    _contextMenu.addAction(_restoreMainViewAction);
    _contextMenu.addSeparator();
    _contextMenu.addAction(_hideWidgetAction);
    _contextMenu.addAction(_hideAllWidgetsAction);
}
