#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include "../basepage.h"
#include "GlobalDefinitions.h"

class AboutPage : public BasePage
{
public:
    AboutPage();
    ~AboutPage();

    void BuildUIElements() override;

private:
    QLabel* buildLabel = Q_NULLPTR;
    QLabel* dateLabel = Q_NULLPTR;
    QLabel* supportLabel = Q_NULLPTR;
    QLabel* baudLabel = Q_NULLPTR;
};

#endif // ABOUTPAGE_H
