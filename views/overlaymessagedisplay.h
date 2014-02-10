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

#ifndef OVERLAYMESSAGEDISPLAY_H
#define OVERLAYMESSAGEDISPLAY_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>

namespace Ui {
class OverlayMessageDisplay;
}

/**
 * @class OverlayMessageDisplay
 *
 * Allow to display messages in the top of the devices view
 */
class OverlayMessageDisplay : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * Constructor
     */
    explicit OverlayMessageDisplay(QWidget *parent = 0);
    /**
      * Destructor
      */
    ~OverlayMessageDisplay();
    /**
     * @overload QWidget::paintEvent
     */
    void paintEvent(QPaintEvent *event);
    /**
     * @brief Change the text displayed
     *
     * @param text The new text
     */
    void setText(const QString &text);
    /**
     * @brief Refresh the geometry of the message to fill the window
     */
    void refreshGeometry();
    /**
     * Delete animations
     */
    void clearAnimations();
    /**
     * Initialize animations
     */
    void initAnimations();
    /**
     * Configure and start the hide animation of the overlay message
     */
    void hideAnimation();
    /**
     *  Configure and start the show animation of the overlay message
     */
    void showAnimation();

public slots:
    /**
     * On timer out, hide the message
     */
    void onTimerOut();
    /**
     * On fade animation out, set the overlay message visibility to false
     */
    void onFadeOut();

private:
    /// GUI
    Ui::OverlayMessageDisplay *ui;
    /// Timer for overlay hide
    QTimer _timer;
    /// Animation for display and hide message
    QPropertyAnimation *_opacityAnimation;
    /// Opacity effect
    QGraphicsOpacityEffect *_opacityEffect;
};

#endif // OVERLAYMESSAGEDISPLAY_H
