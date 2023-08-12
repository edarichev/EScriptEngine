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

ObjectRecord *EScript::getObjectRecord(std::shared_ptr<Symbol> symbol)
{
    auto s = _machine.storage();
    ObjectRecord *r = s.findRecord(symbol.get());
    return r;
}

std::u32string EScript::getUString(int index)
{
    return _machine.strings().get(index);
}

void EScript::eval(const std::u32string &strCode)
{
    std::vector<TCode> buffer;
    auto newBlock = _unit->block()->addBlock();
    Parser parser(newBlock, _machine.strings(), buffer);
    parser.parse(strCode);
    // оптимизатор промежуточного кода будет находиться здесь
    std::vector<uint8_t> objectFile;
    Translator translator;
    // пока нет вложенных блоков, передаём глобальный блок
    translator.translate(newBlock, buffer, objectFile);
    Assembler::disassemble(objectFile, std::cout);
    _machine.load(newBlock, objectFile);
    _machine.run();
}

} // namespace escript
