#include "basevalueobject.h"

BaseValueObject::BaseValueObject(QString name, QString marker, QString units)
{
    this->name = name;
    this->marker = marker;
    this->units = units;
}

void BaseValueObject::setName(QString name)
{
    this->name = name;
}

QString BaseValueObject::getName()
{
    return name;
}

void BaseValueObject::setMarker(QString marker)
{
    this->marker = marker;
}

QString BaseValueObject::getMarker()
{
    return marker;
}

void BaseValueObject::setUnits(QString units)
{
    this->units = units;
}

QString BaseValueObject::getUnits()
{
    return units;
}

int BaseValueObject::GetRegisterIndex()
{
    return registerIndex;
}

void BaseValueObject::SetRegisterIndex(int index)
{
    registerIndex = index;
    emit SetValue();
}
