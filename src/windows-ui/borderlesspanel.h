#ifndef BORDERLESSPANEL_H
#define BORDERLESSPANEL_H

#include <QMouseEvent>

#include "qwinwidget.h"
#include "view.h"

class BorderlessPanel : public QWinWidget {
    Q_OBJECT

public:
    BorderlessPanel(HWND hWnd, View *view);
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

    void maximizeButtonState();
    void restoreButtonState();

private:
    HWND _windowHandle;
    View *_view;
    QCloseEvent _closeEvent;
    QPushButton *_pushButtonMaximize;
};

#endif // BORDERLESSPANEL_H
