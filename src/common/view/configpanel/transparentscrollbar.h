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

#ifndef TRANSPARENTSCROLLBAR_H
#define TRANSPARENTSCROLLBAR_H

#include <QWidget>
#include <QAbstractItemView>
#include <QPointer>

#include "transparentscrollbutton.h"
#include "historylistwidget.h"

/**
 * @class TransparentScrollBar
 *
 * This scrollbar is over a list widget. It is animated on mouse over
 */
class TransparentScrollBar : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    TransparentScrollBar(HistoryListWidget *parentView);
    /// Destructor
    ~TransparentScrollBar();

private slots:
    /// Update the position of the animated scroll depending on the list state
    void updatePos();
    /// On scroll button moved, update the list position
    void onScrollBtnMoved(QPoint);

private:
    /// See QWidget::eventFilter
    bool eventFilter(QObject *obj, QEvent *event);
    /// See QWidget::onResize
    void onResize(QResizeEvent *e);
    /// See QWidget::paintEvent
    void paintEvent(QPaintEvent *event);
    /// See QWidget::resizeEvent
    void resizeEvent(QResizeEvent *event);
    /// See QWidget::mousePressEvent
    void mousePressEvent(QMouseEvent *event);

    /**
     * Move the button to the a global position
     *
     * @param moveTo Global position
     */
    void moveToGlobalPos(QPoint moveTo);

    /// The list widget
    QPointer<HistoryListWidget> m_view;
    /// The scroll button (visible one)
    QPointer<TransparentScrollButton> m_scrollBtn;
    /// The width of the scroll button
    int _scrollButtonWidth;
    /// The minimum height of the scroll button (the height is dynamic, depending on list content)
    int _minScrollButtonHeight;
};

#endif // TRANSPARENTSCROLLBAR_H
