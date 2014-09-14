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

#include "aboutwidget.h"
#include "ui_aboutwidget.h"
#include "appconfig.h"
#include "filehelper.h"

#include <QDebug>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);

    // Set version
    QString txt = ui->aboutLabel->text().replace("#VERSION#", CURRENT_VERSION);
    ui->aboutLabel->setText(txt);

    // Stylesheet
    setStyleSheet(FileHelper::loadFileContent(ABOUT_CSS));
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

void AboutWidget::showAbout()
{
    show();
}

void AboutWidget::on_playLinkButton_clicked()
{
    FileHelper::openURL(PLAY_LINK);
}

void AboutWidget::on_facebookLinkButton_clicked()
{
    FileHelper::openURL(FACEBOOK_LINK);
}

void AboutWidget::on_googleLinkButton_clicked()
{
    FileHelper::openURL(GOOGLE_LINK);
}

void AboutWidget::on_twitterLinkButton_clicked()
{
    FileHelper::openURL(TWITTER_LINK);
}

void AboutWidget::on_githubLinkButton_clicked()
{
    FileHelper::openURL(GITHUB_LINK);
}
