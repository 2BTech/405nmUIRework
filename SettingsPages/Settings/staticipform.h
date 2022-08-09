#ifndef STATICIPFORM_H
#define STATICIPFORM_H

#include <QProcess>

#include "../basesettingspage.h"
#include "../blockingform.h"

class StaticIPForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    StaticIPForm();
    ~StaticIPForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

private slots:
    void OnIPClicked();
    void ReceiveStaticIP(QString val);

    void OnGatewayClicked();
    void ReceiveGateway(QString val);

    void OnSubnetClicked();
    void ReceiveSubnet(QString val);

    void OnApplyClicked();
    void OnClearClicked();

    void OnSetStaticIPExit(int exitCode, QProcess::ExitStatus exitStatus);
    void OnSetStaticIPError(QProcess::ProcessError error);

    void OnClearStaticIPExit(int exitCode, QProcess::ExitStatus exitStatus);
    void OnClearStaticIPError(QProcess::ProcessError error);

private:
    QLabel* statusLabel = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> ipPair;
    QPair<QPushButton*,QLabel*> gatewayPair;
    QPair<QPushButton*,QLabel*> subnetPair;
    QPushButton* applyButton = Q_NULLPTR;
    QPushButton* clearButton = Q_NULLPTR;

    BlockingForm* blocking = Q_NULLPTR;

    QProcess setStaticIPProcess;
    QProcess clearStaticIPPRocess;

    void UpdateUI() override;

    void HandleOldSettingsFile();
};

#endif // STATICIPFORM_H
