#ifndef FLOWCALFORM_H
#define FLOWCALFORM_H

#include "../basesettingspage.h"

class FlowCalForm : public BaseSettingsPage
{
public:
    FlowCalForm();
    ~FlowCalForm();

    void BuildUIElements() override;

private slots:
    void OnCellClicked();
    void OnOzoneClicked();

    void ReceiveCellFlow(QString val);
    void ReceiveOzoneFlow(QString val);

private:
    QPair<QPushButton*,QLabel*> cellPair;
    QPair<QPushButton*,QLabel*> ozonePair;

    void UpdateUI() override;
};

#endif // FLOWCALFORM_H
