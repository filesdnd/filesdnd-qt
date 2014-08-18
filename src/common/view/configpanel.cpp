#include "configpanel.h"
#include "ui_configpanel.h"
#include "appconfig.h"

#include <QFile>
#include <QDebug>

ConfigPanel::ConfigPanel(QWidget *parent) :
    QWidget(parent),
    _refreshMovie(ANIMATED_REFRESH_ICON),
    ui(new Ui::ConfigPanel)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(CONFIG_PANEL_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }

    connect(&_refreshMovie, SIGNAL(frameChanged(int)), this, SLOT(updateRefreshFrame(int)));
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}

void ConfigPanel::on_refreshButton_clicked()
{
    if (_refreshMovie.state() == QMovie::Running) {
        _refreshMovie.setPaused(true);
    } else if (_refreshMovie.state() == QMovie::Paused) {
        _refreshMovie.setPaused(false);
    } else {
        _refreshMovie.start();
    }
}

void ConfigPanel::updateRefreshFrame(int frame)
{
    Q_UNUSED(frame);
    ui->refreshButton->setIcon(QIcon(_refreshMovie.currentPixmap()));
}
