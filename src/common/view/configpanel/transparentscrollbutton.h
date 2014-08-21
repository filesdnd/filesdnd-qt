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

#ifndef TRANSPARENTSCROLLBUTTON_H
#define TRANSPARENTSCROLLBUTTON_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

/**
 * @class TransparentScrollButton
 *
 * This is the button of the animated scrollbar
 * @see TransparentScrollBar
 */
class TransparentScrollButton : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    TransparentScrollButton(QWidget *parent);
    /// Destructor
    ~TransparentScrollButton();

    /// See QWidget::mouseMoveEvent
    void mouseMoveEvent(QMouseEvent *);
    /// See QWidget::mousePressEvent
    void mousePressEvent(QMouseEvent *);

    /// Fade the button in
    void fadeIn();
    /// Fade the button out
    void fadeOut();

signals:
    /// Signal that the button has moved
    void moved(QPoint);

private:
    /// Opacity effect
    QGraphicsOpacityEffect *_opacityEffect;
    /// Fade in/out animation
    QPropertyAnimation  *_fadeAnimation;
    /// Minimum opacity
    float _minOpacity;
    /// Maximum opacity
    float _maxOpacity;

    /// See QWidget::paintEvent
    void paintEvent(QPaintEvent *event);
};
#endif // TRANSPARENTSCROLLBUTTON_H
