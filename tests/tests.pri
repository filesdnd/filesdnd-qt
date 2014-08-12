SOURCES += \
    $$PWD/../tests/autotest.h \
    $$PWD/../tests/modeltest.h

HEADERS += \
    $$PWD/../tests/modeltest.cpp

CONFIG(debug) {
    #QT += testlib
    #QMAKE_CXXFLAGS += -DVERBOSE -DRUN_TESTS
}
