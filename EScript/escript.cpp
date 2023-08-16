/**
 * @file escript.cpp
 * @brief Реализация класса скриптового движка.
 */
#include "stdafx.h"
#include "escript.h"
#include "parser.h"
#include "translator.h"
#include "storage.h"
#include "console.h"

namespace escript {

const Machine &EScript::machine() const
{
    return _machine;
}

void EScript::setShowTCode(bool newShowTCode)
{
    _showTCode = newShowTCode;
}

void EScript::setShowDisassembleListing(bool newShowDisassembleListing)
{
    _showDisassembleListing = newShowDisassembleListing;
}

void EScript::setOutStream(std::ostream &newOutStream)
{
    _outStream = &newOutStream;
}

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
    uint64_t addr = _machine.addressValueOf(symbol);
    ObjectRecord *r = (ObjectRecord*)addr;
    return r;
}

void EScript::eval(const std::u32string &strCode)
{
    std::vector<TCode> buffer;
    auto newBlock = _unit->block()->addBlock();
    std::u32string consoleId = U"console";
    auto consoleObject = _unit->block()->symbolTable()->find(consoleId);
    bool addconsole = consoleObject == nullptr;
    if (addconsole)
        consoleObject = newBlock->symbolTable()->add(consoleId);
    Parser parser(newBlock, _machine.strings(), buffer);
    parser.parse(strCode);
    // оптимизатор промежуточного кода будет находиться здесь
    std::vector<uint8_t> objectFile;
    Translator translator;
    translator.setShowListing(_showTCode);
    // пока нет вложенных блоков, передаём глобальный блок
    translator.translate(newBlock, buffer, objectFile);
    Assembler a(objectFile);
    a.setShowListing(_showDisassembleListing);
    a.disassemble(std::cout);
    _machine.load(newBlock, objectFile);
    if (addconsole) {
        auto consoleRecord = _machine.storage().installRecord(consoleObject.get());
        consoleRecord->type = SymbolType::Object;
        Console *console = new Console();
        consoleRecord->data = (uint64_t)console;
        _machine.replaceValuePtr(consoleObject, consoleRecord);
    }
    uint64_t addr = _machine.addressValueOf(consoleObject);
    ObjectRecord *rec = (ObjectRecord*)addr;
    ((Console*)rec->data)->setOutputStream(*_outStream);
    _machine.run();
}

} // namespace escript
