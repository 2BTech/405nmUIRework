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
    SettingsPages/averagetimeform.cpp \
    SettingsPages/basepage.cpp \
    SettingsPages/basesettingspage.cpp \
    SettingsPages/settingsmenu.cpp \
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
    SettingsPages/averagetimeform.h \
    SettingsPages/basepage.h \
    SettingsPages/basesettingspage.h \
    SettingsPages/settingsmenu.h \
    ValueHandlers/datahandler.h \
    ValueHandlers/settingshandler.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
