#ifndef DATASELECTFORM_H
#define DATASELECTFORM_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "ValueHandlers/ValueObjects/valueobjects.h"
#include "valuegraphsettings.h"

class DataSelectForm : public QWidget
{
    Q_OBJECT
public:
    DataSelectForm();
    ~DataSelectForm();

    void BuildUIElements(QMap<BaseValueObject*, ValueGraphSettings*> valueObjects);

signals:
    void UpdateGraph();
    void CloseForm();

private slots:
    // This function is triggered when the user selects a new variable from varialbleSelector. Updates
    // the other fields with the values from the newly selected graphing object.
    void OnVariablesIndexChange(int index);

    // Apply new settings to graph object
    void OnApplyClicked();
    // Close this form and re-open the graph form
    void OnCloseClicked();

private:
    QMap<BaseValueObject*, ValueGraphSettings*> valueObjects;

    QLabel* title = Q_NULLPTR;

    // Combobox used to select different variables
    QComboBox* variableSelect = Q_NULLPTR;
    // Combobox used to select dfferent colors
    QComboBox* colorSelect = Q_NULLPTR;
    // Checkbox used to determine if the value should be displayed
    QCheckBox* displayCheck = Q_NULLPTR;

    QPushButton* saveButton = Q_NULLPTR;
    QPushButton* cancelButton = Q_NULLPTR;

    // Converts a color to the index of the color in colorSelect.
    int GetIndexFromColor(Qt::GlobalColor color);
    // Converts the index to the Qt::GloalColor it represents
    Qt::GlobalColor GetColorFromIndex(int index);

    void showEvent(QShowEvent* event) override;
};

#endif // DATASELECTFORM_H
