#ifndef TRANSPARENTSCROLLBUTTON_H
#define TRANSPARENTSCROLLBUTTON_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class TransparentScrollButton : public QWidget
{
    Q_OBJECT

public:
    TransparentScrollButton(QWidget *parent);
    ~TransparentScrollButton();

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void fadeIn();
    void fadeOut();

signals:
    void moved(QPoint);

private:
    QGraphicsOpacityEffect *_opacityEffect;
    QPropertyAnimation  *_fadeAnimation;
    float _minOpacity;
    float _maxOpacity;

    void paintEvent(QPaintEvent *event);
};
#endif // TRANSPARENTSCROLLBUTTON_H
