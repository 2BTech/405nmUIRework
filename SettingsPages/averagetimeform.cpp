#include "averagetimeform.h"

AverageTimeForm::AverageTimeForm() : BaseSettingsPage("Averaging Time")
{

}

AverageTimeForm::~AverageTimeForm()
{
    if (avgTimeSelector)
    {
        avgTimeSelector->deleteLater();
        avgTimeSelector = Q_NULLPTR;
    }
}

void AverageTimeForm::BuildUIElements()
{
    BaseSettingsPage::BuildUIElements();

    avgTimeSelector = new QComboBox(this);
    avgTimeSelector->setGeometry(150, 130, 180, 40);
    avgTimeSelector->addItems(QStringList() << "Averaging Time" << "5 Seconds" << "1 Minute" << "5 Minutes" << "1 Hour");
}
