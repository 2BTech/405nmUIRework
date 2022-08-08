#include "valueobjects.h"

template <>
ValueObject<uchar>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
//    valueBytes = new uchar[1];
//    valRegisterBytes = new short[1];

//    qDebug() << name << " UCHAR: " << (sizeof(this->valueBytes) / sizeof(this->valueBytes[0])) << " : " << (sizeof(this->valRegisterBytes) / sizeof(this->valRegisterBytes[0]));
//    qDebug() << valueBytes << " - " << valRegisterBytes;

    memset(valueBytes, 0, 25);
    memset(valRegisterBytes, 0, 25);
}

template <>
ValueObject<int>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
//    valueBytes = new uchar[4];
//    valRegisterBytes = new short[2];
//    qDebug() << name << " INT: " << (sizeof(this->valueBytes) / sizeof(this->valueBytes[0])) << " : " << (sizeof(this->valRegisterBytes) / sizeof(this->valRegisterBytes[0]));
//    qDebug() << valueBytes << " - " << valRegisterBytes;
    memset(valueBytes, 0, 25);
    memset(valRegisterBytes, 0, 25);
}

template <>
ValueObject<float>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
//    valueBytes = new uchar[4];
//    valRegisterBytes = new short[2];
//    qDebug() << name << " FLOAT: " << (sizeof(this->valueBytes) / sizeof(this->valueBytes[0])) << " : " << (sizeof(this->valRegisterBytes) / sizeof(this->valRegisterBytes[0]));
//    qDebug() << valueBytes << " - " << valRegisterBytes;
    memset(valueBytes, 0, 25);
    memset(valRegisterBytes, 0, 25);
}

template <>
ValueObject<QString>::ValueObject(QString name, QString marker, QString units) : BaseValueObject(name, marker, units)
{
//    valueBytes = new uchar[25];
//    valRegisterBytes = new short[2];
//    qDebug() << name << " QSTRING: " << (sizeof(this->valueBytes) / sizeof(this->valueBytes[0])) << " : " << (sizeof(this->valRegisterBytes) / sizeof(this->valRegisterBytes[0]));
//    qDebug() << valueBytes << " - " << valRegisterBytes;
    memset(valueBytes, 0, 25);
    memset(valRegisterBytes, 0, 25);
}

template <>
ValueObject<uchar>::~ValueObject()
{
//    delete [] valueBytes;
//    delete [] valRegisterBytes;
}

template <>
ValueObject<int>::~ValueObject()
{
//    delete [] valueBytes;
//    delete [] valRegisterBytes;
}

template <>
ValueObject<float>::~ValueObject()
{
//    delete [] valueBytes;
//    delete [] valRegisterBytes;
}

template <>
ValueObject<QString>::~ValueObject()
{
//    delete [] valueBytes;
//    delete [] valRegisterBytes;
}

template <>
void ValueObject<uchar>::setValue(uchar val)
{
    bool diff = val != curValue;

    mutex.lock();
    if (diff)
    {
        valueBytes[0] = val;
        valRegisterBytes[0] = curValue;
        curValue = val;
    }
    valHistory.append(val);
    mutex.unlock();

    if (diff)
    {
        emit ValueChanged();
    }
    emit SetValue();
}

template <>
void ValueObject<int>::setValue(int val)
{
    bool diff = val != curValue;
    mutex.lock();
    if (diff)
    {
        union {
            int v;
            uchar bytes[4];
            short rBytes[2];
        };
        v = val;
        memcpy(this->valueBytes, bytes, 4);
        memcpy(valRegisterBytes, rBytes, 2);

        curValue = val;
    }
    valHistory.append(val);
    mutex.unlock();

    if (diff)
    {
        emit ValueChanged();
    }
    emit SetValue();
}

template <>
void ValueObject<float>::setValue(float val)
{
    bool diff = val != curValue;
    mutex.lock();
    if (diff)
    {
        union {
            float v;
            uchar bytes[4];
            short rBytes[2];
        };
        v = val;
        memcpy(this->valueBytes, bytes, 4);
        memcpy(valRegisterBytes, rBytes, 2);

        curValue = val;
    }
    valHistory.append(val);
    mutex.unlock();

    if (diff)
    {
        emit ValueChanged();
    }
    emit SetValue();
}

template <>
void ValueObject<QString>::setValue(QString val)
{
//    qDebug() << "Started setting " << name << " with " << val;
    mutex.lock();
//    qDebug() << "Passed mutex";

    bool diff = val != curValue;

    if (diff)
    {
//        qDebug() << "Name: " << name;
//        qDebug() << "Value bytes are null: " << (valueBytes == Q_NULLPTR);
//        qDebug() << "Val bytes len: " << (sizeof(valueBytes) / sizeof(valueBytes[0]));
//        qDebug() << "Register bytes are null: " << (valRegisterBytes == Q_NULLPTR);
//        qDebug() << "Reg bytes len: " << (sizeof(valRegisterBytes) / sizeof(valRegisterBytes[0]));

        int toCpy = val.length();
        // Max size is 25, make sure not to overwrite buffer
        toCpy = toCpy > 25 ? 25 : toCpy;

        //qDebug() << "ToCpy: " << toCpy;

//        // Copy the data into the first part of the buffer
        memcpy(valueBytes, val.toLatin1().data(), toCpy);

        // Zero the remaining section
        memset(valueBytes + toCpy, 0, 25 - toCpy);

        //qDebug() << "ModbusValues: " << val;
        for (int i = 0; i < 25; i++)
        {
            valRegisterBytes[i] = valueBytes[i];
            //qDebug() << "Setting: " << registerIndex + i << static_cast<char>(valRegisterBytes[i]);
        }

        curValue = val;
    }
    valHistory.append(val);
    mutex.unlock();

    if (diff)
    {
        emit ValueChanged();
    }
    emit SetValue();
}

template <>
void ValueObject<uchar>::setValue(BaseValueObject* oth)
{
    ValueObject<uchar>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<uchar>*>(oth)) != Q_NULLPTR)
    {
        other->Mutex()->lock();
        setValue(other->curValue);
        other->Mutex()->unlock();
    }
}

template <>
void ValueObject<int>::setValue(BaseValueObject* oth)
{
    ValueObject<int>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<int>*>(oth)) != Q_NULLPTR)
    {
        other->Mutex()->lock();
        setValue(other->curValue);
        other->Mutex()->unlock();
    }
}

template <>
void ValueObject<float>::setValue(BaseValueObject* oth)
{
    ValueObject<float>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<float>*>(oth)) != Q_NULLPTR)
    {
        other->Mutex()->lock();
        setValue(other->curValue);
        other->Mutex()->unlock();
    }
}

template <>
void ValueObject<QString>::setValue(BaseValueObject* oth)
{
    ValueObject<QString>* other = Q_NULLPTR;
    if (oth != Q_NULLPTR && (other = dynamic_cast<ValueObject<QString>*>(oth)) != Q_NULLPTR)
    {
        other->Mutex()->lock();
        setValue(other->curValue);
        other->Mutex()->unlock();
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
    //qDebug() << "Setting " << name << " with string";
    setValue(val);
    //qDebug() << "Finished setting " << name << " with string";
}

template <>
uchar ValueObject<uchar>::getValue()
{
    uchar temp;
    mutex.lock();
    temp = curValue;
    mutex.unlock();
    return temp;

    //return temp;
}

template <>
int ValueObject<int>::getValue()
{
    int temp;
    mutex.lock();
    temp = curValue;
    mutex.unlock();
    return temp;
    //return value;
}

template <>
float ValueObject<float>::getValue()
{
    float temp;
    mutex.lock();
    temp = curValue;
    mutex.unlock();
    return temp;
    //return value;
}

template <>
QString ValueObject<QString>::getValue()
{
    QString temp;
    mutex.lock();
    temp = curValue;
    mutex.unlock();
    return temp;
    //return value;
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
        return dynamic_cast<ValueObject<uchar>*>(other)->curValue == curValue;
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
        return dynamic_cast<ValueObject<int>*>(other)->curValue == curValue;
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
        return dynamic_cast<ValueObject<float>*>(other)->curValue == curValue;
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
        return dynamic_cast<ValueObject<QString>*>(other)->curValue == curValue;
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
    cpy->SetNumSigFigs(numSigFigs);
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
    QString temp;
    mutex.lock();
    temp = QString::number(curValue);
    mutex.unlock();

    return temp;
}

template <>
QString ValueObject<int>::ToString()
{
    QString temp;
    mutex.lock();
    temp = QString::number(curValue);
    mutex.unlock();

    return temp;
}

template <>
QString ValueObject<float>::ToString()
{
    char buffer[25];
    mutex.lock();
    sprintf(buffer, QString("%1.").append(QString::number(numSigFigs)).append('f').toLatin1(), curValue);
    mutex.unlock();
    return QString(buffer);
}

template <>
QString ValueObject<QString>::ToString()
{
    QString temp;
    mutex.lock();
    temp = curValue;
    mutex.unlock();

    return temp;
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

template <>
int ValueObject<uchar>::GetNumRegisters()
{
    return 1;
}

template <>
int ValueObject<int>::GetNumRegisters()
{
    return 2;
}

template <>
int ValueObject<float>::GetNumRegisters()
{
    return 2;
}

template <>
int ValueObject<QString>::GetNumRegisters()
{
    return 25;
}

template <>
short* ValueObject<uchar>::GetRegisterValues()
{
    return valRegisterBytes;
}

template <>
short* ValueObject<int>::GetRegisterValues()
{
    return valRegisterBytes;
}

template <>
short* ValueObject<float>::GetRegisterValues()
{
    return valRegisterBytes;
}

template <>
short* ValueObject<QString>::GetRegisterValues()
{
    return valRegisterBytes;
}

template <>
uchar* ValueObject<uchar>::GetBytes()
{
    return valueBytes;
}

template <>
uchar* ValueObject<int>::GetBytes()
{
    return valueBytes;
}

template <>
uchar* ValueObject<float>::GetBytes()
{
    return valueBytes;
}

template <>
uchar* ValueObject<QString>::GetBytes()
{
    return valueBytes;
}



