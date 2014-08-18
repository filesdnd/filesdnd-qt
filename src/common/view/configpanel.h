#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>

namespace Ui {
class ConfigPanel;
}

class ConfigPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPanel(QWidget *parent = 0);
    ~ConfigPanel();

private slots:
    void on_refreshButton_clicked();
    void updateRefreshFrame(int frame);

private:
    Ui::ConfigPanel *ui;
    QMovie _refreshMovie;
};

#endif // CONFIGPANEL_H
