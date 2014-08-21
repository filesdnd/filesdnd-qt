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

#include "configpanel.h"
#include "ui_configpanel.h"
#include "appconfig.h"
#include "filehelper.h"
#include "settingsmanager.h"
#include "historyelementview.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QListWidget>

ConfigPanel::ConfigPanel(QWidget *parent) :
    QWidget(parent),
    _refreshMovie(ANIMATED_REFRESH_ICON),
    _view(0),
    ui(new Ui::ConfigPanel)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(CONFIG_PANEL_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }

    // Manage history list widget
    createHistoryListWidget();

    // Connect events
    connect(&_refreshMovie, SIGNAL(frameChanged(int)), this, SLOT(updateRefreshFrame(int)));
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
    delete _historyScrollBar;
}

void ConfigPanel::createHistoryListWidget() {
    _historyScrollBar = new TransparentScrollBar(ui->historyListWidget);

    for (int i = 0; i < 10; ++i) {
        HistoryElementView *historyViewElement = new HistoryElementView(QDateTime::currentDateTime().toString("dd/MM/yyyy"),
                                                                        "long-file-name-example-long.txt",
                                                                        HistoryElementType(i % 3));
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,historyViewElement->sizeHint().height() - 15));
        ui->historyListWidget->addItem(item);
        ui->historyListWidget->setItemWidget(item, historyViewElement);
    }
}

void ConfigPanel::mousePressEvent(QMouseEvent *event) {
    if (ui->downloadFolderWidget->geometry().contains(event->pos())) {
        openDownloadFolder();
    } else if (ui->settingsWidget->geometry().contains(event->pos())) {
        qDebug() << "Settings";
    } else if (ui->aboutWidget->geometry().contains(event->pos())) {
        qDebug() << "About";
    }

    QWidget::mousePressEvent(event);
}

void ConfigPanel::openDownloadFolder() {
    QDir dlDir;

    dlDir.mkpath(SettingsManager::getDestinationFolder());
    FileHelper::openURL("file:///" + QDir::toNativeSeparators(SettingsManager::getDestinationFolder()));
}

void ConfigPanel::setMainView(View *view)
{
    _view = view;
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

void ConfigPanel::onHistoryChanged(const QList<HistoryElement> &history)
{
    //clearHistory();
    foreach(HistoryElement elt, history)
    {
        HistoryElementView *historyViewElement = new HistoryElementView(elt.getDateTime("dd/MM - hh:mm"), elt.getText(), elt.getType());
        QListWidgetItem *item = new QListWidgetItem();
        connect(historyViewElement, SIGNAL(cancelIncomingTransfert()),
                _view, SLOT(onCancelIncomingTransfert()));
        item->setSizeHint(QSize(0,historyViewElement->sizeHint().height()));
        ui->historyListWidget->addItem(item);
        ui->historyListWidget->setItemWidget(item, historyViewElement);
    }
}
