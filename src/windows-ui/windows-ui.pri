LIBS += gdi32.lib dwmapi.lib

OTHER_FILES += \
    windows-ui/application.css

SOURCES += \
    windows-ui/borderlesswindow.cpp \
    windows-ui/borderlesspanel.cpp \
    windows-ui/qwinhost.cpp \
    windows-ui/qwinwidget.cpp

HEADERS += \
    windows-ui/borderlesswindow.h \
    windows-ui/borderlesspanel.h \
    windows-ui/qwinhost.h \
    windows-ui/qwinwidget.h
