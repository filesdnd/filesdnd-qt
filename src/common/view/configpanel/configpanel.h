#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>

#include "transparentscrollbar.h"
#include "historyelement.h"
#include "view.h"

namespace Ui {
class ConfigPanel;
}

class ConfigPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPanel(QWidget *parent = 0);
    ~ConfigPanel();

    void setMainView(View *view);
    void openDownloadFolder();

    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_refreshButton_clicked();
    void updateRefreshFrame(int frame);

public slots:
    /**
    * SLOT : on history changed
    * @param history New history value
    */
    void onHistoryChanged(const QList<HistoryElement> &history);

private:
    Ui::ConfigPanel *ui;
    QMovie _refreshMovie;
    View *_view;
    TransparentScrollBar *_historyScrollBar;

    void createHistoryListWidget();
};

#endif // CONFIGPANEL_H
