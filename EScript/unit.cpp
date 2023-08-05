/**
 * @file unit.cpp
 * @brief Программный модуль (реализация)
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

}

std::shared_ptr<Block> Unit::block()
{
    if (!_block)
        _block = std::make_shared<Block>(shared_from_this());
    return _block;
}


} // namespace escript
