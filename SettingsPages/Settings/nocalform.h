#ifndef NOCALFORM_H
#define NOCALFORM_H

#include "../basesettingspage.h"

class NOCalForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    NOCalForm();
    ~NOCalForm();

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

#endif // NOCALFORM_H
