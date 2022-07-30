#ifndef MODBUSSERVERFORM_H
#define MODBUSSERVERFORM_H

#include "../basepage.h"

class ModbusServerForm : public BasePage
{
    Q_OBJECT
public:
    ModbusServerForm();
    ~ModbusServerForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

private slots:
    void OnIPAddressClicked();
    void OnPortClicked();
    void OnDeviceIDClicked();
    void OnUpdateClicked();

private:
    QLabel* statusLabel = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> ipPair;
    QPair<QPushButton*,QLabel*> portPair;
    QPair<QPushButton*,QLabel*> idPair;
    QPushButton* updateButton = Q_NULLPTR;
};

#endif // MODBUSSERVERFORM_H
