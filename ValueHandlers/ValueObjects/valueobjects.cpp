#include "valueobjects.h"

template <>
ValueObject<uchar>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
    bytes = new uchar[1];
}

template <>
ValueObject<int>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
    bytes = new uchar[4];
}

template <>
ValueObject<float>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
    bytes = new uchar[4];
}

template <>
ValueObject<QString>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
    bytes = new uchar[25];
}

template <>
ValueObject<uchar>::~ValueObject()
{
    delete [] bytes;
}

template <>
ValueObject<int>::~ValueObject()
{
    delete [] bytes;
}

template <>
ValueObject<float>::~ValueObject()
{
    delete [] bytes;
}

template <>
ValueObject<QString>::~ValueObject()
{
    delete [] bytes;
}

template <>
void ValueObject<uchar>::setValue(uchar val)
{
    if (val != value)
    {
        bytes[0] = val;
        value = val;
        emit ValueChanged();
    }
}

template <>
void ValueObject<int>::setValue(int val)
{
    if (val != value)
    {
        union {
            int v;
            uchar bytes[4];
        };
        v = val;
        memcpy(this->bytes, bytes, 4);

        value = val;
        emit ValueChanged();
    }
}

template <>
void ValueObject<float>::setValue(float val)
{
    if (val != value)
    {
        union {
            float v;
            uchar bytes[4];
        };
        v = val;
        memcpy(this->bytes, bytes, 4);

        value = val;
        emit ValueChanged();
    }
}

template <>
void ValueObject<QString>::setValue(QString val)
{
    if (val != value)
    {
        int toCpy = val.length();
        // Max size is 25, make sure not to overwrite buffer
        toCpy = toCpy > 25 ? 25 : toCpy;

        // Copy the data into the first part of the buffer
        memcpy(bytes, val.toLatin1().data(), toCpy);
        // Zero the remaining section
        memset(bytes + toCpy, 0, 25 - toCpy);

        value = val;
        emit ValueChanged();
    }
}

template <>
void ValueObject<uchar>::setValue(BaseValueObject* oth)
{
    ValueObject<uchar>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<uchar>*>(oth)) != Q_NULLPTR)
    {
        setValue(other->value);
    }
}

template <>
void ValueObject<int>::setValue(BaseValueObject* oth)
{
    ValueObject<int>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<int>*>(oth)) != Q_NULLPTR)
    {
        setValue(other->value);
    }
}

template <>
void ValueObject<float>::setValue(BaseValueObject* oth)
{
    ValueObject<float>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<float>*>(oth)) != Q_NULLPTR)
    {
        setValue(other->value);
    }
}

template <>
void ValueObject<QString>::setValue(BaseValueObject* oth)
{
    ValueObject<QString>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<QString>*>(oth)) != Q_NULLPTR)
    {
        setValue(other->value);
    }
}

template <>
void ValueObject<uchar>::setValue_str(QString val)
{
    setValue(val.toUInt());
}

template <>
void ValueObject<int>::setValue_str(QString val)
{
    setValue(val.toInt());
}

template <>
void ValueObject<float>::setValue_str(QString val)
{
    setValue(val.toFloat());
}

template <>
void ValueObject<QString>::setValue_str(QString val)
{
    setValue(val);
}

template <>
uchar ValueObject<uchar>::getValue()
{
    return value;
}

template <>
int ValueObject<int>::getValue()
{
    return value;
}

template <>
float ValueObject<float>::getValue()
{
    return value;
}

template <>
QString ValueObject<QString>::getValue()
{
    return value;
}

template <>
bool ValueObject<uchar>::ValueEquals(BaseValueObject* other)
{
    // Cannot check against a null object
    if (other == Q_NULLPTR)
    {
        return false;
    }
    // Cannot check against an object of a different type
    else if (!dynamic_cast<ValueObject<uchar>*>(other))
    {
        return false;
    }
    else
    {
        return dynamic_cast<ValueObject<uchar>*>(other)->value == value;
    }
}

template <>
bool ValueObject<int>::ValueEquals(BaseValueObject* other)
{
    // Cannot check against a null object
    if (other == Q_NULLPTR)
    {
        return false;
    }
    // Cannot check against an object of a different type
    else if (!dynamic_cast<ValueObject<int>*>(other))
    {
        return false;
    }
    else
    {
        return dynamic_cast<ValueObject<int>*>(other)->value == value;
    }
}

template <>
bool ValueObject<float>::ValueEquals(BaseValueObject* other)
{
    // Cannot check against a null object
    if (other == Q_NULLPTR)
    {
        return false;
    }
    // Cannot check against an object of a different type
    else if (!dynamic_cast<ValueObject<float>*>(other))
    {
        return false;
    }
    else
    {
        return dynamic_cast<ValueObject<float>*>(other)->value == value;
    }
}

template <>
bool ValueObject<QString>::ValueEquals(BaseValueObject* other)
{
    // Cannot check against a null object
    if (other == Q_NULLPTR)
    {
        return false;
    }
    // Cannot check against an object of a different type
    else if (!dynamic_cast<ValueObject<QString>*>(other))
    {
        return false;
    }
    else
    {
        return dynamic_cast<ValueObject<QString>*>(other)->value == value;
    }
}

template <>
BaseValueObject* ValueObject<uchar>::MakeCopy()
{
    ValueObject<uchar>* cpy = new ValueObject<uchar>(name, marker, units);
    cpy->setValue(this);
    return cpy;
}

template <>
BaseValueObject* ValueObject<int>::MakeCopy()
{
    ValueObject<int>* cpy = new ValueObject<int>(name, marker, units);
    cpy->setValue(this);
    return cpy;
}

template <>
BaseValueObject* ValueObject<float>::MakeCopy()
{
    ValueObject<float>* cpy = new ValueObject<float>(name, marker, units);
    cpy->setValue(this);
    return cpy;
}

template <>
BaseValueObject* ValueObject<QString>::MakeCopy()
{
    ValueObject<QString>* cpy = new ValueObject<QString>(name, marker, units);
    cpy->setValue(this);
    return cpy;
}

template <>
QString ValueObject<uchar>::ToString()
{
    return QString::number(value);
}

template <>
QString ValueObject<int>::ToString()
{
    return QString::number(value);
}

template <>
QString ValueObject<float>::ToString()
{
    char buffer[25];
    sprintf(buffer, "%1.2f", value);
    return QString(buffer);
}

template <>
QString ValueObject<QString>::ToString()
{
    return value;
}


template <>
ValueType ValueObject<uchar>::GetValueType()
{
    return ValueType::UCHAR_VALUE;
}

template <>
ValueType ValueObject<int>::GetValueType()
{
    return ValueType::INT_VALUE;
}

template <>
ValueType ValueObject<float>::GetValueType()
{
    return ValueType::FLOAT_VALUE;
}

template <>
ValueType ValueObject<QString>::GetValueType()
{
    return ValueType::STRING_VALUE;
}

