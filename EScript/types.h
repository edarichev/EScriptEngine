/**
 * @file types.h
 * @brief Определения типов
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
#ifndef TYPES_H
#define TYPES_H

#include "EScript_global.h"

namespace escript {

/**
 * @brief Тип символа
 */
enum class SymbolType : uint8_t
{
    Null,
    Integer,
    Real,
    Boolean,
    String,
    Function,
    Array,
    Object,
    /**
     * Вспомогательный тип, он нужен только в таблице символов
     */
    Variable,
    Block,
};

using IntType = long;
using RealType = double;
using StringType = std::u32string; // потом сделать по стандарту отдельный класс

std::u32string to_u32string(const SymbolType &t, uint64_t v);
std::u32string to_u32string(const std::string &ascii);
std::u32string to_u32string(int64_t v);
std::u32string to_u32string(double v);
std::u32string to_u32string(bool v);

} // namespace escript

#endif // TYPES_H
