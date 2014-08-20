SOURCES += \
    common/other/controller.cpp \
    common/other/model.cpp \
    common/other/fdndapplication.cpp \
    common/other/updatemanager.cpp \
    common/config/appconfig.cpp \
    common/entities/device.cpp \
    common/entities/datastruct.cpp \
    common/entities/txtrecord.cpp \
    common/entities/service.cpp \
    common/entities/historyelement.cpp \
    common/threads/servicethread.cpp \
    common/threads/clipboardthreadevent.cpp \
    common/threads/devicethread.cpp \
    common/threads/deviceconnectionthreadevent.cpp \
    common/threads/devicepingthreadevent.cpp \
    common/threads/devicepongthreadevent.cpp \
    common/threads/devicecanceltransfertthreadevent.cpp \
    common/helpers/filehelper.cpp \
    common/helpers/logmanager.cpp \
    common/helpers/folderzipper.cpp \
    common/helpers/settingsmanager.cpp \
    common/helpers/servicehelper.cpp \
    common/udp/udpdiscovery.cpp \
    common/view/deviceview.cpp \
    common/view/smalldeviceview.cpp \
    common/view/abstractdeviceview.cpp \
    common/view/dialogs/aboutdialog.cpp \
    common/view/dialogs/settingsdialog.cpp \
    common/view/historyelementview.cpp \
    common/view/centerinfowidget.cpp \
    common/view/dialogs/updatedialog.cpp \
    common/view/widget.cpp \
    common/view/overlaymessagedisplay.cpp \
    common/view/progressindicator.cpp \
    common/view/view.cpp \
    common/view/configpanel/configpanel.cpp \
    common/helpers/fonthelper.cpp \
    common/view/configpanel/transparentscrollbar.cpp \
    common/view/configpanel/transparentscrollbutton.cpp \
    common/view/configpanel/historylistwidget.cpp

HEADERS += \
    common/other/controller.h \
    common/other/model.h \
    common/other/fdndapplication.h \
    common/other/updatemanager.h \
    common/config/appconfig.h \
    common/entities/device.h \
    common/entities/datastruct.h \
    common/entities/txtrecord.h \
    common/entities/service.h \
    common/entities/historyelement.h \
    common/threads/servicethread.h \
    common/threads/clipboardthreadevent.h \
    common/threads/devicethread.h \
    common/threads/deviceconnectionthreadevent.h \
    common/threads/devicepingthreadevent.h \
    common/threads/devicepongthreadevent.h \
    common/threads/devicecanceltransfertthreadevent.h \
    common/helpers/filehelper.h \
    common/helpers/logmanager.h \
    common/helpers/folderzipper.h \
    common/helpers/settingsmanager.h \
    common/helpers/servicehelper.h \
    common/udp/udpdiscovery.h \
    common/view/deviceview.h \
    common/view/smalldeviceview.h \
    common/view/abstractdeviceview.h \
    common/view/dialogs/aboutdialog.h \
    common/view/dialogs/settingsdialog.h \
    common/view/historyelementview.h \
    common/view/centerinfowidget.h \
    common/view/dialogs/updatedialog.h \
    common/view/widget.h \
    common/view/overlaymessagedisplay.h \
    common/view/progressindicator.h \
    common/view/view.h \
    common/view/configpanel/configpanel.h \
    common/helpers/fonthelper.h \
    common/view/configpanel/transparentscrollbar.h \
    common/view/configpanel/transparentscrollbutton.h \
    common/view/configpanel/historylistwidget.h

FORMS += \
    common/view/deviceview.ui \
    common/view/smalldeviceview.ui \
    common/view/dialogs/aboutdialog.ui \
    common/view/dialogs/settingsdialog.ui \
    common/view/historyelementview.ui \
    common/view/centerinfowidget.ui \
    common/view/dialogs/updatedialog.ui \
    common/view/overlaymessagedisplay.ui \
    common/view/view.ui \
    common/view/configpanel/configpanel.ui

OTHER_FILES += \
    common/configpanel.css
