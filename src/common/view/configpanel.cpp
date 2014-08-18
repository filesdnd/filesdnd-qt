#include "configpanel.h"
#include "ui_configpanel.h"
#include "appconfig.h"

#include <QFile>
#include <QDebug>

ConfigPanel::ConfigPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigPanel)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(CONFIG_PANEL_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}
