#include "transparentscrollbar.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QDebug>
#include <QPainter>

#include "transparentscrollbutton.h"

TransparentScrollBar::TransparentScrollBar(QAbstractItemView *parentView)
    : QWidget(parentView, Qt::FramelessWindowHint)
    , m_view(parentView)
{
    Q_ASSERT( parentView );

    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_scrollBtn = new TransparentScrollButton(parentView);
    m_scrollBtn->setFixedSize(15, 75);
    resize(10, 0);

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
    switch (event->type())
    {
    case QEvent::Enter:
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
    QScrollBar *sb = m_view->verticalScrollBar();
    const int val = sb->value();
    const int max = sb->maximum();
    const int x = pos().x() + (width() - m_scrollBtn->width()) / 2;

    if ( max == 0 )
    {
        m_scrollBtn->move(x, pos().y());
        return ;
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

void TransparentScrollBar::resizeEvent( QResizeEvent * event )
{
    Q_UNUSED(event);
    updatePos();
}
