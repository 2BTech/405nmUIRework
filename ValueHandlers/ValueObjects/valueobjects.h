#ifndef VALUEOBJECTS_H
#define VALUEOBJECTS_H

#include "basevalueobject.h"

template <typename T>
class ValueObject : public BaseValueObject
{
public:
    ValueObject(QString name = "unset name", QString marker = "unset marker", QString units = "");
    ~ValueObject();

    // Set the value of the object from a string
    void setValue_str(QString val) override;
    // Set the value of the object
    void setValue(T value);
    // Sets the value of the object from another object
    void setValue(BaseValueObject* other) override;
    // Get the value of the object
    T getValue();

    bool ValueEquals(BaseValueObject* other) override;

    // Makes a copy of this object. Used with settings pages
    BaseValueObject* MakeCopy() override;

    // Get a string representation
     QString ToString() override;

     ValueType GetValueType() override;

protected:
    // Current value for the object
    T value = T();
    // Bytes that make up value
    uchar* bytes = Q_NULLPTR;
};

#endif // VALUEOBJECTS_H
