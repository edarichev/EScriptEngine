/**
 * @file types.cpp
 * @brief Некоторые преобразования для типов
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
#include "types.h"
#include "objectrecord.h"
#include "stringobject.h"
#include "arrayobject.h"

using namespace std;

namespace escript {

std::u32string to_u32string(const SymbolType &t, uint64_t v)
{
    std::ostringstream strs;
    std::string s1;
    switch (t) {
    case SymbolType::Integer:
        strs << (int64_t)v;
        s1 = strs.str();
        break;
    case SymbolType::Boolean:
        s1 = v ? "true" : "false";
        break;
    case SymbolType::Real: {
        strs << std::defaultfloat << bit_cast<double>(v);
        s1 = strs.str();
        break;
    }
    case SymbolType::String:
        return ((StringObject*)v)->uString();
    case SymbolType::Array:
        return ((Array*)v)->uString();
    case SymbolType::Function:
        return U"function";
    case SymbolType::Null:
        return U"null";
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)v;
        return to_u32string(rec->type, rec->data);
    }
    default:
        throw std::domain_error("Conversion from SymbolType to u32string not supported");
    }
    return std::u32string(s1.begin(), s1.end());
}

u32string to_u32string(const std::string &ascii)
{
    return u32string(ascii.begin(), ascii.end());
}

u32string to_u32string(int64_t v)
{
    return to_u32string(SymbolType::Integer, v);
}

u32string to_u32string(double v)
{
    return to_u32string(SymbolType::Real, v);
}

u32string to_u32string(bool v)
{
    return to_u32string(SymbolType::Boolean, v);
}


} // namespace escript
