/**
 * @file symbol.cpp
 * @brief Запись в таблице символов (реализация)
 */
#include "stdafx.h"
#include "symbol.h"
#include "unit.h"

namespace escript {

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

Symbol::Symbol(const std::u32string &id)
    : _name(id)
{

}

Symbol::~Symbol()
{

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
