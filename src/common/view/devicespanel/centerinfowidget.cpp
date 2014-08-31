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

#include "centerinfowidget.h"
#include "ui_centerinfowidget.h"
#include "appconfig.h"

#include <QDebug>

QMovie *CenterInfoWidget::AnimatedLogo = 0;
QMovie *CenterInfoWidget::AnimatedBonjour = 0;

CenterInfoWidget::CenterInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterInfoWidget)
{
    ui->setupUi(this);

    if (AnimatedLogo == 0) {
        AnimatedLogo = new QMovie(ANIMATED_WAITING_LOGO);
    }

    if (AnimatedBonjour == 0) {
        AnimatedBonjour = new QMovie(ANIMATED_BONJOUR_LOGO);
    }
}

CenterInfoWidget::~CenterInfoWidget()
{
    AnimatedLogo->setPaused(true);
    AnimatedBonjour->setPaused(true);
    delete ui;
}

void CenterInfoWidget::setNoDeviceMode()
{
    ui->icon->setMovie(AnimatedLogo);
    AnimatedLogo->setPaused(false);
    AnimatedBonjour->setPaused(true);
    ui->infoLabel->setText(tr("Nous cherchons vos périphériques"));
}

void CenterInfoWidget::setBonjourErrorMode(const QString &message)
{
    ui->icon->setMovie(AnimatedBonjour);
    //ui->icon->setPixmap(QPixmap(BONJOUR_LOGO));
    AnimatedBonjour->setPaused(false);
    AnimatedLogo->setPaused(true);
    ui->infoLabel->setText(message);
}
