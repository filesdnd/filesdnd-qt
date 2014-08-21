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

#include "transparentscrollbutton.h"

#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QColor>
#include <QDebug>

TransparentScrollButton::TransparentScrollButton( QWidget *parent ) :
    QWidget(parent),
    _minOpacity(0),
    _maxOpacity(0.5)
{
    _opacityEffect = new QGraphicsOpacityEffect(this);
    _opacityEffect->setOpacity(_minOpacity);
    setGraphicsEffect(_opacityEffect);

    _fadeAnimation = new QPropertyAnimation(_opacityEffect, "opacity");
}

TransparentScrollButton::~TransparentScrollButton()
{
    delete _opacityEffect;
}

void TransparentScrollButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void TransparentScrollButton::mouseMoveEvent(QMouseEvent *event)
{
    emit moved(event->globalPos());
}

void TransparentScrollButton::fadeIn()
{
    _fadeAnimation->stop();
    _fadeAnimation->setDuration(250);
    _fadeAnimation->setStartValue(_opacityEffect->opacity());
    _fadeAnimation->setEndValue(_maxOpacity);
    _fadeAnimation->start();
}

void TransparentScrollButton::fadeOut()
{
    _fadeAnimation->stop();
    _fadeAnimation->setDuration(600);
    _fadeAnimation->setStartValue(_opacityEffect->opacity());
    _fadeAnimation->setEndValue(_minOpacity);
    _fadeAnimation->start();
}

void TransparentScrollButton::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    // Draw any scroll button
    QPainter p(this);
    QRect rc(5, 5, rect().width() - 6, rect().height() - 6);

    p.fillRect(rc, QColor(100, 100, 100, 150));
}
