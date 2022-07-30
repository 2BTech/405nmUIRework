#include "flowcalform.h"

FlowCalForm::FlowCalForm() : BaseSettingsPage("Flow Calibration")
{

}

FlowCalForm::~FlowCalForm()
{

}

void FlowCalForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    QFont font;
    font.setPointSize(15);

    cellPair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("N"), QRect(100, 90, 280, 50));
    cellPair.second->setText("Cell Flow Slope: 0.00");
    cellPair.second->setFont(font);
    connect(cellPair.first, &QPushButton::clicked, this, &FlowCalForm::OnCellClicked);

    ozonePair = AddLabelSetting(SettingsHandler::GetInstance()->GetSetting("M"), QRect(100, 160, 280, 50));
    ozonePair.second->setText("Ozone Flow Slope: 0.00");
    ozonePair.second->setFont(font);
    connect(ozonePair.first, &QPushButton::clicked, this, &FlowCalForm::OnOzoneClicked);
}

void FlowCalForm::OnCellClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &FlowCalForm::ReceiveCellFlow);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.00);
}

void FlowCalForm::ReceiveCellFlow(QString val)
{
    pageSettings[0].second->setValue_str(val);
    UpdateUI();
}

void FlowCalForm::OnOzoneClicked()
{
    isGettingSettings = true;
    connect(NumberPadForm::GetInstance(), &NumberPadForm::Finished, this, &FlowCalForm::ReceiveOzoneFlow);
    NumberPadForm::GetInstance()->Start(this, NumberPadMode::GET_FLOAT, 0.5, 3.00);
}

void FlowCalForm::ReceiveOzoneFlow(QString val)
{
    pageSettings[1].second->setValue_str(val);
    UpdateUI();
}

void FlowCalForm::UpdateUI()
{
    cellPair.second->setText(pageSettings[0].second->getName() + ": " + pageSettings[0].second->ToString());
    ozonePair.second->setText(pageSettings[1].second->getName() + ": " + pageSettings[1].second->ToString());
}
