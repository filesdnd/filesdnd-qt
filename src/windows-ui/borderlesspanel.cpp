#include <windows.h>

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>

#include "borderlesspanel.h"
#include "borderlesswindow.h"

#include <windowsx.h>

BorderlessPanel::BorderlessPanel(HWND hWnd , View *view) :
    QWinWidget(hWnd)
{
    _closeEvent = QCloseEvent();
    _view = view;
    _windowHandle = hWnd;
    setObjectName("mainPanel");

    setStyleSheet(FileHelper::loadFileContent(WINDOWS_APP_CSS));

    // Horizontal layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);

    // Window title
    QLabel *windowTitle = new QLabel(this);
    windowTitle->setObjectName("windowTitle");
    windowTitle->setText("Files Drag & Drop");
    windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Center window title
    horizontalLayout->addStretch();
    horizontalLayout->addSpacing(70);
    horizontalLayout->addWidget(windowTitle);
    horizontalLayout->addStretch();

    // System buttons
    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    horizontalLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT( pushButtonMinimizeClicked()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    horizontalLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximizeClicked()));

    // Close
    QPushButton *pushButtonClose = new QPushButton( "", this );
    pushButtonClose->setObjectName( "pushButtonClose" );
    horizontalLayout->addWidget( pushButtonClose );
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonCloseClicked()));

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(0);
    verticalLayout->setAlignment(Qt::AlignHCenter);
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(view);

    // Show
    centralWidget->setLayout(verticalLayout);
    mainGridLayout->addWidget(centralWidget);

    show();
}

// Button events
void BorderlessPanel::pushButtonMinimizeClicked() {
    ShowWindow(parentWindow(), SW_MINIMIZE);
}

void BorderlessPanel::pushButtonMaximizeClicked() {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(parentWindow(), &wp);

    if (wp.showCmd == SW_MAXIMIZE) {
        ShowWindow(parentWindow(), SW_RESTORE);
    } else {
        ShowWindow(parentWindow(), SW_MAXIMIZE);
    }
}

void BorderlessPanel::pushButtonCloseClicked() {
    _view->closeEvent(&_closeEvent);
}

#if QT_VERSION >= 0x050000
bool BorderlessPanel::nativeEvent(const QByteArray &, void *msg, long *) {
#else
bool BorderlessPanel::winEvent(MSG *message, long *) {
#endif
#if QT_VERSION >= 0x050000
    MSG *message = (MSG *)msg;
#endif
    switch(message->message)
    {
        case WM_SYSKEYDOWN:
            if (message->wParam == VK_SPACE) {
                RECT winrect;
                GetWindowRect(_windowHandle, &winrect);
                TrackPopupMenu(GetSystemMenu(_windowHandle, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, _windowHandle, NULL);
                break;
            }
            break;
        case WM_KEYDOWN:
            if (message->wParam == VK_F5 ||
                    message->wParam == VK_F6 ||
                    message->wParam == VK_F7)
            {
                SendMessage(_windowHandle, WM_KEYDOWN, message->wParam, message->lParam);
                break;
            }
            break;
        case WM_KILLFOCUS:
            SendMessage(_windowHandle, WM_KILLFOCUS, message->wParam, message->lParam);
            break;
        case WM_SETFOCUS:
            SendMessage(_windowHandle, WM_SETFOCUS, message->wParam, message->lParam);
            break;
    }

    return false;
}

void BorderlessPanel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        ReleaseCapture();
        SendMessage(_windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }

    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (event -> button() == Qt::LeftButton)
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(parentWindow(), &wp);
            if (wp.showCmd == SW_MAXIMIZE) {
                ShowWindow(parentWindow(), SW_RESTORE);
            } else {
                ShowWindow(parentWindow(), SW_MAXIMIZE);
            }
        }
    }
}
