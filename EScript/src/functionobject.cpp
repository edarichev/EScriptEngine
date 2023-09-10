/**
 * @file functionobject.cpp
 * @brief Реализация класса функции.
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
#include "functionobject.h"

namespace escript {


Function::Function(Symbol *sym) : _symbol(sym)
{
    _managed = true;
}

Function::~Function()
{

}

uint64_t Function::callAddress() const
{
    return _callAddress;
}

void Function::setCallAddress(uint64_t newCallAddress)
{
    _callAddress = newCallAddress;
}

const std::u32string &Function::name() const
{
    return _name;
}

void Function::setName(const std::u32string &newName)
{
    _name = newName;
}

void Function::addOffset(int64_t offset)
{
    _callAddress += offset;
}


} // namespace escript
