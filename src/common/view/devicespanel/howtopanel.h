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

#ifndef HOWTOPANEL_H
#define HOWTOPANEL_H

#include <QWidget>

namespace Ui {
class HowToPanel;
}

enum StepPages {
    STEP1,
    STEP2
};

class HowToPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HowToPanel(QWidget *parent = 0);
    ~HowToPanel();

signals:
    void howToOver();

private slots:
    void on_step1Next_clicked();
    void on_step2Next_clicked();
    void on_step2Pev_clicked();

private:
    Ui::HowToPanel *ui;
};

#endif // HOWTOPANEL_H
