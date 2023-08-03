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

Symbol::Symbol(std::shared_ptr<Unit> unit,
               const std::u32string &id,
               SymbolType dataType)
    : _unit(unit), _name(id), _type(dataType)
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
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(_name);
}

SymbolType Symbol::type() const
{
    return _type;
}

void Symbol::setType(SymbolType newType)
{
    if (_type != newType) {
        _type = newType;
        switch (_type) {
        case SymbolType::Integer:
            break;
        default:
            throw std::domain_error("Unsupported data type");
        }
    }
}

} // namespace escript
