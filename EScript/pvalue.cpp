#include "stdafx.h"
#include "pvalue.h"
#include "stringobject.h"

namespace escript {

bool operator<(const PValue &lhs, const PValue &rhs)
{
    switch (lhs.type) {
    case SymbolType::Boolean:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return lhs.boolValue < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.boolValue < rhs.intValue ? true : false;
        case SymbolType::Real:
            return lhs.boolValue < rhs.realValue ? true : false;
        default:
            return false;
        }
        break;
    case SymbolType::Integer:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return (bool)lhs.intValue < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.intValue < rhs.intValue;
        case SymbolType::Real:
            return lhs.intValue < rhs.realValue;
        default:
            return false;
        }
        break;
    case SymbolType::Real:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return lhs.realValue ? 1 : 0 < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.realValue < rhs.intValue;
        case SymbolType::Real:
            return lhs.realValue < rhs.realValue;
        default:
            return false;
        }
        break;
    default:
        return false;
    }
    return false;
}

bool operator<=(const PValue &lhs, const PValue &rhs)
{
    return !(operator<(rhs, lhs));
}

bool operator==(const PValue &lhs, const PValue &rhs)
{
    return !((lhs < rhs) && !(rhs < lhs));
}

bool operator>(const PValue &lhs, const PValue &rhs)
{
    return rhs < lhs;
}

bool operator>=(const PValue &lhs, const PValue &rhs)
{
    return !(lhs < rhs);
}

PValue::PValue()
{

}

PValue::PValue(const PValue &rhs)
{
    operator=(rhs);
}

PValue::PValue(SymbolType t, int64_t rhs)
{
    type = t;
    switch (t) {
    case SymbolType::Boolean:
        boolValue = rhs ? true : false;
        break;
    case SymbolType::Integer:
        intValue = bit_cast<int64_t>(rhs);
        break;
    case SymbolType::Real:
        realValue = bit_cast<double>(rhs);
        break;
    default:
        throw std::domain_error("PValue: not supported type");
    }
}

PValue::PValue(int64_t rhs)
{
    operator=(rhs);
}

PValue::PValue(int rhs)
{
    operator=(rhs);
}

PValue::PValue(bool rhs)
{
    operator=((bool)rhs);
}

PValue::PValue(double rhs)
{
    operator=(rhs);
}

PValue &PValue::operator=(const PValue &rhs)
{
    type = rhs.type;
    switch (type) {
    case SymbolType::Boolean:
        boolValue = rhs.boolValue;
        break;
    case SymbolType::Integer:
        intValue = rhs.intValue;
        break;
    case SymbolType::Real:
        realValue = rhs.realValue;
        break;
    default:
        break;
    }
    return *this;
}

PValue &PValue::operator=(int64_t rhs)
{
    type = SymbolType::Integer;
    intValue = rhs;
    return *this;
}

PValue &PValue::operator=(int rhs)
{
    type = SymbolType::Integer;
    intValue = rhs;
    return *this;
}

PValue &PValue::operator=(bool rhs)
{
    type = SymbolType::Boolean;
    boolValue = rhs;
    return *this;
}

PValue &PValue::operator=(double rhs)
{
    type = SymbolType::Real;
    realValue = rhs;
    return *this;
}

bool PValue::asBoolean() const
{
    switch (type) {
    case SymbolType::Boolean:
        return boolValue;
    case SymbolType::Integer:
        return (bool)intValue;
    case SymbolType::Real:
        return (bool)realValue;
    default:
        return false;
    }
}

uint64_t PValue::value64() const
{
    switch (type) {
    case SymbolType::Integer:
        return bit_cast<uint64_t>(intValue);
    case SymbolType::Real:
        return bit_cast<uint64_t>(realValue);
    case SymbolType::Boolean:
        return boolValue ? 1 : 0;
    case SymbolType::String:
        return bit_cast<uint64_t>(strValue);
    default:
        throw std::domain_error("unsupproted type by PValue");
    }
}

PValue PValue::getValue(ObjectRecord *ptr)
{
    PValue val;
    switch (ptr->type) {
    case SymbolType::Integer:
        val = bit_cast<int64_t>(ptr->data);
        break;
    case SymbolType::Real:
        val = bit_cast<double>(ptr->data);
        break;
    case SymbolType::Boolean:
        val = ptr->data ? true : false;
        break;
    default:
        throw std::domain_error("Unsupported type: getValue");
    }
    return val;
}

PValue PValue::getValue(const StackValue &item)
{
    PValue val;
    switch (item.type) {
    // тип переменной
    case SymbolType::Integer:
        val = bit_cast<int64_t>(item.value);
        break;
    case SymbolType::Real:
        val = bit_cast<double>(item.value);
        break;
    case SymbolType::Boolean:
        val = item.value ? true : false;
        break;
    case SymbolType::Variable:
        return getValue((ObjectRecord*)item.value);
    case SymbolType::String:
        val.strValue = (StringObject*)item.value;
        val.type = SymbolType::String;
        break;
    default:
        throw std::domain_error("Unsupported type: getValue(pair)");
    }
    return val;
}

PValue PValue::binaryOpValues(const PValue &value1, const PValue &value2, ArithmeticOperation op)
{
    // в арифметических действиях true/false рассматриваем как целые числа 1, 0
    // и результат переводим в целое
    switch (value1.type) {
    case SymbolType::Integer:
        switch (value2.type) {
        case SymbolType::Integer:
            return calcValues(value1.intValue, value2.intValue, op);
        case SymbolType::Real:
            return calcValues(value1.intValue, value2.realValue, op);
        case SymbolType::Boolean:
            return calcValues(value1.intValue, (IntType)(value2.boolValue ? 1 : 0), op);
        default:
            break;
        }
        break;
    case SymbolType::Real:
        switch (value2.type) {
        case SymbolType::Integer:
            return calcValues(value1.realValue, value2.intValue, op);
        case SymbolType::Real:
            return calcValues(value1.realValue, value2.realValue, op);
        case SymbolType::Boolean:
            return calcValues(value1.realValue, (RealType)(value2.boolValue ? 1 : 0), op);
        default:
            break;
        }
        break;
    case SymbolType::Boolean:
        switch (value2.type) {
        case SymbolType::Integer:
            return calcValues(value1.boolValue ? 1 : 0, value2.intValue, op);
        case SymbolType::Real:
            return calcValues(value1.boolValue ? 1.0 : 0.0, value2.realValue, op);
        case SymbolType::Boolean:
            return calcValues((IntType)value1.boolValue, (IntType)value2.boolValue, op);
        default:
            break;
        }
        break;
    case SymbolType::String:
        switch (value2.type) {
        case SymbolType::String: {
            StringObject *newString = StringObject::concat(value1.strValue, value2.strValue);
            // как установить эту строку в таблицу строк?
            // возвращаем это значение, а вызывающая функция установит его,
            // в таблицу если результат - строка (т.к. при каждой конверсии
            // будет новая строка),
            PValue ret;
            ret.strValue = newString;
            ret.type = SymbolType::String;
            return ret;
        }
        default:
            break;
        }
        break;
    default:
        break;
    }
    throw std::domain_error("Unsupported type");
}


} // namespace escript
