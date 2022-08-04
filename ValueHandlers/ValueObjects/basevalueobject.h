#ifndef BASEVALUEOBJECT_H
#define BASEVALUEOBJECT_H

// Signals and slots
#include <QObject>
// Name
#include <QString>
#include <QMutex>

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

    int GetRegisterIndex();
    void SetRegisterIndex(int index);
    virtual int GetNumRegisters() = 0;
    virtual uchar* GetBytes() = 0;
    virtual short* GetRegisterValues() = 0;

    QMutex* Mutex()
    {
        return &mutex;
    }

signals:
    // Emitted when value is changed
    void ValueChanged();
    void SetValue();

protected:
    // Name of the value
    QString name;
    // Makrer for the value
    QString marker;
    // Units for the value
    QString units;
    // First modbus register for the value
    int registerIndex = -1;

    QMutex mutex;
};

#endif // BASEVALUEOBJECT_H
