/**
 * @file symbol.cpp
 * @brief Запись в таблице символов (реализация)
 */
#include "stdafx.h"
#include "symbol.h"
#include "unit.h"

namespace escript {

Symbol::Symbol(std::shared_ptr<Unit> unit,
               const std::u32string &id,
               SymbolType dataType)
    : _unit(unit), _name(id), _type(dataType)
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
    if (_type != newType) {
        detachOldValue();
        _type = newType;
        switch (_type) {
        case SymbolType::Integer:
            _dataPtr = _unit->addStaticIntValue();
            break;
        default:
            throw std::domain_error("Unsupported data type");
        }
    }
}

void *Symbol::data() const
{
    return _dataPtr;
}

void Symbol::detachOldValue()
{
    // TODO: уведомить среду выполнения, что cтарое значение больше не нужно
}

} // namespace escript
