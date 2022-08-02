#include "valuehandlerbase.h"

ValueHandlerBase::ValueHandlerBase() : QObject()
{

}

BaseValueObject* ValueHandlerBase::GetSetting(QString marker)
{
    if (markerValueMap.contains(marker))
    {
        return markerValueMap[marker];
    }
    else
    {
        qDebug() << "ERROR: Trying to get unknown setting: " << marker;
        qDebug() << "Keys: " << markerValueMap.keys();
        return Q_NULLPTR;
    }
}

QList<BaseValueObject*> ValueHandlerBase::GetAllValues()
{
    return values;
}

bool ValueHandlerBase::AddValue(BaseValueObject* value)
{
    if (!markerValueMap.contains(value->getMarker()))
    {
        markerValueMap[value->getMarker()] = value;
        return true;
    }
    else
    {
        qDebug() << "Trying to add duplicate setting: " << value->getMarker();
        return false;
    }
}

int ValueHandlerBase::GetRegisterIndex()
{
    return registerIndex;
}

void ValueHandlerBase::SetRegisterIndex(int index)
{
    registerIndex = index;
}

int ValueHandlerBase::GetNumRegisters()
{
    return numRegisters;
}

void ValueHandlerBase::SetNumRegisters(int index)
{
    numRegisters = index;
}

int ValueHandlerBase::GetCoilIndex()
{
    return coilIndex;
}

void ValueHandlerBase::SetCoilIndex(int index)
{
    coilIndex = index;
}
