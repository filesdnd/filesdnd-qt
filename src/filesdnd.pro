#-------------------------------------------------
#
# Project created by QtCreator 2012-11-20T07:41:21
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "files-drag-and-drop"
TEMPLATE = app
CONFIG -= console

TRANSLATIONS = $$PWD/../filesdnd_fr.ts \
    $$PWD/../filesdnd_en.ts

CODECFORTR = UTF-8
CODECFORSRC = UTF-8

ICON = "$$PWD/images/ic_launcher_hd.icns"

win32: QMAKE_CXXFLAGS += -W4 -DNOMINMAX
unix: QMAKE_CXXFLAGS += -Wall

# Tests
include("$PWD/../../tests/tests.pri")
INCLUDEPATH += \
    "$$PWD/../tests/"

# Zeroconf lib
include("zeroconf/zeroconf.pri")
INCLUDEPATH += \
    "$$PWD/zeroconf/"

# QtSingleApplication
include("qtsingleapplication/singleapplication/singleapplication.pri")
include("qtsingleapplication/lockedfile/lockedfile.pri")

# Common
include("common/common.pri")
INCLUDEPATH += \
    "$$PWD/common/" \
    "$$PWD/common/config/" \
    "$$PWD/common/entities/" \
    "$$PWD/common/helpers/" \
    "$$PWD/common/threads/" \
    "$$PWD/common/udp/"

SOURCES += \
    main.cpp\
    views/view.cpp \
    views/deviceview.cpp \
    views/smalldeviceview.cpp \
    views/abstractdeviceview.cpp \
    views/dialogs/aboutdialog.cpp \
    views/dialogs/settingsdialog.cpp \
    views/historyelementview.cpp \
    views/centerinfowidget.cpp \
    views/historygripbutton.cpp \
    views/dialogs/updatedialog.cpp \
    views/widget.cpp \
    views/overlaymessagedisplay.cpp \
    views/progressindicator.cpp

HEADERS += \
    views/view.h \
    views/deviceview.h \
    views/smalldeviceview.h \
    views/abstractdeviceview.h \
    views/dialogs/aboutdialog.h \
    views/dialogs/settingsdialog.h \
    views/historyelementview.h \
    views/centerinfowidget.h \
    views/historygripbutton.h \
    views/dialogs/updatedialog.h \
    views/widget.h \
    views/overlaymessagedisplay.h \
    views/progressindicator.h

FORMS += \
    views/view.ui \
    views/deviceview.ui \
    views/smalldeviceview.ui \
    views/dialogs/aboutdialog.ui \
    views/dialogs/settingsdialog.ui \
    views/historyelementview.ui \
    views/centerinfowidget.ui \
    views/dialogs/updatedialog.ui \
    views/overlaymessagedisplay.ui

mac:LIBS += /usr/lib/libobjc.dylib
!mac:unix:LIBS += -ldns_sd
win32: LIBS += \
    $$PWD/../lib/dnssd.lib \
    /NoDefaultLib:LIBCMT
win32: LIBS += $$PWD/../lib/AdvAPI32.lib

INCLUDEPATH += \
    "$$PWD" \
    "$$PWD/../include/" \
    "$$PWD/views/"

RESOURCES += resources.qrc
RC_FILE = icon.rc
