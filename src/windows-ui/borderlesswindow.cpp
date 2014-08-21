#include "borderlesswindow.h"

#include <dwmapi.h>
#include <windowsx.h>
#include <stdexcept>

#include <QPushButton>
#include <QFile>

HWND winId = 0;

BorderlessPanel *BorderlessWindow::mainPanel;
QApplication *BorderlessWindow::a;
LONG BorderlessWindow::borderWidth = 8;
HINSTANCE BorderlessWindow::hInstance = GetModuleHandle(NULL);

BorderlessWindow::BorderlessWindow(QApplication *app, const int x, const int y, const int width, const int height, View *view) :
    hWnd(0),
    borderless(false),
    borderlessResizeable(true),
    aeroShadow(false),
    closed(false),
    visible(true)
{
    connect(view, SIGNAL(showWindow()), this, SLOT(show()));

    WNDCLASSEX wcx = { 0 };
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = hInstance;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra	= 0;
    wcx.cbWndExtra	= 0;
    wcx.lpszClassName = L"WindowClass";
    wcx.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wcx.hCursor = LoadCursor(hInstance, IDC_ARROW);
    RegisterClassEx(&wcx);
    if (FAILED(RegisterClassEx(&wcx)))
        throw std::runtime_error("Couldn't register window class");

    hWnd = CreateWindow(L"WindowClass", L"Files Drag & Drop", static_cast<DWORD>(Style::windowed), x, y, width, height, 0, 0, hInstance, nullptr);

    if (!hWnd)
        throw std::runtime_error("Couldn't create window because of reasons");

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    mainPanel = new BorderlessPanel(hWnd, view);
    winId = (HWND)mainPanel->winId();

    toggleBorderless();
    hide();

    a = app;
}

BorderlessWindow::~BorderlessWindow()
{
    hide();
    DestroyWindow(hWnd);
}

HDC hdc;
PAINTSTRUCT ps;

LRESULT CALLBACK BorderlessWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BorderlessWindow *window = reinterpret_cast<BorderlessWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (!window)
        return DefWindowProc(hWnd, message, wParam, lParam);

    switch (message) {
        case WM_KEYDOWN: {
            if (wParam != VK_TAB)
                return DefWindowProc(hWnd, message, wParam, lParam);

            SetFocus(winId);
            break;
        }

        // ALT + SPACE or F10 system menu
        case WM_SYSCOMMAND: {
            if (wParam == SC_KEYMENU) {
                RECT winrect;
                GetWindowRect(hWnd, &winrect);
                TrackPopupMenu(GetSystemMenu(hWnd, true), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, hWnd, NULL);
                break;
            } else {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        case WM_SETFOCUS: {
            RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
            break;
        }

        case WM_NCCALCSIZE: {
            //this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
            if (window->borderless) {
                return 0;
            }
            break;
        }

        case WM_KILLFOCUS: {
            RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdcPaint = BeginPaint(hWnd, &ps);
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
            RECT r;

            GetClientRect(hWnd, &r);
            FillRect(hdcPaint, &r, brush);

            SelectObject(hdcPaint, GetStockObject(DC_PEN));
            SetDCPenColor(hdcPaint, RGB(150, 150, 150));
            if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7 && GetFocus() == 0)
                Rectangle(hdcPaint, r.left + 1, r.top + 1, r.right - 1, r.bottom - 1);

            DeleteObject(brush);
            EndPaint(hWnd, &ps);

            return 0;
        }

        case WM_NCHITTEST: {
            if (window->borderless)
            {
                if (window->borderlessResizeable)
                {
                    RECT winrect;
                    GetWindowRect(hWnd, &winrect);
                    long x = GET_X_LPARAM(lParam);
                    long y = GET_Y_LPARAM(lParam);

                    //bottom left corner
                    if (x >= winrect.left && x < winrect.left + borderWidth &&
                         y < winrect.bottom && y >= winrect.bottom - borderWidth)
                    {
                        return HTBOTTOMLEFT;
                    }
                    //bottom right corner
                    if (x < winrect.right && x >= winrect.right - borderWidth &&
                         y < winrect.bottom && y >= winrect.bottom - borderWidth)
                    {
                        return HTBOTTOMRIGHT;
                    }
                    //top left corner
                    if (x >= winrect.left && x < winrect.left + borderWidth &&
                         y >= winrect.top && y < winrect.top + borderWidth)
                    {
                        return HTTOPLEFT;
                    }
                    //top right corner
                    if (x < winrect.right && x >= winrect.right - borderWidth &&
                         y >= winrect.top && y < winrect.top + borderWidth)
                    {
                        return HTTOPRIGHT;
                    }
                    //left border
                    if (x >= winrect.left && x < winrect.left + borderWidth)
                    {
                        return HTLEFT;
                    }
                    //right border
                    if (x < winrect.right && x >= winrect.right - borderWidth)
                    {
                        return HTRIGHT;
                    }
                    //bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
                    {
                        return HTBOTTOM;
                    }
                    //top border
                    if (y >= winrect.top && y < winrect.top + borderWidth)
                    {
                        return HTTOP;
                    }
                }

                return HTCAPTION;
            }
            break;
        }

        case WM_SIZE: {
            RECT winrect;
            GetClientRect(hWnd, &winrect);

            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(hWnd, &wp);

            if (wp.showCmd == SW_MAXIMIZE) {
                QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>("pushButtonMaximize");
                pushButtonMaximize->setStyleSheet("#pushButtonMaximize {image: url(:/borderless/windows-ui/images/borderless-restore.png);} #pushButtonMaximize:hover { image: url(:/borderless/windows-ui/images/borderless-restore-hover.png); }");
                mainPanel->setGeometry(borderWidth * 2, borderWidth * 2, winrect.right - borderWidth * 4, winrect.bottom - borderWidth * 4);
            } else {
                QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>("pushButtonMaximize");
                pushButtonMaximize->setStyleSheet("#pushButtonMaximize {image: url(:/borderless/windows-ui/images/borderless-maximize.png);} #pushButtonMaximize:hover { image: url(:/borderless/windows-ui/images/borderless-maximize-hover.png); }");
                mainPanel->setGeometry(borderWidth, borderWidth, winrect.right - borderWidth * 2, winrect.bottom - borderWidth * 2);
            }
            break;
        }

        case WM_GETMINMAXINFO: {
            MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
            if (window->minimumSize.required) {
                minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();
                minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
            }

            if (window->maximumSize.required) {
                minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
                minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
            }
            return 0;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void BorderlessWindow::toggleBorderless() {
    if (visible)
    {
        Style newStyle = (borderless) ? Style::windowed : Style::aero_borderless;
        SetWindowLongPtr(hWnd, GWL_STYLE, static_cast<LONG>(newStyle));

        borderless = !borderless;
        if (newStyle == Style::aero_borderless) {
            toggleShadow();
        }
    }
}

void BorderlessWindow::toggleShadow() {
    if (borderless) {
        aeroShadow = !aeroShadow;
        const MARGINS shadow_on = { 1, 1, 1, 1 };
        const MARGINS shadow_off = { 0, 0, 0, 0 };
        DwmExtendFrameIntoClientArea(hWnd, (aeroShadow) ? (&shadow_on) : (&shadow_off));
    }
}

void BorderlessWindow::toggleResizeable() {
    borderlessResizeable = borderlessResizeable ? false : true;
}

bool BorderlessWindow::isResizeable() {
    return borderlessResizeable ? true : false;
}

void BorderlessWindow::show() {
    ShowWindow(hWnd, SW_SHOW);
    visible = true;

    // Redraw frame & Update borderless state
    SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void BorderlessWindow::hide() {
    ShowWindow(hWnd, SW_HIDE);
    visible = false;
}

bool BorderlessWindow::isVisible() {
    return visible ? true : false;
}

// Minimum size
void BorderlessWindow::setMinimumSize(const int width, const int height) {
    this->minimumSize.required = true;
    this->minimumSize.width = width;
    this->minimumSize.height = height;
}

bool BorderlessWindow::isSetMinimumSize() {
    return this->minimumSize.required;
}

void BorderlessWindow::removeMinimumSize() {
    this->minimumSize.required = false;
    this->minimumSize.width = 0;
    this->minimumSize.height = 0;
}

int BorderlessWindow::getMinimumWidth() {
    return minimumSize.width;
}

int BorderlessWindow::getMinimumHeight() {
    return minimumSize.height;
}

// Maximum size
void BorderlessWindow::setMaximumSize(const int width, const int height) {
    this->maximumSize.required = true;
    this->maximumSize.width = width;
    this->maximumSize.height = height;
}

bool BorderlessWindow::isSetMaximumSize() {
    return this->maximumSize.required;
}

void BorderlessWindow::removeMaximumSize() {
    this->maximumSize.required = false;
    this->maximumSize.width = 0;
    this->maximumSize.height = 0;
}

int BorderlessWindow::getMaximumWidth() {
    return maximumSize.width;
}

int BorderlessWindow::getMaximumHeight() {
    return maximumSize.height;
}

