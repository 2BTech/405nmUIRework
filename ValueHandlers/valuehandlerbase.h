#ifndef VALUEHANDLERBASE_H
#define VALUEHANDLERBASE_H

#include <QMap>
#include <QObject>
#include <QDebug>

#include "ValueObjects/basevalueobject.h"
#include "ValueObjects/valueobjects.h"
#include "GlobalDefinitions.h"

class ValueHandlerBase : public QObject
{
    Q_OBJECT
public:
    ValueHandlerBase();

    BaseValueObject* GetSetting(QString marker);
    QList<BaseValueObject*> GetAllValues();

    int GetRegisterIndex();
    void SetRegisterIndex(int index);

    int GetNumRegisters();
    void SetNumRegisters(int index);

    int GetCoilIndex();
    void SetCoilIndex(int index);

signals:
    void SetModbusCoil(int index);

protected:
    QMap<QString, BaseValueObject*> markerValueMap;
    QList<BaseValueObject*> values;

    int registerIndex = -1;
    int numRegisters = 0;
    int coilIndex = -1;

    virtual void BuildObjects() = 0;

    bool AddValue(BaseValueObject* values);
};

#endif // VALUEHANDLERBASE_H
