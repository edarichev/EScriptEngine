/**
 * @file pvalue.h
 * @brief Структура для упаковки тип+значение (как variant)
 */
#ifndef PVALUE_H
#define PVALUE_H

#include "EScript_global.h"
#include "types.h"
#include "storage.h"

namespace escript {

/**
 * @brief Арифметические операции, строго с 0, для конвертирования в
 *        OperationType или Opcode используйте массив
 */
enum class ArithmeticOperation : uint8_t
{
    Add = 0,
    Sub = 1,
    Mul = 2,
    Div = 3
};

struct PValue;

// для строк и неприводимых типов - другая функция
template<typename T1, typename T2>
decltype(auto) calcValues(T1 v1, T2 v2, ArithmeticOperation op)
{
    switch (op) {
    case ArithmeticOperation::Add:
        return PValue(v1 + v2);
    case ArithmeticOperation::Mul:
        return PValue(v1 * v2);
    case ArithmeticOperation::Div:
        return PValue(v1 / v2);
    case ArithmeticOperation::Sub:
        return PValue(v1 - v2);
    default:
        throw std::domain_error("Unsupported bin.op");
    }
}

/**
 * @brief Структура для упаковки тип+значение (как variant)
 */
struct PValue
{
    SymbolType type { SymbolType::Undefined };
    union {
        int64_t intValue { 0 };
        double realValue;
        bool boolValue;
    };
    PValue() = default;
    explicit PValue(int64_t rhs)
    {
        operator=(rhs);
    }
    explicit PValue(bool rhs)
    {
        operator=(rhs);
    }
    explicit PValue(double rhs)
    {
        operator=(rhs);
    }
    PValue &operator=(int64_t rhs)
    {
        type = SymbolType::Integer;
        intValue = rhs;
        return *this;
    }
    PValue &operator=(bool rhs)
    {
        type = SymbolType::Boolean;
        boolValue = rhs;
        return *this;
    }
    PValue &operator=(double rhs)
    {
        type = SymbolType::Real;
        realValue = rhs;
        return *this;
    }
    /**
     * @brief Возвращает значение как uint64_t
     * @return
     */
    uint64_t value64() const
    {
        switch (type) {
        case SymbolType::Integer:
            return bit_cast<uint64_t>(intValue);
        case SymbolType::Real:
            return bit_cast<uint64_t>(realValue);
        case SymbolType::Boolean:
            return boolValue ? 1 : 0;
        default:
            throw std::domain_error("unsupproted type by PValue");
        }
    }

    static PValue getValue(ObjectRecord *ptr)
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

    static PValue getValue(const std::pair<SymbolType, uint64_t> &item)
    {
        PValue val;
        switch (item.first) {
        // тип переменной
        case SymbolType::Integer:
            val = bit_cast<int64_t>(item.second);
            break;
        case SymbolType::Real:
            val = bit_cast<double>(item.second);
            break;
        case SymbolType::Boolean:
            val = item.second ? true : false;
            break;
        case SymbolType::Variable:
            return getValue((ObjectRecord*)item.second);
        default:
            throw std::domain_error("Unsupported type: getValue(pair)");
        }
        return val;
    }

    static PValue binaryOpValues(const PValue &value1, const PValue &value2,
                                 ArithmeticOperation op)
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
        throw std::domain_error("Unsupported type");
    }
};

} // namespace escript

#endif // PVALUE_H
