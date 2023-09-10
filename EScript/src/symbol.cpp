/**
 * @file symbol.cpp
 * @brief Запись в таблице символов (реализация)
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
#include "symbol.h"
#include "unit.h"

namespace escript {

Symbol::Symbol(const std::u32string &id)
    : _name(id)
{

}

Symbol::~Symbol()
{

}

uint64_t Symbol::location() const
{
    return _location;
}

void Symbol::setLocation(uint64_t newLocation)
{
    _location = newLocation;
}

void Symbol::addOffset(uint64_t offset)
{
    _location += offset;
}

const std::u32string &Symbol::name() const
{
    return _name;
}

const std::string Symbol::utf8Name() const
{
    return to_utf8(_name);
}

} // namespace escript
