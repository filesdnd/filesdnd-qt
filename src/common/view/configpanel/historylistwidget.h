#ifndef HISTORYLISTWIDGET_H
#define HISTORYLISTWIDGET_H

#include <QListWidget>

class HistoryListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit HistoryListWidget(QWidget *parent = 0);
    ~HistoryListWidget();

    QSize contentSize() const;

signals:

public slots:

};

#endif // HISTORYLISTWIDGET_H
