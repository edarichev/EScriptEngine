/**
 * @file escript.cpp
 * @brief Реализация класса скриптового движка.
 */
#include "stdafx.h"
#include "escript.h"
#include "parser.h"

namespace escript {

EScript::EScript()
{
    _unit = std::make_shared<Unit>();
}

EScript::~EScript()
{
    clear();
}

Unit *EScript::unit()
{
    return _unit.get();
}

void EScript::clear()
{
    _unit->clear();
}

void EScript::eval(const std::u32string &strCode)
{
    Parser parser;
    parser.parse(strCode);
}

} // namespace escript
