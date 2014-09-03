#include "macwindow.h"
#include "ui_macwindow.h"

MacWindow::MacWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view, QWidget *parent) :
    QMainWindow(parent),
    _app(app),
    _view(view),
    ui(new Ui::MacWindow)
{
    ui->setupUi(this);

    connect(view, SIGNAL(showWindow()), this, SLOT(show()));
    connect(view, SIGNAL(hideWindow()), this, SLOT(hide()));

    setWindowTitle("Files Drag & Drop");
    setGeometry(x, y, width, height);
    setStyleSheet(FileHelper::loadFileContent(MAC_APP_CSS));

    ui->centralwidget->layout()->addWidget(view);
}

MacWindow::~MacWindow()
{
    delete ui;
}
