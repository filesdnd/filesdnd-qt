#-------------------------------------------------
#
# Project created by QtCreator 2012-11-20T07:41:21
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = "files"
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
#QMAKE_CXXFLAGS += -DVERBOSE -DRUN_TESTS

# Zeroconf lib
include("zeroconf/zeroconf.pri")

# QtSingleApplication
include("qtsingleapplication/singleapplication/singleapplication.pri")
include("qtsingleapplication/lockedfile/lockedfile.pri")

SOURCES += \
    main.cpp\
    views/view.cpp \
    controller.cpp \
    views/deviceview.cpp \
    model.cpp \
    entities/device.cpp \
    entities/datastruct.cpp \
    helpers/filehelper.cpp \
    helpers/logmanager.cpp \
    views/smalldeviceview.cpp \
    views/abstractdeviceview.cpp \
    views/dialogs/aboutdialog.cpp \
    helpers/settingsmanager.cpp \
    views/dialogs/settingsdialog.cpp \
    entities/service.cpp \
    entities/historyelement.cpp \
    views/historyelementview.cpp \
    helpers/servicehelper.cpp \
    udp/udpdiscovery.cpp \
    views/centerinfowidget.cpp \
    views/historygripbutton.cpp \
    config/appconfig.cpp \
    entities/txtrecord.cpp \
    helpers/folderzipper.cpp \
    threads/servicethread.cpp \
    threads/clipboardthreadevent.cpp \
    threads/devicethread.cpp \
    threads/deviceconnectionthreadevent.cpp \
    threads/devicepingthreadevent.cpp \
    threads/devicepongthreadevent.cpp \
    updatemanager.cpp \
    views/dialogs/updatedialog.cpp \
    views/widget.cpp \
    views/overlaymessagedisplay.cpp \
    views/progressindicator.cpp \
    threads/devicecanceltransfertthreadevent.cpp \
    fdndapplication.cpp

HEADERS += \
    views/view.h \
    controller.h \
    views/deviceview.h \
    model.h \
    entities/device.h \
    entities/datastruct.h \
    helpers/filehelper.h \
    helpers/logmanager.h \
    views/smalldeviceview.h \
    views/abstractdeviceview.h \
    views/dialogs/aboutdialog.h \
    helpers/settingsmanager.h \
    views/dialogs/settingsdialog.h \
    entities/service.h \
    entities/historyelement.h \
    views/historyelementview.h \
    config/appconfig.h \
    helpers/servicehelper.h \
    udp/udpdiscovery.h \
    views/centerinfowidget.h \
    views/historygripbutton.h \
    entities/txtrecord.h \
    helpers/folderzipper.h \
    threads/servicethread.h \
    threads/clipboardthreadevent.h \
    threads/devicethread.h \
    threads/deviceconnectionthreadevent.h \
    threads/devicepingthreadevent.h \
    threads/devicepongthreadevent.h \
    updatemanager.h \
    views/dialogs/updatedialog.h \
    views/widget.h \
    views/overlaymessagedisplay.h \
    views/progressindicator.h \
    threads/devicecanceltransfertthreadevent.h \
    fdndapplication.h

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
    "$$PWD/../tests/" \
    "$$PWD/config/" \
    "$$PWD/entities/" \
    "$$PWD/helpers/" \
    "$$PWD/threads/" \
    "$$PWD/udp/" \
    "$$PWD/views/" \
    "$$PWD/zeroconf/"

RESOURCES += resources.qrc
RC_FILE = icon.rc
