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

HowToPanel::HowToPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HowToPanel)
{
    ui->setupUi(this);

    // Configuration
    ui->stackedWidget->setWrap(true);
    ui->stackedWidget->setSpeed(SLIDING_WIDGET_ANIMATION_SPEED);
    ui->stackedWidget->setAnimation(QEasingCurve::OutCubic);
    ui->stackedWidget->setCurrentIndex(STEP1);

    // Center widget info
    ui->logo->setNoDeviceMode();
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
