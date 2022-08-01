QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#include(3rdParty/qtserialport/src/serialport/serialport-lib.pri)

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SerialHandlers/serialdevicehandler.cpp \
    SerialHandlers/serialhandlerbase.cpp \
    SerialHandlers/serialmenuhandler.cpp \
    SerialHandlers/serialport.cpp \
    SerialHandlers/serialreadthread.cpp \
    SerialHandlers/serialwritethread.cpp \
    SettingsPages/Files/deletefilethread.cpp \
    SettingsPages/Files/filebrowserform.cpp \
    SettingsPages/Files/savefilethread.cpp \
    SettingsPages/Settings/aboutpage.cpp \
    SettingsPages/Settings/adaptivefilterform.cpp \
    SettingsPages/Settings/datafilemenu.cpp \
    SettingsPages/Settings/datafilemenuform.cpp \
    SettingsPages/Settings/datetimeform.cpp \
    SettingsPages/Settings/flowcalform.cpp \
    SettingsPages/Settings/modbusserverform.cpp \
    SettingsPages/Settings/modeform.cpp \
    SettingsPages/Settings/no2calform.cpp \
    SettingsPages/Settings/nocalform.cpp \
    SettingsPages/Settings/staticipform.cpp \
    SettingsPages/Settings/systemsettingsform.cpp \
    SettingsPages/Utility/numberpadform.cpp \
    SettingsPages/averagetimeform.cpp \
    SettingsPages/basepage.cpp \
    SettingsPages/basesettingspage.cpp \
    SettingsPages/blockingform.cpp \
    SettingsPages/settingsmenu.cpp \
    ValueHandlers/ValueObjects/basevalueobject.cpp \
    ValueHandlers/ValueObjects/valueobjects.cpp \
    ValueHandlers/datahandler.cpp \
    ValueHandlers/settingshandler.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GlobalDefinitions.h \
    SerialHandlers/serialdevicehandler.h \
    SerialHandlers/serialhandlerbase.h \
    SerialHandlers/serialmenuhandler.h \
    SerialHandlers/serialport.h \
    SerialHandlers/serialreadthread.h \
    SerialHandlers/serialwritethread.h \
    SettingsPages/Files/deletefilethread.h \
    SettingsPages/Files/filebrowserform.h \
    SettingsPages/Files/savefilethread.h \
    SettingsPages/Settings/aboutpage.h \
    SettingsPages/Settings/adaptivefilterform.h \
    SettingsPages/Settings/datafilemenu.h \
    SettingsPages/Settings/datafilemenuform.h \
    SettingsPages/Settings/datetimeform.h \
    SettingsPages/Settings/flowcalform.h \
    SettingsPages/Settings/modbusserverform.h \
    SettingsPages/Settings/modeform.h \
    SettingsPages/Settings/no2calform.h \
    SettingsPages/Settings/nocalform.h \
    SettingsPages/Settings/staticipform.h \
    SettingsPages/Settings/systemsettingsform.h \
    SettingsPages/Utility/numberpadform.h \
    SettingsPages/averagetimeform.h \
    SettingsPages/basepage.h \
    SettingsPages/basesettingspage.h \
    SettingsPages/blockingform.h \
    SettingsPages/settingsmenu.h \
    ValueHandlers/ValueObjects/basevalueobject.h \
    ValueHandlers/ValueObjects/valueobjects.h \
    ValueHandlers/datahandler.h \
    ValueHandlers/settingshandler.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
