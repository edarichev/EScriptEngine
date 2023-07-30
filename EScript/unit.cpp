/**
 * @file unit.cpp
 * @brief Программный модуль (реализация)
 */
#include "stdafx.h"
#include "unit.h"

namespace escript {


Unit::Unit()
{
    _block = std::make_shared<Block>();
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
    return _block;
}


} // namespace escript
