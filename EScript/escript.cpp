/**
 * @file escript.cpp
 * @brief Реализация класса скриптового движка.
 */
#include "stdafx.h"
#include "escript.h"
#include "parser.h"
#include "translator.h"

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
    std::vector<TCode> buffer;
    Parser parser(_unit, buffer);
    parser.parse(strCode);
    // оптимизатор промежуточного кода будет находиться здесь
    std::vector<uint8_t> objectFile;
    Translator translator;
    // пока нет вложенных блоков, передаём глобальный блок
    translator.translate(_unit->block(), buffer, objectFile);
}

} // namespace escript
