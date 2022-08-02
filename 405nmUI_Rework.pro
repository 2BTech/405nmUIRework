QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#include(3rdParty/qtserialport/src/serialport/serialport-lib.pri)

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Modbus/ASCII/modbusasciibase.cpp \
    Modbus/ASCII/modbusasciimaster.cpp \
    Modbus/ASCII/modbusasciimessage.cpp \
    Modbus/ASCII/modbusasciislave.cpp \
    Modbus/RTU/modbusrtubase.cpp \
    Modbus/RTU/modbusrtumaster.cpp \
    Modbus/RTU/modbusrtumessage.cpp \
    Modbus/RTU/modbusrtuslave.cpp \
    Modbus/TCP/modbustcpbase.cpp \
    Modbus/TCP/modbustcpmaster.cpp \
    Modbus/TCP/modbustcpmessage.cpp \
    Modbus/TCP/modbustcpslave.cpp \
    Modbus/modbusbase.cpp \
    Modbus/modbusdatatable.cpp \
    Modbus/modbushandler.cpp \
    Modbus/modbushelpers.cpp \
    Modbus/modbusmaster.cpp \
    Modbus/modbusmessage.cpp \
    Modbus/modbusslave.cpp \
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
    ValueHandlers/valuehandlerbase.cpp \
    main.cpp \
    mainwindow.cpp \
    parametersform.cpp \
    utilities.cpp

HEADERS += \
    GlobalDefinitions.h \
    Modbus/ASCII/modbusasciibase.h \
    Modbus/ASCII/modbusasciimaster.h \
    Modbus/ASCII/modbusasciimessage.h \
    Modbus/ASCII/modbusasciislave.h \
    Modbus/RTU/modbusrtubase.h \
    Modbus/RTU/modbusrtumaster.h \
    Modbus/RTU/modbusrtumessage.h \
    Modbus/RTU/modbusrtuslave.h \
    Modbus/TCP/modbustcpbase.h \
    Modbus/TCP/modbustcpmaster.h \
    Modbus/TCP/modbustcpmessage.h \
    Modbus/TCP/modbustcpslave.h \
    Modbus/modbusbase.h \
    Modbus/modbusdatatable.h \
    Modbus/modbusdefs.h \
    Modbus/modbushandler.h \
    Modbus/modbushelpers.h \
    Modbus/modbusmaster.h \
    Modbus/modbusmessage.h \
    Modbus/modbusslave.h \
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
    ValueHandlers/modbusvaluedefs.h \
    ValueHandlers/settingshandler.h \
    ValueHandlers/valuehandlerbase.h \
    mainwindow.h \
    parametersform.h \
    utilities.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
