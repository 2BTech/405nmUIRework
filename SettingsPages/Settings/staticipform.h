#ifndef STATICIPFORM_H
#define STATICIPFORM_H

#include "../basesettingspage.h"

class StaticIPForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    StaticIPForm();
    ~StaticIPForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

signals:
    void UpdateStaticIPSettings(QString ipAddress, QString gateway, QString subnet);
    void ClearStaticIPAddress();

private slots:
    void OnIPClicked();
    void ReceiveStaticIP(QString val);

    void OnGatewayClicked();
    void ReceiveGateway(QString val);

    void OnSubnetClicked();
    void ReceiveSubnet(QString val);

    void OnApplyClicked();
    void OnClearClicked();

private:
    QLabel* statusLabel = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> ipPair;
    QPair<QPushButton*,QLabel*> gatewayPair;
    QPair<QPushButton*,QLabel*> subnetPair;
    QPushButton* applyButton = Q_NULLPTR;
    QPushButton* clearButton = Q_NULLPTR;

    void UpdateUI() override;

    void HandleOldSettingsFile();
};

#endif // STATICIPFORM_H
