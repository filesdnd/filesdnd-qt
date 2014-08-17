#include "configpanel.h"
#include "ui_configpanel.h"

#include <QFile>

ConfigPanel::ConfigPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigPanel)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/css/common/css/configpanel.css");
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}
