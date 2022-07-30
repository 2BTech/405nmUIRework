#include "staticipform.h"

StaticIPForm::StaticIPForm() : BasePage("Static IP Settings")
{

}

StaticIPForm::~StaticIPForm()
{
    if (statusLabel)
    {
        statusLabel->deleteLater();

        ipPair.first->deleteLater();
        ipPair.second->deleteLater();

        gatewayPair.first->deleteLater();
        gatewayPair.second->deleteLater();

        subnetPair.first->deleteLater();
        subnetPair.second->deleteLater();

        applyButton->deleteLater();
        clearButton->deleteLater();

        statusLabel = Q_NULLPTR;

        applyButton = clearButton = Q_NULLPTR;
    }
}

void StaticIPForm::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(10);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(70, 70, 340, 40);
    statusLabel->setText("Status: Using dynamic address");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(font);

    font.setPointSize(12);

    ipPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    ipPair.second->setGeometry(100, 100, 280, 40);
    ipPair.second->setText("IP Address: 000.000.000.000");
    ipPair.second->setFont(font);

    ipPair.first->setGeometry(100, 100, 280, 40);
    ipPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(ipPair.first, &QPushButton::clicked, this, &StaticIPForm::OnIPClicked);

    gatewayPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    gatewayPair.second->setGeometry(100, 140, 280, 40);
    gatewayPair.second->setText("Gateway: 000.000.000.000");
    gatewayPair.second->setFont(font);

    gatewayPair.first->setGeometry(100, 140, 280, 40);
    gatewayPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(gatewayPair.first, &QPushButton::clicked, this, &StaticIPForm::OnGatewayClicked);

    subnetPair = QPair<QPushButton*,QLabel*>(new QPushButton(this), new QLabel(this));

    subnetPair.second->setGeometry(100, 180, 280, 40);
    subnetPair.second->setText("Subnet Mask: 000.000.000.000");
    subnetPair.second->setFont(font);

    subnetPair.first->setGeometry(100, 180, 280, 40);
    subnetPair.first->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); }");
    connect(subnetPair.first, &QPushButton::clicked, this, &StaticIPForm::OnSubnetClicked);

    clearButton = new QPushButton(this);
    clearButton->setGeometry(100, 230, 120, 40);
    clearButton->setText("Clear");
    clearButton->setFont(font);
    clearButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(clearButton, &QPushButton::clicked, this, &StaticIPForm::OnClearClicked);

    applyButton = new QPushButton(this);
    applyButton->setGeometry(260, 230, 120, 40);
    applyButton->setText("Apply");
    applyButton->setFont(font);
    applyButton->setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6fa0cc, stop:1 #627c8a); }");
    connect(applyButton, &QPushButton::clicked, this, &StaticIPForm::OnClearClicked);
}

void StaticIPForm::OnIPClicked()
{

}

void StaticIPForm::OnGatewayClicked()
{

}

void StaticIPForm::OnSubnetClicked()
{

}

void StaticIPForm::OnApplyClicked()
{

}

void StaticIPForm::OnClearClicked()
{

}
