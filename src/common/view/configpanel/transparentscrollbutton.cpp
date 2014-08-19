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
    _fadeAnimation->setDuration(350);
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
    _fadeAnimation->setStartValue(_opacityEffect->opacity());
    _fadeAnimation->setEndValue(_maxOpacity);
    _fadeAnimation->start();
}

void TransparentScrollButton::fadeOut()
{
    _fadeAnimation->stop();
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
