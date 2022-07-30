#include "aboutpage.h"

AboutPage::AboutPage() : BasePage("About")
{

}

AboutPage::~AboutPage()
{
    if (buildLabel)
    {
        buildLabel->deleteLater();
        dateLabel->deleteLater();
        supportLabel->deleteLater();
        baudLabel->deleteLater();

        buildLabel = dateLabel = supportLabel = baudLabel = Q_NULLPTR;
    }
}

void AboutPage::BuildUIElements()
{
    BasePage::BuildUIElements();

    QFont font;
    font.setPointSize(12);

    buildLabel = new QLabel(this);
    buildLabel->setGeometry(70, 70, 340, 40);
    buildLabel->setText(QString("Build Version: ").append(BUILD_VER));
    buildLabel->setFont(font);

    dateLabel = new QLabel(this);
    dateLabel->setGeometry(70, 110, 340, 40);
    dateLabel->setText(QString("Build date: ").append(BUILD_DATE));
    dateLabel->setFont(font);

    supportLabel = new QLabel(this);
    supportLabel->setGeometry(70, 150, 340, 40);
    supportLabel->setText("Support: https://twobtech.com");
    supportLabel->setFont(font);

    baudLabel = new QLabel(this);
    baudLabel->setGeometry(70, 190, 340, 40);
    baudLabel->setText("Baud Rate: 2400");
    baudLabel->setFont(font);
}
