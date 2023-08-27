/**
 * @file stackvalue.h
 * @brief Значение, хранимое в стеке машины во время выполнения
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
#ifndef STACKVALUE_H
#define STACKVALUE_H

#include "types.h"

namespace escript {

class Array;
class Function;
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
    /**
     * @brief Возвращает либо строку, содержащуюся в экземпляре StringObject,
     *        либо преобразует в строку содержимое.
     * @return
     */
    const std::u32string getStringValue() const noexcept(false);
    /**
     * @brief Пытается извлечь целое значение из всех возможных полей
     * @return Целое значение или исключение.
     */
    int64_t getIntValue() const noexcept(false);
    double getRealValue() const noexcept(false);
    Array *getArrayValue() const noexcept(false);
    bool getBoolValue() const noexcept(false);
    Function *getFunction() const;
    /**
     * @brief Вернёт true, если тип равен указанному или если тип - переменная,
     * и тип этой переменной равен указанному.
     * @param t
     * @return
     */
    bool ofType(SymbolType t) const;
};


} // namespace escript

#endif // STACKVALUE_H
