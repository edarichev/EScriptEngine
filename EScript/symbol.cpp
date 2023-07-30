/**
 * @file symbol.cpp
 * @brief Запись в таблице символов (реализация)
 */
#include "stdafx.h"
#include "symbol.h"

namespace escript {

Symbol::Symbol()
{

}

Symbol::~Symbol()
{

}

SymbolType Symbol::type() const
{
    return _type;
}

void Symbol::setType(SymbolType newType)
{
    _type = newType;
}


} // namespace escript
