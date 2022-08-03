#ifndef RANGEFORM_H
#define RANGEFORM_H

// Used to toggle which axis should be auto scaled
#include <QCheckBox>
// Used to notify the user of changes
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "SettingsPages/Utility/numberpadform.h"

class RangeForm : public QWidget
{
    Q_OBJECT
public:
    RangeForm();
    ~RangeForm();

    void SetInformation(QDateTime* pMinX, QDateTime* pMaxX, double *pMinY, double* pMaxY, bool* pAutoMinX, bool* pAutoMaxX, bool* pAutoMinY, bool* pAutoMaxY);

signals:
    void UpdateGraph();
    void CloseForm();

private slots:
    void OnMinXClicked();
    void ReceiveMinX(QString val);

    void OnMaxXClicked();
    void ReceiveMaxX(QString val);

    void OnMinYClicked();
    void ReceiveMinY(QString val);

    void OnMaxYClicked();
    void ReceiveMaxY(QString val);

    void OnToggleClicked();
    void OnToggleAllClicked();

    void OnSaveClicked();
    void OnCancelClicked();

private:
    QLabel* title = Q_NULLPTR;

    QLabel* minXLabel = Q_NULLPTR;
    QPushButton* minXMask = Q_NULLPTR;

    QLabel* maxXLabel = Q_NULLPTR;
    QPushButton* maxXMask = Q_NULLPTR;

    QLabel* minYLabel = Q_NULLPTR;
    QPushButton* minYMask = Q_NULLPTR;

    QLabel* maxYLabel = Q_NULLPTR;
    QPushButton* maxYMask = Q_NULLPTR;

    QCheckBox* autoMinX = Q_NULLPTR;
    QCheckBox* autoMaxX = Q_NULLPTR;
    QCheckBox* autoMinY = Q_NULLPTR;
    QCheckBox* autoMaxY = Q_NULLPTR;
    QCheckBox* autoAll = Q_NULLPTR;

    QList<QCheckBox*> toggles;

    QPushButton* saveButton = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;

    bool isGettingSetting = false;

    QDateTime* pMinX = Q_NULLPTR;
    QDateTime* pMaxX = Q_NULLPTR;
    double* pMinY = Q_NULLPTR;
    double* pMaxY = Q_NULLPTR;

    QDateTime tempMinX;
    QDateTime tempMaxX;
    double tempMinY = 0;
    double tempMaxY = 0;

    bool* pAutoMinX = Q_NULLPTR;
    bool* pAutoMaxX = Q_NULLPTR;
    bool* pAutoMinY = Q_NULLPTR;
    bool* pAutoMaxY = Q_NULLPTR;

    void BuildUIElements();

    void UpdateUI();

    void showEvent(QShowEvent* event) override;
};

#endif // RANGEFORM_H
