/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#include "howtopanel.h"
#include "ui_howtopanel.h"
#include "config/appconfig.h"
#include "filehelper.h"

#define HISTORY_PIXMAP_FR ":/howto/images/other/help-history-fr.png"
#define MENU_PIXMAP_FR ":/howto/images/other/help-nav-fr.png"
#define NEXT_STEP_PIXMAP_FR ":/howto/images/other/next-step-fr.png"
#define DISCOVERY_PIXMAP_FR ":/howto/images/other/help-search-fr.png"
#define DROP_EXAMPLE_PIXMAP_FR ":/howto/images/other/drop-device-fr.png"
#define GO_PIXMAP_FR ":/howto/images/other/end-step-fr.png"

#define HISTORY_PIXMAP_EN ":/howto/images/other/help-history-en.png"
#define MENU_PIXMAP_EN ":/howto/images/other/help-nav-en.png"
#define NEXT_STEP_PIXMAP_EN ":/howto/images/other/next-step-en.png"
#define DISCOVERY_PIXMAP_EN ":/howto/images/other/help-search-en.png"
#define DROP_EXAMPLE_PIXMAP_EN ":/howto/images/other/drop-device-en.png"
#define GO_PIXMAP_EN ":/howto/images/other/end-step-en.png"

HowToPanel::HowToPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HowToPanel)
{
    ui->setupUi(this);

    // Stylesheet
    setStyleSheet(FileHelper::loadFileContent(HOWTO_PANEL_CSS));

    // Configuration
    ui->stackedWidget->setWrap(true);
    ui->stackedWidget->setSpeed(SLIDING_WIDGET_ANIMATION_SPEED);
    ui->stackedWidget->setAnimation(QEasingCurve::OutCubic);
    ui->stackedWidget->setCurrentIndex(STEP1);

    // Center widget info
    ui->logo->setNoDeviceMode();

    // Languages
    if (QLocale::system().language() == QLocale::French) {
        ui->historyPixmap->setPixmap(QPixmap(HISTORY_PIXMAP_FR));
        ui->menuPixmap->setPixmap(QPixmap(MENU_PIXMAP_FR));
        ui->nextStepPixmap->setPixmap(QPixmap(NEXT_STEP_PIXMAP_FR));
        ui->discoveryPixmap->setPixmap(QPixmap(DISCOVERY_PIXMAP_FR));
        ui->dropExamplePixmap->setPixmap(QPixmap(DROP_EXAMPLE_PIXMAP_FR));
        ui->goPixmap->setPixmap(QPixmap(GO_PIXMAP_FR));
    } else {
        ui->historyPixmap->setPixmap(QPixmap(HISTORY_PIXMAP_EN));
        ui->menuPixmap->setPixmap(QPixmap(MENU_PIXMAP_EN));
        ui->nextStepPixmap->setPixmap(QPixmap(NEXT_STEP_PIXMAP_EN));
        ui->discoveryPixmap->setPixmap(QPixmap(DISCOVERY_PIXMAP_EN));
        ui->dropExamplePixmap->setPixmap(QPixmap(DROP_EXAMPLE_PIXMAP_EN));
        ui->goPixmap->setPixmap(QPixmap(GO_PIXMAP_EN));
    }
}

HowToPanel::~HowToPanel()
{
    delete ui;
}

void HowToPanel::on_step1Next_clicked()
{
    ui->stackedWidget->slideInIdx(STEP2);
}

void HowToPanel::on_step2Next_clicked()
{
    emit howToOver();
}

void HowToPanel::on_step2Pev_clicked()
{
    ui->stackedWidget->slideInIdx(STEP1);
}
