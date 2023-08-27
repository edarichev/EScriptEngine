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
#include "stdafx.h"
#include "operand.h"
#include "symbol.h"
#include "automationobject.h"

namespace escript {

Operand::Operand(){}

Operand::Operand(IntType value)
{
    intValue = value;
    type = SymbolType::Integer;
}

Operand::Operand(RealType value)
{
    realValue = value;
    type = SymbolType::Real;
}

Operand::Operand(bool value)
{
    boolValue = value;
    type = SymbolType::Boolean;
}

Operand::Operand(Symbol *symbol, SymbolType stype)
{
    switch (stype) {
    case SymbolType::Function:
        type = stype;
        function = symbol;
        break;
    case SymbolType::Variable:
        type = stype;
        variable = symbol;
        break;
    default:
        throw std::domain_error("Not supported type for Operand");
    }
}

std::string Operand::toString() const
{
    std::string opStr;
    switch (type) {
    case SymbolType::Integer:
        opStr = std::to_string(intValue);
        break;
    case SymbolType::Real:
        opStr = std::to_string(realValue);
        break;
    case SymbolType::Variable:
        opStr = variable->utf8Name();
        break;
    case SymbolType::Boolean:
        opStr = boolValue ? "true" : "false";
        break;
    case SymbolType::Function:
        opStr = "function";
        break;
    case SymbolType::Undefined:
        opStr = "undefined";
        break;
    case SymbolType::String:
        opStr = "string";
        break;
    case SymbolType::Array:
        opStr = "array";
        break;
    default:
        throw std::domain_error(__PRETTY_FUNCTION__ + std::string(": Unsupported SymbolType"));
    }
    return opStr;
}



} // namespace escript
