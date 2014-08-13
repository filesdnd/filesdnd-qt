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

#ifndef HISTORYGRIPBUTTON_H
#define HISTORYGRIPBUTTON_H

#include <QLabel>
#include <QMouseEvent>

/**
  * @enum HistoryButtonState
  *
  * State of the history button
  */
enum HistoryButtonState
{
    LEFT,
    RIGHT
};

/**
  * @class HistoryGripButton
  *
  * Arrow button made to slide the history
  */
class HistoryGripButton : public QLabel
{
    Q_OBJECT
public:
    /**
      * Constructor
      */
    explicit HistoryGripButton(QWidget *parent = 0);

    /**
      * Move event to handle the click on the label as a button
      */
    void mousePressEvent(QMouseEvent *ev);

    /**
      * Tells if the button is in the left state
      *
      * @return True if the button is in the left state, false otherwise
      */
    bool leftState();
    /**
      * Put the button in the left state
      */
    void leftArrow();
    /**
      * Put the button in the right state
      */
    void rightArrow();
    
signals:
    /**
      * Emit that the label was clicked
      */
    void clicked();
    
public slots:

private:
    /// State of the button (LEFT or RIGHT arrow)
    HistoryButtonState _state;
    
};

#endif // HISTORYGRIPBUTTON_H
