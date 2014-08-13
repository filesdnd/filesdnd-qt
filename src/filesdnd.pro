#-------------------------------------------------
#
# Project created by QtCreator 2012-11-20T07:41:21
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui-private

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
include("qtsingleapplication/qtsingleapplication.pri")

# Common
include("common/common.pri")
INCLUDEPATH += \
    "$$PWD/common/" \
    "$$PWD/common/config/" \
    "$$PWD/common/entities/" \
    "$$PWD/common/helpers/" \
    "$$PWD/common/threads/" \
    "$$PWD/common/udp/" \
    "$$PWD/common/view/"

# Windows UI
include("windows-ui/windows-ui.pri")
INCLUDEPATH += \
    "$$PWD/windows-ui/"

# Windows UI
include("linux-ui/linux-ui.pri")
INCLUDEPATH += \
    "$$PWD/linux-ui/"

SOURCES += \
    main.cpp

OTHER_FILES += \
    $$PWD/../.travis.yml \
    $$PWD/../LICENSE \
    $$PWD/../.gitignore \
    $$PWD/../README.md

mac:LIBS += /usr/lib/libobjc.dylib
!mac:unix:LIBS += -ldns_sd
win32: LIBS += \
    $$PWD/../lib/dnssd.lib \
    /NoDefaultLib:LIBCMT
win32: LIBS += $$PWD/../lib/AdvAPI32.lib

INCLUDEPATH += \
    "$$PWD" \
    "$$PWD/../include/"

RESOURCES += resources.qrc \
    common/resources.qrc
RC_FILE = icon.rc
