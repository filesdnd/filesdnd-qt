SOURCES += \
    qtsingleapplication/lockedfile/qtlockedfile.cpp

HEADERS += \
    qtsingleapplication/lockedfile/qtlockedfile.h

unix: SOURCES += qtsingleapplication/lockedfile/qtlockedfile_unix.cpp
win32: SOURCES += qtsingleapplication/lockedfile/qtlockedfile_win.cpp
