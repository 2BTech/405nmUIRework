#include "modbusbase.h"

ModbusBase::ModbusBase()
{

}

ModbusBase::ModbusBase(ModbusDatatable* datatable)
{
    this->dataTable = datatable;
}

void ModbusBase::SetDatatable(ModbusDatatable* datatable)
{
    this->dataTable = datatable;
}

QString ModbusBase::ErrorString()
{
    return errorString;
}

void ModbusBase::SetUnitID(unsigned char unitID)
{
    this->unitID = unitID;
}

unsigned char ModbusBase::UnitID()
{
    return unitID;
}

void ModbusBase::SetInformation(unsigned char unitID, ModbusDatatable *datatable)
{
    this->unitID = unitID;
    this->dataTable = datatable;
}
