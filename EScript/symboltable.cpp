/**
 * @file symboltable.cpp
 * @brief Таблица символов (реализация)
 */
#include "stdafx.h"
#include "symboltable.h"

namespace escript {

static const constexpr char32_t *TEMP_VARIABLE_PREFIX = U"#tmp_";

SymbolTable::SymbolTable(
        std::shared_ptr<Unit> unit,
        std::shared_ptr<SymbolTable> parentSymbolTable)
    : _unit(unit), _parentTable(parentSymbolTable)
{

}

SymbolTable::~SymbolTable()
{

}

void SymbolTable::clear()
{
    _orderedSymbols.clear();
}

std::shared_ptr<Symbol> SymbolTable::add(const std::u32string &identifier)
{
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>(identifier);
    if (_symbols.find(identifier) != _symbols.end())
        throw std::domain_error("duplicate identifier");
    _symbols.insert(std::make_pair(identifier, symbol));
    _orderedSymbols.push_back(symbol);
    return symbol;
}

void SymbolTable::addRange(std::shared_ptr<SymbolTable> &&otherTable)
{
    _orderedSymbols.insert(_orderedSymbols.end(), otherTable->begin(), otherTable->end());
    _symbols.merge(std::move(otherTable->_symbols));
}

std::shared_ptr<Symbol> SymbolTable::addTemp()
{
    return add(makeTempId());
}

std::shared_ptr<Symbol> SymbolTable::operator[](const std::u32string &identifier)
{
    return find(identifier);
}

std::shared_ptr<Symbol> SymbolTable::find(const std::u32string &identifier)
{
    auto it = _symbols.find(identifier);
    if (it != _symbols.end())
        return it->second;
    if (_parentTable) {
        return _parentTable->find(identifier);
    }
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTable::findCurrentScopeOnly(const std::u32string &identifier)
{
    auto it = _symbols.find(identifier);
    if (it != _symbols.end())
        return it->second;
    // Возможно, родительский, если он верхнего уровня, тоже надо
    // просмотреть, т.к. всё сливаются в глобальную область:
    if (_parentTable && !_parentTable->_parentTable) {
        return _parentTable->find(identifier);
    }
    return nullptr;
}

void SymbolTable::addOffset(uint64_t offset)
{
    for (auto &item : _symbols)
        item.second->addOffset(offset);

}

std::u32string SymbolTable::makeTempId()
{
    std::u32string identifier;
    identifier.reserve(30);
    identifier.append(TEMP_VARIABLE_PREFIX);
    long i = _tmpVariableCounter++;
    std::string id = std::to_string(i);
    identifier.insert(identifier.begin() + identifier.length(), id.begin(), id.end());
    return identifier;
}

long SymbolTable::_tmpVariableCounter = 1;

} // namespace escript
