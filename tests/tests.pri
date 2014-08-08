SOURCES += \
    $$PWD/../tests/autotest.h \
    $$PWD/../tests/modeltest.h

HEADERS += \
    $$PWD/../tests/modeltest.cpp

debug {
    QT += testlib
}
