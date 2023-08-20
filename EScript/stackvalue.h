/**
 * @file stackvalue.h
 * @brief Значение, хранимое в стеке машины во время выполнения
 */
#ifndef STACKVALUE_H
#define STACKVALUE_H

#include "types.h"

namespace escript {

/**
 * @brief Значение, хранимое в стеке машины во время выполнения
 */
struct StackValue
{
    /**
     * Тип значения
     */
    SymbolType type { SymbolType::Undefined };
    /**
     * Значение: либо простой тип (число, boolean), либо указатель на
     * ObjectRecord или StringObject в таблице строк
     */
    uint64_t value {};

    StackValue(){}
    StackValue(SymbolType t, uint64_t v) : type(t), value(v){}
    const std::u32string uString() const;
    /**
     * @brief Пытается извлечь целое значение из всех возможных полей
     * @return Целое значение или исключение.
     */
    int64_t getIntValue() const;
    double getRealValue() const;
};


} // namespace escript

#endif // STACKVALUE_H
