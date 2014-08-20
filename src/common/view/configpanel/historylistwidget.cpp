#include "historylistwidget.h"

HistoryListWidget::HistoryListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

HistoryListWidget::~HistoryListWidget()
{
}

QSize HistoryListWidget::contentSize() const
{
    return QListWidget::contentsSize();
}
