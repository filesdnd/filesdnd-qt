#ifndef MACWINDOW_H
#define MACWINDOW_H

#include <QMainWindow>

#include "view.h"

namespace Ui {
class MacWindow;
}

class MacWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MacWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view, QWidget *parent = 0);
    ~MacWindow();

private:
    Ui::MacWindow *ui;
    QApplication *_app;
    View *_view;
};

#endif // MACWINDOW_H
