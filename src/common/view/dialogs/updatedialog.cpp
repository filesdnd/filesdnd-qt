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

#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "helpers/filehelper.h"
#include "helpers/settingsmanager.h"
#include "appconfig.h"

#include <QDebug>

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Nouvelle version disponible"));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::updateAndShow(const QString &version, const QString &note)
{
    QPoint globalParentPos = parentWidget()->mapToGlobal(parentWidget()->pos());

    _version = version;
    ui->updateLabel->setText(tr("La version ")
                             + version
                             + tr(" est disponible (version actuelle : ")
                             + QString(CURRENT_VERSION)
                             + tr(")\nVoulez vous mettre l'application à jour ?"));
    ui->releaseNote->setHtml(note);

    setGeometry(globalParentPos.x() + (parentWidget()->width() / 2) - (width() / 2),
                globalParentPos.y() + (parentWidget()->height() / 2) - (height() / 2),
                width(),
                height());
    show();
}

void UpdateDialog::on_updateButton_clicked()
{
    QString website;

    if (QLocale::system().language() == QLocale::French)
        website = WEB_SITE;
    else
        website = EN_WEB_SITE;

    FileHelper::openURL(QString(website));
    close();
}

void UpdateDialog::on_laterButton_clicked()
{
    close();
}

void UpdateDialog::on_ignoreButton_clicked()
{
    SettingsManager::setIgnoredVersion(_version);
    close();
}
