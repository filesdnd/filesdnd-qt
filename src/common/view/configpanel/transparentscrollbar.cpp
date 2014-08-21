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

#include "transparentscrollbar.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QDebug>
#include <QPainter>

#include "transparentscrollbutton.h"

TransparentScrollBar::TransparentScrollBar(HistoryListWidget *parentView)
    : QWidget(parentView, Qt::FramelessWindowHint),
    m_view(parentView),
    _scrollButtonWidth(13),
    _minScrollButtonHeight(70)
{
    Q_ASSERT(parentView);

    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_scrollBtn = new TransparentScrollButton(parentView);
    m_scrollBtn->setFixedSize(_scrollButtonWidth, _minScrollButtonHeight);
    resize(_scrollButtonWidth, 0);

    m_view->installEventFilter(this);

    connect(m_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updatePos()));
    connect(m_scrollBtn, SIGNAL(moved(QPoint)), this, SLOT(onScrollBtnMoved(QPoint)));
}

TransparentScrollBar::~TransparentScrollBar()
{
    removeEventFilter(m_view);
}


void TransparentScrollBar::mousePressEvent(QMouseEvent *event)
{
    moveToGlobalPos(event->globalPos());
}

void TransparentScrollBar::moveToGlobalPos(QPoint moveTo)
{
    QScrollBar *sb = m_view->verticalScrollBar();
    float max = sb->maximum();
    QPoint viewGlobalPos = m_view->parentWidget()->mapToGlobal(m_view->pos());

    float minPx = viewGlobalPos.y();
    float maxPx = viewGlobalPos.y() + m_view->height();

    float ratio = (moveTo.y() - minPx) / (maxPx - minPx);
    float value = ratio * max;

    sb->setValue(value);
}

bool TransparentScrollBar::eventFilter(QObject *obj, QEvent *event)
{
    int viewportHeight;
    int contentHeight;

    switch (event->type())
    {
    case QEvent::Enter:
        viewportHeight = m_view->height();
        contentHeight = m_view->contentSize().height();

        if (contentHeight > viewportHeight)
            m_scrollBtn->fadeIn();
        break;

    case QEvent::Leave:
        m_scrollBtn->fadeOut();
        break;

    case QEvent::Resize:
        onResize( static_cast<QResizeEvent *>(event));
        break;
    }

    return QWidget::eventFilter( obj, event );
}

void TransparentScrollBar::onResize(QResizeEvent *e)
{
    const int x = e->size().width() - width();
    const int y = 0;
    const int w = width();
    const int h = e->size().height();

    move(x, y);
    resize(w, h);

    updatePos();
}

void TransparentScrollBar::onScrollBtnMoved(QPoint point) {

    moveToGlobalPos(point);
}

void TransparentScrollBar::updatePos()
{
    // Set scroll button height
    float viewportHeight = m_view->height();
    float contentHeight = m_view->contentSize().height();
    float ratio = viewportHeight / contentHeight;
    int size = qMax((int)(ratio * viewportHeight), _minScrollButtonHeight);
    m_scrollBtn->setFixedSize(_scrollButtonWidth, size);

    // Set scroll button position
    QScrollBar *sb = m_view->verticalScrollBar();
    const int val = sb->value();
    const int max = sb->maximum();
    const int x = pos().x() + (width() - m_scrollBtn->width()) / 2;

    if (max == 0)
    {
        m_scrollBtn->move(x, pos().y());
        return;
    }

    const int maxY = height() - m_scrollBtn->height();
    const int y = (maxY * val) / max;

    m_scrollBtn->move(x, y);
}

void TransparentScrollBar::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QRect rc(0, 0, rect().width() - 1, rect().height() - 1);

    // Draw any scroll background
    p.fillRect(rc, QColor(255, 255, 255, 0));
}

void TransparentScrollBar::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event);
    updatePos();
}
