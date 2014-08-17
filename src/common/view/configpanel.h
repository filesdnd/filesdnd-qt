#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>

namespace Ui {
class ConfigPanel;
}

class ConfigPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPanel(QWidget *parent = 0);
    ~ConfigPanel();

private:
    Ui::ConfigPanel *ui;
};

#endif // CONFIGPANEL_H
