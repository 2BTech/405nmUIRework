#include "modbusserverform.h"

ModbusServerForm::ModbusServerForm() : BasePage("Modbus Server")
{

}

ModbusServerForm::~ModbusServerForm()
{
    if (updateButton)
    {
        ipPair.first->deleteLater();
        ipPair.second->deleteLater();

        portPair.first->deleteLater();
        portPair.second->deleteLater();

        idPair.first->deleteLater();
        idPair.second->deleteLater();

        updateButton->deleteLater();

        statusLabel->deleteLater();

        updateButton = Q_NULLPTR;

        statusLabel = Q_NULLPTR;
    }
}

void ModbusServerForm::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(10);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(70, 70, 340, 40);
    statusLabel->setText("Connection from 127.0.0.1");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(font);

    font.setPointSize(12);

    ipPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    ipPair.second->setGeometry(100, 100, 280, 40);
    ipPair.second->setText("IP Address: 000.000.000.000");
    ipPair.second->setFont(font);

    ipPair.first->setGeometry(100, 100, 280, 40);
    ipPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(ipPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnIPAddressClicked);

    portPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    portPair.second->setGeometry(100, 140, 280, 40);
    portPair.second->setText("Port: 50200");
    portPair.second->setFont(font);

    portPair.first->setGeometry(100, 140, 280, 40);
    portPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(portPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnPortClicked);

    idPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    idPair.second->setGeometry(100, 180, 280, 40);
    idPair.second->setText("Device ID: 50200");
    idPair.second->setFont(font);

    idPair.first->setGeometry(100, 180, 280, 40);
    idPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(idPair.first, &QPushButton::clicked, this, &ModbusServerForm::OnDeviceIDClicked);

    updateButton = new QPushButton(this);
    updateButton->setGeometry(180, 230, 120, 40);
    updateButton->setText("Update");
    updateButton->setFont(font);
    updateButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(updateButton, &QPushButton::clicked, this, &ModbusServerForm::OnUpdateClicked);
}

void ModbusServerForm::OnIPAddressClicked()
{

}

void ModbusServerForm::OnPortClicked()
{

}

void ModbusServerForm::OnDeviceIDClicked()
{

}

void ModbusServerForm::OnUpdateClicked()
{

}
