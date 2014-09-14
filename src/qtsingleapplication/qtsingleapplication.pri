SOURCES += \
    $$PWD/singleapplication/qtsinglecoreapplication.cpp \
    $$PWD/singleapplication/qtsingleapplication.cpp \
    $$PWD/singleapplication/qtlocalpeer.cpp \
    $$PWD/lockedfile/qtlockedfile.cpp

HEADERS += \
    $$PWD/singleapplication/qtsinglecoreapplication.h \
    $$PWD/singleapplication/qtsingleapplication.h \
    $$PWD/singleapplication/qtlocalpeer.h \
    $$PWD/lockedfile/qtlockedfile.h

win32 {
    SOURCES += $$PWD/lockedfile/qtlockedfile_win.cpp
} else {
    SOURCES += $$PWD/lockedfile/qtlockedfile_unix.cpp
}
