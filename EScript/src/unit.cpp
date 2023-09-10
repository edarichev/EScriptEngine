/**
 * @file unit.cpp
 * @brief Программный модуль (реализация)
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
#include "unit.h"

namespace escript {


Unit::Unit()
{

}

Unit::~Unit()
{
    clear();
}

void Unit::clear()
{
    _block->clear();
    _block.reset();
}

std::shared_ptr<Block> Unit::block()
{
    if (!_block)
        _block = std::make_shared<Block>(shared_from_this());
    return _block;
}


} // namespace escript
