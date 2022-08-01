#ifndef MODBUSSERVERFORM_H
#define MODBUSSERVERFORM_H

#include "../basesettingspage.h"
#include "utilities.h"

class ModbusServerForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    ModbusServerForm();
    ~ModbusServerForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

public slots:
    void OnModbusConnection(QString peerIP);
    void OnModbusDisconnect();

signals:
    void UpdateModbus(QString ipAddress, int port, uchar deviceID);

private slots:
    void OnIPAddressClicked();

    void OnPortClicked();
    void ReceiveNewPort(QString val);

    void OnDeviceIDClicked();
    void ReceiveNewID(QString val);

    //void OnUpdateClicked();
    void OnSaveClicked() override;

private:
    QLabel* statusLabel = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> ipPair;
    QPair<QPushButton*,QLabel*> portPair;
    QPair<QPushButton*,QLabel*> idPair;
    //QPushButton* updateButton = Q_NULLPTR;

    QString deviceIPAddress = "127.0.0.1";

    void CheckForOldSettings();

    void UpdateUI() override;

    void showEvent(QShowEvent* event) override;
};

#endif // MODBUSSERVERFORM_H
