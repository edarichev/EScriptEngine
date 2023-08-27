/**
 * @file stackvalue.cpp
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
#include "stdafx.h"
#include "stackvalue.h"
#include "objectrecord.h"
#include "stringobject.h"
#include "arrayobject.h"
#include <cmath>
#include <utility>
#include "arrayobject.h"

namespace escript {

const std::u32string StackValue::getStringValue() const
{
    return to_u32string(type, value);
}

int64_t to_int64(const std::u32string &s)
{
    if (s.empty())
        return 0;
    return std::stoll(std::wstring(s.begin(), s.end()));
}

double to_double(const std::u32string &s)
{
    if (s.empty())
        return 0;
    return std::stod(std::wstring(s.begin(), s.end()));
}

int64_t intValue(SymbolType type, uint64_t value)
{
    switch (type) {
    case SymbolType::Boolean:
        return value ? 1 : 0;
    case SymbolType::Integer:
        return (int64_t)value;
    case SymbolType::Real:
        return (int64_t)bit_cast<double>(value);
    case SymbolType::String:
        return to_int64(((StringObject*)value)->uString());
    case SymbolType::Variable:
        return intValue(((ObjectRecord*)value)->type, ((ObjectRecord*)value)->data);
    default:
        throw std::domain_error("Can not convert value to Integer");
    }
}

double doubleValue(SymbolType type, uint64_t value)
{
    switch (type) {
    case SymbolType::Boolean:
        return value ? 1 : 0;
    case SymbolType::Integer:
        return (int64_t)value;
    case SymbolType::Real:
        return bit_cast<double>(value);
    case SymbolType::String:
        return to_double(((StringObject*)value)->uString());
    case SymbolType::Variable:
        return doubleValue(((ObjectRecord*)value)->type, ((ObjectRecord*)value)->data);
    default:
        throw std::domain_error("Can not convert value to Real");
    }
}

Array *arrayValue(SymbolType type, uint64_t value)
{
    Array *a = nullptr;
    if (type == SymbolType::Array)
        a = (Array*)value;
    else if (type == SymbolType::Variable) {
        ObjectRecord *rec = (ObjectRecord*)value;
        if (rec->type == SymbolType::Array)
            a = (Array*)rec->data;
    }
    if (a == nullptr)
        throw std::domain_error("The value does not contains Array object");
    return a;
}

bool boolValue(SymbolType type, uint64_t value)
{
    switch (type) {
    case SymbolType::Boolean:
    case SymbolType::Integer:
    case SymbolType::Real:
        return (bool)value;
    case SymbolType::String:
        return ((StringObject*)value)->uString() == U"true";
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)value;
        return boolValue(rec->type, rec->data);
    }
    default:
        return false;
    }
}

int64_t StackValue::getIntValue() const
{
    return intValue(type, value);
}

double StackValue::getRealValue() const
{
    return doubleValue(type, value);
}

Array *StackValue::getArrayValue() const
{
    return arrayValue(type, value);
}

bool StackValue::getBoolValue() const noexcept(false)
{
    return boolValue(type, value);
}

Function *StackValue::getFunction() const
{
    if (type == SymbolType::Function)
        return (Function*)value;
    if (type == SymbolType::Variable) {
        ObjectRecord *rec = (ObjectRecord*)value;
        if (rec->type == SymbolType::Function)
            return (Function*)rec->data;
    }
    return nullptr;
}

bool StackValue::ofType(SymbolType t) const
{
    if (type == t)
        return true;
    if (type == SymbolType::Variable) {
        ObjectRecord *rec = (ObjectRecord*)value;
        return rec->type == t;
    }
    return false;
}



} // namespace escript
