/**
 * @file operand.h
 * @brief Операнд трёхадресного кода.
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
#ifndef ESCRIPT_OPERANDRECORD_H
#define ESCRIPT_OPERANDRECORD_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;
class Block;
class StringObject;

struct ESCRIPT_EXPORT Operand
{
    SymbolType type = SymbolType::Null;
    union {
        IntType intValue = 0;
        RealType realValue;
        bool boolValue;
        Symbol *variable;
        Symbol *function;
        Block *block;
        StringObject *strValue;
    };
    Operand();
    explicit Operand(IntType value);
    explicit Operand(RealType value);
    explicit Operand(bool value);
    explicit Operand(Symbol *symbol, SymbolType stype);
    std::string toString() const;
};

} // namespace escript

#endif // ESCRIPT_OPERANDRECORD_H
