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
    common/helpers/fonthelper.cpp \
    common/udp/udpdiscovery.cpp \
    common/view/devicespanel/abstractdeviceview.cpp \
    common/view/devicespanel/overlaymessagedisplay.cpp \
    common/view/devicespanel/centerinfowidget.cpp \
    common/view/devicespanel/deviceview.cpp \
    common/view/devicespanel/aboutwidget.cpp \
    common/view/devicespanel/settingswidget.cpp \
    common/view/dialogs/updatedialog.cpp \
    common/view/widgets/widget.cpp \
    common/view/widgets/smalldeviceview.cpp \
    common/view/other/progressindicator.cpp \
    common/view/other/view.cpp \
    common/view/configpanel/configpanel.cpp \
    common/view/configpanel/historyelementview.cpp \
    common/view/configpanel/transparentscrollbar.cpp \
    common/view/configpanel/transparentscrollbutton.cpp \
    common/view/configpanel/historylistwidget.cpp \
    common/view/devicespanel/slidingstackedwidget.cpp

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
    common/helpers/fonthelper.h \
    common/helpers/servicehelper.h \
    common/udp/udpdiscovery.h \
    common/view/devicespanel/abstractdeviceview.h \
    common/view/devicespanel/overlaymessagedisplay.h \
    common/view/devicespanel/centerinfowidget.h \
    common/view/devicespanel/deviceview.h \
    common/view/devicespanel/aboutwidget.h \
    common/view/devicespanel/settingswidget.h \
    common/view/dialogs/updatedialog.h \
    common/view/widgets/widget.h \
    common/view/widgets/smalldeviceview.h \
    common/view/other/progressindicator.h \
    common/view/other/view.h \
    common/view/configpanel/configpanel.h \
    common/view/configpanel/historyelementview.h \
    common/view/configpanel/transparentscrollbar.h \
    common/view/configpanel/transparentscrollbutton.h \
    common/view/configpanel/historylistwidget.h \
    common/view/devicespanel/slidingstackedwidget.h

FORMS += \
    common/view/devicespanel/deviceview.ui \
    common/view/devicespanel/centerinfowidget.ui \
    common/view/devicespanel/overlaymessagedisplay.ui \
    common/view/widgets/smalldeviceview.ui \
    common/view/dialogs/updatedialog.ui \
    common/view/other/view.ui \
    common/view/configpanel/historyelementview.ui \
    common/view/configpanel/configpanel.ui \
    common/view/devicespanel/settingswidget.ui \
    common/view/devicespanel/aboutwidget.ui

OTHER_FILES += \
    common/configpanel.css
