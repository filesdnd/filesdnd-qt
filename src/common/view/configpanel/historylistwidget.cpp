#include "historylistwidget.h"
#include "appconfig.h"

#include <QFile>
#include <QDebug>
#include <QAbstractItemView>

HistoryListWidget::HistoryListWidget(QWidget *parent) :
    QListWidget(parent)
{
    // Stylesheet
    QFile stylesheetFile(HISTORY_LIST_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }
}

HistoryListWidget::~HistoryListWidget()
{
}

QSize HistoryListWidget::contentSize() const
{
    return QListWidget::contentsSize();
}
