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

#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <QDebug>
#include <QEasingCurve>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

/*!
Description
        SlidingStackedWidget is a class that is derived from QtStackedWidget
        and allows smooth side shifting of widgets, in addition
        to the original hard switching from one to another as offered by
        QStackedWidget itself.
*/

class SlidingStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    //! This enumeration is used to define the animation direction
    enum t_direction {
            LEFT2RIGHT,
            RIGHT2LEFT,
            TOP2BOTTOM,
            BOTTOM2TOP,
            AUTOMATIC
    };

    //! The Constructor and Destructor
    SlidingStackedWidget(QWidget *parent);
    ~SlidingStackedWidget(void);

public slots:
    //! Some basic settings API
    void setSpeed(int speed);   //animation duration in milliseconds
    void setAnimation(enum QEasingCurve::Type animationtype); //check out the QEasingCurve documentation for different styles
    void setVerticalMode(bool vertical=true);
    void setWrap(bool wrap);    //wrapping is related to slideInNext/Prev;it defines the behaviour when reaching last/first page

    //! The Animation / Page Change API
    void slideInNext();
    void slideInPrev();
    void slideInIdx(int idx, enum t_direction direction=AUTOMATIC);

signals:
    //! this is used for internal purposes in the class engine
    void animationFinished(void);

protected slots:
    //! this is used for internal purposes in the class engine
    void animationDoneSlot(void);

protected:
    //! this is used for internal purposes in the class engine
    void slideInWgt(QWidget * widget, enum t_direction direction=AUTOMATIC);

    QWidget *m_mainwindow;

    int m_speed;
    enum QEasingCurve::Type m_animationtype;
    bool m_vertical;
    int m_now;
    int m_next;
    bool m_wrap;
    QPoint m_pnow;
    bool m_active;
    QParallelAnimationGroup *m_animationGroup;
    QPropertyAnimation *m_animnow;
    QPropertyAnimation *m_animnext;

    QList<QWidget*> blockedPageList;

};














#endif // SLIDINGSTACKEDWIDGET_H
