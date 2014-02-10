#-------------------------------------------------
#
# Project created by QtCreator 2012-11-20T07:41:21
#
#-------------------------------------------------

QT += core gui network widgets

TARGET = "Files Drag And Drop"
TEMPLATE = app
CONFIG -= console

TRANSLATIONS = filesdnd_fr.ts \
    filesdnd_en.ts

CODECFORTR = UTF-8
CODECFORSRC = UTF-8

ICON = "images/ic_launcher_hd.icns"

win32: QMAKE_CXXFLAGS += -W4 -DNOMINMAX
unix: QMAKE_CXXFLAGS += -Wall

#CONFIG(debug) {
#    QMAKE_CXXFLAGS += -DVERBOSE -DRUN_TESTS
#    QT += testlib
#}

SOURCES += main.cpp\
    views/view.cpp \
    zeroconf/bonjourserviceresolver.cpp \
    zeroconf/bonjourservicebrowser.cpp \
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
    zeroconf/bonjourservicereconfirmer.cpp \
    zeroconf/bonjourserviceregister.cpp \
    entities/service.cpp \
    entities/historyelement.cpp \
    views/historyelementview.cpp \
    helpers/servicehelper.cpp \
    udp/udpdiscovery.cpp \
    views/centerinfowidget.cpp \
    views/historygripbutton.cpp \
    tests/modeltest.cpp \
    config/appconfig.cpp \
    entities/txtrecord.cpp \
    helpers/folderzipper.cpp \
    threads/servicethread.cpp \
    threads/clipboardthreadevent.cpp \
    threads/devicethread.cpp \
    QtSingleApplication/lockedfile/qtlockedfile.cpp \
    QtSingleApplication/singleapplication/qtsinglecoreapplication.cpp \
    QtSingleApplication/singleapplication/qtsingleapplication.cpp \
    QtSingleApplication/singleapplication/qtlocalpeer.cpp \
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

unix: SOURCES += QtSingleApplication/lockedfile/qtlockedfile_unix.cpp
win32: SOURCES += QtSingleApplication/lockedfile/qtlockedfile_win.cpp

HEADERS  += views/view.h \
    zeroconf/bonjourserviceresolver.h \
    zeroconf/bonjourservicebrowser.h \
    zeroconf/bonjourrecord.h \
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
    zeroconf/bonjourservicereconfirmer.h \
    zeroconf/bonjourserviceregister.h \
    entities/service.h \
    entities/historyelement.h \
    views/historyelementview.h \
    config/appconfig.h \
    helpers/servicehelper.h \
    udp/udpdiscovery.h \
    views/centerinfowidget.h \
    views/historygripbutton.h \
    tests/modeltest.h \
    tests/autotest.h \
    entities/txtrecord.h \
    helpers/folderzipper.h \
    threads/servicethread.h \
    threads/clipboardthreadevent.h \
    threads/devicethread.h \
    QtSingleApplication/lockedfile/qtlockedfile.h \
    QtSingleApplication/singleapplication/qtsinglecoreapplication.h \
    QtSingleApplication/singleapplication/qtsingleapplication.h \
    QtSingleApplication/singleapplication/qtlocalpeer.h \
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

FORMS += views/view.ui \
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
win32: LIBS += $$PWD/lib/dnssd.lib \
     /NoDefaultLib:LIBCMT
win32: LIBS += $$PWD/lib/AdvAPI32.lib

INCLUDEPATH += "./include/" \
    "./views/" \
    "./views/dialogs" \
    "./entities/" \
    "./config/" \
    "./helpers/" \
    "./udp/" \
    "./zeroconf/"

RESOURCES += \
    resources.qrc

RC_FILE = icon.rc
