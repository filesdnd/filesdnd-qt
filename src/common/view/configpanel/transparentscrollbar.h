#ifndef TRANSPARENTSCROLLBAR_H
#define TRANSPARENTSCROLLBAR_H

#include <QWidget>
#include <QAbstractItemView>
#include <QPointer>

#include "transparentscrollbutton.h"

class TransparentScrollBar : public QWidget
{
    Q_OBJECT

public:
    TransparentScrollBar(QAbstractItemView *parentView);
    ~TransparentScrollBar();

private slots:
    void updatePos();
    void onScrollBtnMoved(QPoint);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void onResize(QResizeEvent *e);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void moveToGlobalPos(QPoint moveTo);

    QPointer<QAbstractItemView> m_view;
    QPointer<TransparentScrollButton> m_scrollBtn;
    QPoint m_pressed;
};

#endif // TRANSPARENTSCROLLBAR_H
