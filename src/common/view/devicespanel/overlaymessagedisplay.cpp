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

#include "overlaymessagedisplay.h"
#include "ui_overlaymessagedisplay.h"
#include "config/appconfig.h"

#include <QPainter>

OverlayMessageDisplay::OverlayMessageDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverlayMessageDisplay),
    _timer(this)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoBackground);

    setWindowOpacity(0.6);
    setVisible(false);

    connect(&_timer, SIGNAL(timeout()),
            this, SLOT(onTimerOut()));

    _timer.setSingleShot(true);

    initAnimations();
}

OverlayMessageDisplay::~OverlayMessageDisplay()
{
    close();
    clearAnimations();

    delete ui;
}

void OverlayMessageDisplay::onTimerOut()
{
    hideAnimation();
}

void OverlayMessageDisplay::onFadeOut()
{
    setVisible(false);
}

void OverlayMessageDisplay::setText(const QString &text)
{
    ui->message->setText(text);
    showAnimation();
    _timer.start(OVERLAY_TIMEOUT);
}

void OverlayMessageDisplay::refreshGeometry()
{
    int newWidth = parentWidget()->width() * 0.9;

    setGeometry(parentWidget()->width() / 2 - newWidth / 2 - 15,
                parentWidget()->y(),
                newWidth,
                height());
}

void OverlayMessageDisplay::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    refreshGeometry();
}

void OverlayMessageDisplay::clearAnimations()
{
    delete _opacityEffect;
    delete _opacityAnimation;
}

void OverlayMessageDisplay::initAnimations()
{
    _opacityAnimation = new QPropertyAnimation(this);
    _opacityAnimation->setPropertyName("opacity");
    _opacityAnimation->setDuration(OPACITY_ANIMATION_TIMER);
    _opacityAnimation->setEasingCurve(QEasingCurve::OutQuint);
    _opacityAnimation->setDirection(QAbstractAnimation::Forward);

    _opacityEffect = new QGraphicsOpacityEffect(this);
    _opacityAnimation->setTargetObject(_opacityEffect);
     setGraphicsEffect(_opacityEffect);
}

void OverlayMessageDisplay::hideAnimation()
{
    connect(_opacityAnimation, SIGNAL(finished()), this, SLOT(onFadeOut()));
    _opacityEffect->setOpacity(1);
    _opacityAnimation->setStartValue(_opacityEffect->opacity());
    _opacityAnimation->setEndValue(0);

    _opacityAnimation->start();
}

void OverlayMessageDisplay::showAnimation()
{
    setVisible(true);

    _opacityAnimation->disconnect(this);
    _opacityEffect->setOpacity(0);
    _opacityAnimation->setStartValue(_opacityEffect->opacity());
    _opacityAnimation->setEndValue(1);

    _opacityAnimation->start();
}
