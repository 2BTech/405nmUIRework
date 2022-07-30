#ifndef NO2CALFORM_H
#define NO2CALFORM_H

#include "../basesettingspage.h"

class NO2CalForm : public BaseSettingsPage
{
public:
    NO2CalForm();
    ~NO2CalForm();

    void BuildUIElements() override;

    void UpdateUI() override;

private slots:
    void OnSlopeClicked();
    void OnZeroClicked();
    void OnAnalogClicked();

    void ReceiveSlope(QString val);
    void ReceiveZero(QString val);
    void ReceiveAnalog(QString val);

private:
    QPair<QPushButton*,QLabel*> slopePair;
    QPair<QPushButton*,QLabel*> zeroPair;
    QPair<QPushButton*,QLabel*> analogPair;
};

#endif // NO2CALFORM_H
