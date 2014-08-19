#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>

#include "transparentscrollbar.h"

namespace Ui {
class ConfigPanel;
}

class ConfigPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPanel(QWidget *parent = 0);
    ~ConfigPanel();
    void openDownloadFolder();

private slots:
    void on_refreshButton_clicked();
    void updateRefreshFrame(int frame);

    void mousePressEvent(QMouseEvent *event);

private:
    Ui::ConfigPanel *ui;
    QMovie _refreshMovie;
    TransparentScrollBar *_historyScrollBar;

    void createHistoryListWidget();
};

#endif // CONFIGPANEL_H
