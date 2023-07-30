/**
 * @file symboltable.cpp
 * @brief Таблица символов (реализация)
 */
#include "stdafx.h"
#include "symboltable.h"

namespace escript {

static const constexpr char32_t *TEMP_VARIABLE_PREFIX = U"_tmp_";

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parentSymbolTable)
    : _parentTable(parentSymbolTable)
{

}

SymbolTable::~SymbolTable()
{

}

std::shared_ptr<Symbol> SymbolTable::add(const std::u32string &identifier)
{
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>();
    if (_symbols.find(identifier) != _symbols.end())
        throw std::domain_error("duplicate identifier");
    _symbols[identifier] = symbol;
    return symbol;
}

std::shared_ptr<Symbol> SymbolTable::addTemp()
{
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>();
    std::u32string identifier;
    identifier.reserve(30);
    identifier.append(TEMP_VARIABLE_PREFIX);
    long i = _tmpVariableCounter++;
    while (i) {
        long k = i % 10;
        char32_t c = k + '0';
        identifier.push_back(c);
        i /= 10;
    }
    return symbol;
}

std::shared_ptr<Symbol> SymbolTable::operator[](const std::u32string &identifier)
{
    auto it = _symbols.find(identifier);
    if (it != _symbols.end())
        return it->second;
    return nullptr;
}

long SymbolTable::_tmpVariableCounter = 1;

} // namespace escript
