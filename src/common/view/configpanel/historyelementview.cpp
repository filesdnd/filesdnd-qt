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

#include "historyelementview.h"
#include "ui_historyelementview.h"
#include "helpers/settingsmanager.h"
#include "helpers/filehelper.h"
#include "appconfig.h"

#include <QFileInfo>
#include <QDebug>

HistoryElementView::HistoryElementView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryElementView)
{
    ui->setupUi(this);

    setProgressBarEnabled(false);
}

HistoryElementView::HistoryElementView(const QString &date, const QString &text, const QString &userName,
                                       qint64 size, HistoryElementType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryElementView)
{
    ui->setupUi(this);

    _type = type;
    _text = text;
    _date = date;
    _userName = userName;
    _size = size;

    refresh();
}

HistoryElementView::~HistoryElementView()
{
    delete ui;
}

QString HistoryElementView::getText() const
{
    return _text;
}

bool HistoryElementView::isDownloading() const
{
    return ui->progressBar->isVisible();
}

QIcon HistoryElementView::getIcon()
{
    QIcon icon;

    if (FileHelper::isSpreadsheet(_text)) {
        icon = QIcon(XLSX_ICON);
    } else if(FileHelper::isDoc(_text)) {
        icon = QIcon(DOC_ICON);
    } else if(FileHelper::isPdf(_text)) {
        icon = QIcon(PDF_ICON);
    } else if(FileHelper::isZip(_text)) {
        icon = QIcon(ZIP_ICON);
    } else if(FileHelper::isMusic(_text)) {
        icon = QIcon(MUSIC_ICON);
    } else if(FileHelper::isImage(_text)) {
        icon = QIcon(IMG_ICON);
    } else if(FileHelper::isMovie(_text)) {
        icon = QIcon(MOVIE_ICON);
    } else {
        icon = QIcon(FILE_ICON);
    }

    return icon;
}

void HistoryElementView::refresh()
{
    QPair<QString, QString> trucatedText = textForType(_text);

    switch (_type)
    {
    case HISTORY_FILE_TYPE:
        // File information
        ui->fileName->setText(trucatedText.first);
        ui->fileDate->setText(_date);
        ui->fileSize->setText(FileHelper::getSizeAsString(_size));
        ui->fileName->setWordWrap(false);
        ui->fileUserLabel->setText(trucateName(_userName));
        ui->stackedWidget->setCurrentIndex(0);

        // File icon
        ui->fileIcon->setPixmap(getIcon().pixmap(HISTORY_ICON_SIZE));
        break;

    case HISTORY_FOLDER_TYPE:
        ui->fileName->setText(trucatedText.first);
        ui->fileDate->setText(_date);
        ui->fileSize->setText(FileHelper::getSizeAsString(_size));
        ui->fileName->setWordWrap(false);
        ui->fileUserLabel->setText(trucateName(_userName));
        ui->stackedWidget->setCurrentIndex(0);
        break;

    case HISTORY_TEXT_TYPE:
        ui->text->setText(trucatedText.first);
        ui->textDate->setText(_date);
        ui->text->setWordWrap(true);
        ui->textUserLabel->setText(trucateName(_userName));
        ui->stackedWidget->setCurrentIndex(1);
        break;

    case HISTORY_URL_TYPE:
        ui->url->setText(trucatedText.first);
        ui->urlDate->setText(_date);
        ui->url->setWordWrap(true);
        ui->urlUserLabel->setText(trucateName(_userName));
        ui->stackedWidget->setCurrentIndex(2);
        break;
    }

    setToolTip(isFileFolder() ? _text : trucatedText.second);
    setProgressBarEnabled(false);
}

bool HistoryElementView::isFileFolder()
{
    return (_type == HISTORY_FOLDER_TYPE || _type == HISTORY_FILE_TYPE);
}

QString HistoryElementView::trucateName(QString fullDataName)
{
    int cutAt = 10;
    QString endingString = ".";
    QString trucatedName = fullDataName.left(cutAt - endingString.size());

    if (fullDataName.size() > trucatedName.size())
        trucatedName.append(endingString);

    return trucatedName;
}

QPair<QString, QString> HistoryElementView::textForType(QString text)
{
    int cutAt = 25;
    QString endingString = " ...";
    int maxTextSize = isFileFolder() ? (cutAt - endingString.size()) : 100;
    QPair<QString, QString> trucatedText;
    QString leftText = text.left(maxTextSize);

    if (text.size() > maxTextSize) {
        leftText.append(endingString);
    }

    trucatedText.first = leftText;
    trucatedText.second = leftText;


    if (!isFileFolder())
    {
        int size = trucatedText.first.size();
        int i = 0;

        while (size > cutAt)
        {
            trucatedText.first.insert(++i * cutAt, " ");
            size -= cutAt;
        }
    }

    return trucatedText;
}

HistoryElementType HistoryElementView::getType() const
{
    return _type;
}

void HistoryElementView::setProgress(unsigned progress)
{
    if (isFileFolder())
    {
        ui->fileSize->setText(FileHelper::getFileSize(_text));
        ui->progressBar->setValue(progress);
        ui->fileProgressWidget->setVisible(true);
    }
}

void HistoryElementView::setProgressBarEnabled(bool enabled)
{
    ui->progressBar->setValue(0);
    ui->fileProgressWidget->setVisible(enabled);
}

QString HistoryElementView::getFileSize() const
{
    return ui->fileSize->text();
}

void HistoryElementView::on_cancelButton_clicked()
{
    emit cancelIncomingTransfert();
}
