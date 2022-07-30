#ifndef STATICIPFORM_H
#define STATICIPFORM_H

#include "../basepage.h"

class StaticIPForm : public BasePage
{
public:
    StaticIPForm();
    ~StaticIPForm();

    // Builds all UI elements for the page
    void BuildUIElements() override;

private slots:
    void OnIPClicked();
    void OnGatewayClicked();
    void OnSubnetClicked();
    void OnApplyClicked();
    void OnClearClicked();

private:
    QLabel* statusLabel = Q_NULLPTR;
    QPair<QPushButton*,QLabel*> ipPair;
    QPair<QPushButton*,QLabel*> gatewayPair;
    QPair<QPushButton*,QLabel*> subnetPair;
    QPushButton* applyButton = Q_NULLPTR;
    QPushButton* clearButton = Q_NULLPTR;
};

#endif // STATICIPFORM_H
