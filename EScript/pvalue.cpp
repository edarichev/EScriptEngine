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
    return !(lhs < rhs) && !(rhs < lhs);
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
    case SymbolType::String:
        strValue = (StringObject*)rhs;
        break;
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)rhs;
        switch (rec->type) {
        case SymbolType::Boolean:
            boolValue = rec->data ? true : false;
            break;
        case SymbolType::Integer:
            intValue = bit_cast<int64_t>(rec->data);
            break;
        case SymbolType::Real:
            realValue = bit_cast<double>(rec->data);
            break;
        case SymbolType::String:
            strValue = (StringObject*)rec->data;
            break;
        default:
            throw std::domain_error("PValue: not supported type");
        }
        break;
    }
    default:
        throw std::domain_error("PValue: not supported type");
    }
}

PValue::PValue(int64_t rhs)
{
    operator=(rhs);
}

PValue::PValue(uint64_t rhs)
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
    case SymbolType::String:
        strValue = rhs.strValue;
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

PValue &PValue::operator=(uint64_t rhs)
{
    type = SymbolType::Integer;
    intValue = bit_cast<int64_t>(rhs);
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

std::u32string PValue::uString() const
{
    return to_u32string(type, value64());
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
    case SymbolType::String:
        val.strValue = (StringObject*)ptr->data;
        val.type = SymbolType::String;
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
    default:
        break;
    }
    // всё остальное переводим в строку как получится
    switch (op) {
    case ArithmeticOperation::Add: {
        auto str = value1.uString() + value2.uString();
        StringObject *newString = new StringObject(str);
        PValue ret;
        ret.strValue = newString;
        ret.type = SymbolType::String;
        return ret;
    }
    case ArithmeticOperation::BoolLess:
        return PValue(value1.uString() < value2.uString());
    case ArithmeticOperation::BoolLessOrEqual:
        return PValue(value1.uString() <= value2.uString());
    case ArithmeticOperation::BoolGreater:
        return PValue(value1.uString() > value2.uString());
    case ArithmeticOperation::BoolGreaterOrEqual:
        return PValue(value1.uString() >= value2.uString());
    case ArithmeticOperation::BoolEqual:
        return PValue(value1.uString() == value2.uString());
    default:
        break;
    }
    throw std::domain_error("Unsupported binary operation for specified types");
}

void PValue::increment()
{
    switch (type) {
    case SymbolType::Integer:
        intValue++;
        break;
    case SymbolType::Real:
        realValue++;
        break;
    default:
        throw std::domain_error("Unable to increment: not supported type");
    }
}

void PValue::decrement()
{
    switch (type) {
    case SymbolType::Integer:
        intValue--;
        break;
    case SymbolType::Real:
        realValue--;
        break;
    default:
        throw std::domain_error("Unable to decrement: not supported type");
    }
}

PValue PValue::lshift(const PValue &v1, const PValue &v2)
{
    // только целые
    int64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = v1.intValue;
    else
        throw std::domain_error("Only integer numbers are supported: <<");
    int64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = v2.intValue;
    else
        throw std::domain_error("Only integer numbers are supported: <<");
    return PValue(left << right);
}

PValue PValue::rshift(const PValue &v1, const PValue &v2)
{
    // только целые
    int64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = v1.intValue;
    else
        throw std::domain_error("Only integer numbers are supported: >>");
    int64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = v2.intValue;
    else
        throw std::domain_error("Only integer numbers are supported: >>");
    return PValue(left >> right);
}

PValue PValue::rshiftz(const PValue &v1, const PValue &v2)
{
    // только целые
    uint64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = bit_cast<uint64_t>(v1.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: <<");
    uint64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = bit_cast<uint64_t>(v2.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: <<");
    return PValue(left >> right);
}

PValue PValue::bit_and(const PValue &v1, const PValue &v2)
{
    // только целые
    uint64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = bit_cast<uint64_t>(v1.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: bit and");
    uint64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = bit_cast<uint64_t>(v2.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: bit and");
    return PValue(left & right);
}

PValue PValue::bit_or(const PValue &v1, const PValue &v2)
{
    // только целые
    uint64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = bit_cast<uint64_t>(v1.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: bit or");
    uint64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = bit_cast<uint64_t>(v2.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: bit or");
    return PValue(left | right);
}

PValue PValue::bit_xor(const PValue &v1, const PValue &v2)
{
    // только целые
    uint64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = bit_cast<uint64_t>(v1.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: xor");
    uint64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = bit_cast<uint64_t>(v2.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: xor");
    return PValue(left ^ right);
}

PValue PValue::log_and(const PValue &v1, const PValue &v2)
{
    return PValue(v1.asBoolean() && v2.asBoolean());
}

PValue PValue::log_or(const PValue &v1, const PValue &v2)
{
    return PValue(v1.asBoolean() || v2.asBoolean());
}

PValue PValue::mod(const PValue &v1, const PValue &v2)
{
    // только целые
    uint64_t left = 0;
    if (v1.type == SymbolType::Integer)
        left = bit_cast<uint64_t>(v1.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: xor");
    uint64_t right = 0;
    if (v2.type == SymbolType::Integer)
        right = bit_cast<uint64_t>(v2.intValue);
    else
        throw std::domain_error("Only integer numbers are supported: xor");
    return PValue(left % right);
}

} // namespace escript
