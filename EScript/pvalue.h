/**
 * @file pvalue.h
 * @brief Структура для упаковки тип+значение (как variant)
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#ifndef PVALUE_H
#define PVALUE_H

#include "EScript_global.h"
#include "types.h"
#include "storage.h"
#include "arrayobject.h"
#include "stackvalue.h"

namespace escript {

/**
 * @brief Арифметические операции, строго с 0, для конвертирования в
 *        OperationType или OpCode используйте массив
 */
enum class ArithmeticOperation : uint8_t
{
    Add = 0,
    Sub = 1,
    Mul = 2,
    Div = 3,
    BoolLess = 4,
    BoolLessOrEqual = 5,
    BoolGreater = 6,
    BoolGreaterOrEqual = 7,
    BoolEqual = 8,
    LShift = 9,
    RShift = 10,
    RShiftZero = 11,
    BITAND = 12,
    BITOR = 13,
    BITXOR = 14,
    LOGAND = 15,
    LOGOR = 16,
    Mod = 17,
    BoolNotEqual = 18,
};

class StringObject;
class Function;

/**
 * @brief Структура для упаковки тип+значение (как variant)
 *        и выполнения арифметических и прочих операций.
 * @details
 * Предназначена для облегчения вычислений.
 * Структура сама извлекает либо хранимое
 * в ObjectRecord значение, либо вытаскивает его из переменной, если
 * запись указывает на переменную.
 *
 * Была идея соединить её со StackValue, но получится чудовище.
 * Пусть StackValue занимается стеком, а PValue - вычислениями.
 */
struct PValue
{
    SymbolType type { SymbolType::Null };
    union {
        int64_t intValue { 0 };
        double realValue;
        bool boolValue;
        StringObject *strValue;
        Array *arrValue;
        Function *function;
    };
    PValue();
    PValue(const PValue &rhs) = default;
    explicit PValue(SymbolType t, int64_t rhs);
    explicit PValue(int64_t rhs);
    explicit PValue(uint64_t rhs);
    explicit PValue(int rhs);
    explicit PValue(bool rhs);
    explicit PValue(double rhs);
    explicit PValue(const StackValue &rhs);
    PValue &operator=(const PValue &rhs) = default;
    PValue &operator=(int64_t rhs);
    PValue &operator=(uint64_t rhs);
    PValue &operator=(int rhs);
    PValue &operator=(bool rhs);
    PValue &operator=(double rhs);

    PValue(PValue &&rhs) = default;

    PValue &operator=(PValue &&rhs) = default;

    /**
     * @brief Если значение не false, не null, не undefined или другое пустое,
     *        то вернёт true.
     * @return
     */
    bool asBoolean() const;
    bool isNull() const;
    /**
     * @brief Возвращает значение как uint64_t
     * @return
     */
    uint64_t value64() const;

    std::u32string uString() const;

    static PValue getValue(ObjectRecord *ptr);

    static PValue getValue(const escript::StackValue &item);

    static PValue binaryOpValues(const PValue &value1, const PValue &value2,
                                 ArithmeticOperation op);

    void increment();
    void decrement();

    friend bool operator<(const PValue &v1, const PValue &v2);

    static PValue lshift(const PValue &v1, const PValue &v2);
    static PValue rshift(const PValue &v1, const PValue &v2);
    static PValue rshiftz(const PValue &v1, const PValue &v2);
    static PValue bit_and(const PValue &v1, const PValue &v2);
    static PValue bit_or(const PValue &v1, const PValue &v2);
    static PValue bit_xor(const PValue &v1, const PValue &v2);
    static PValue log_and(const PValue &v1, const PValue &v2);
    static PValue log_or(const PValue &v1, const PValue &v2);
    static PValue mod(const PValue &v1, const PValue &v2);
};

bool operator<(const PValue &v1, const PValue &v2);
bool operator<=(const PValue &v1, const PValue &v2);
bool operator>(const PValue &v1, const PValue &v2);
bool operator>=(const PValue &v1, const PValue &v2);
bool operator==(const PValue &v1, const PValue &v2);

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
    case ArithmeticOperation::Mod:
        return PValue::mod(PValue(v1), PValue(v2));
    case ArithmeticOperation::BoolLess:
        return PValue(PValue(v1) < PValue(v2));
    case ArithmeticOperation::BoolLessOrEqual:
        return PValue(PValue(v1) <= PValue(v2));
    case ArithmeticOperation::BoolGreater:
        return PValue(PValue(v1) > PValue(v2));
    case ArithmeticOperation::BoolGreaterOrEqual:
        return PValue(PValue(v1) >= PValue(v2));
    case ArithmeticOperation::BoolEqual:
        return PValue(PValue(v1) == PValue(v2));
    case ArithmeticOperation::BoolNotEqual:
        return PValue(PValue(v1) == PValue(v2));
    case ArithmeticOperation::LShift:
        return PValue::lshift(PValue(v1), PValue(v2));
    case ArithmeticOperation::RShift:
        return PValue::rshift(PValue(v1), PValue(v2));
    case ArithmeticOperation::RShiftZero:
        return PValue::rshiftz(PValue(v1), PValue(v2));
    case ArithmeticOperation::BITAND:
        return PValue::bit_and(PValue(v1), PValue(v2));
    case ArithmeticOperation::BITOR:
        return PValue::bit_or(PValue(v1), PValue(v2));
    case ArithmeticOperation::BITXOR:
        return PValue::bit_xor(PValue(v1), PValue(v2));
    case ArithmeticOperation::LOGAND:
        return PValue::log_and(PValue(v1), PValue(v2));
    case ArithmeticOperation::LOGOR:
        return PValue::log_or(PValue(v1), PValue(v2));
    default:
        throw std::domain_error("Unsupported bin.op");
    }
}



} // namespace escript

#endif // PVALUE_H
