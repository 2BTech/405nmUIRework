#ifndef BASEVALUEOBJECT_H
#define BASEVALUEOBJECT_H

// Signals and slots
#include <QObject>
// Name
#include <QString>

enum class ValueType {
    UCHAR_VALUE,
    INT_VALUE,
    FLOAT_VALUE,
    STRING_VALUE
};

class BaseValueObject : public QObject
{
    Q_OBJECT
public:
    BaseValueObject(QString name = "unset name", QString marker = "unset marker", QString units = "");

    void setName(QString name);
    QString getName();

    void setMarker(QString marker);
    QString getMarker();

    void setUnits(QString units);
    QString getUnits();

    virtual void setValue_str(QString str) = 0;
    virtual void setValue(BaseValueObject* other) = 0;

    // Used to check if the value in two objects is the same
    virtual bool ValueEquals(BaseValueObject* other) = 0;

    // Makes a copy of this object. Used with settings pages
    virtual BaseValueObject* MakeCopy() = 0;

    // Get a string representation
    virtual QString ToString() = 0;

    virtual ValueType GetValueType() = 0;

signals:
    // Emitted when value is changed
    void ValueChanged();

protected:
    // Name of the value
    QString name;
    // Makrer for the value
    QString marker;
    // Units for the value
    QString units;
};

#endif // BASEVALUEOBJECT_H