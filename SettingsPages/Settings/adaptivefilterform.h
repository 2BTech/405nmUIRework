#ifndef ADAPTIVEFILTERFORM_H
#define ADAPTIVEFILTERFORM_H

#include "../basesettingspage.h"

class AdaptiveFilterForm : public BaseSettingsPage
{
    Q_OBJECT
public:
    AdaptiveFilterForm();
    ~AdaptiveFilterForm();

    void BuildUIElements() override;

private slots:
    void OnShortClicked();
    void OnLongClicked();
    void OnDiffClicked();
    void OnPerClicked();

    void ReceiveNewShort(QString val);
    void ReceiveNewLong(QString val);
    void ReceiveNewDiff(QString val);
    void ReceiveNewPer(QString val);

private:
    QPair<QPushButton*,QLabel*> shortPair;
    QPair<QPushButton*,QLabel*> longPair;
    QPair<QPushButton*,QLabel*> diffPair;
    QPair<QPushButton*,QLabel*> perPair;

    void UpdateUI() override;
};

#endif // ADAPTIVEFILTERFORM_H
