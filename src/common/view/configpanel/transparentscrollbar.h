#ifndef TRANSPARENTSCROLLBAR_H
#define TRANSPARENTSCROLLBAR_H

#include <QWidget>
#include <QAbstractItemView>
#include <QPointer>

#include "transparentscrollbutton.h"
#include "historylistwidget.h"

class TransparentScrollBar : public QWidget
{
    Q_OBJECT

public:
    TransparentScrollBar(HistoryListWidget *parentView);
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

    QPointer<HistoryListWidget> m_view;
    QPointer<TransparentScrollButton> m_scrollBtn;
    QPoint m_pressed;
    int _scrollButtonWidth;
    int _minScrollButtonHeight;
};

#endif // TRANSPARENTSCROLLBAR_H
