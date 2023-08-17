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

void EScript::attachObject(AutomationObject *obj, const std::u32string &name)
{
    if (_isAlreadyRunned)
        throw std::domain_error("An object can only be attached before the first launch.");
    _deferredObjects.push_back(std::make_pair(obj, name));
}

void EScript::detachObject(AutomationObject *obj)
{
    // если в отложенных, то ещё не добавлено в таблицу символов
    for (auto it = _deferredObjects.begin(); it != _deferredObjects.end(); ++it) {
        if ((*it).first == obj) {
            _deferredObjects.erase(it);
            return;
        }
    }
    // удалять из таблицы символов нет смысла - указтель зашит намертво в код.
    // его, правда, можно обнулить
    // убираем из таблицы объектов:
    _machine.storage().removeRecord(obj);
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
    if (!_isAlreadyRunned) {
        for (auto &&o : _deferredObjects) {
            auto newSymbol = newBlock->symbolTable()->add(o.second);
            _deferredSymbols.push_back(newSymbol);
        }
    }
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
    if (_showDisassembleListing)
        std::cout << "=== DISASSEMBLING ===" << std::endl;
    a.disassemble(std::cout);
    _machine.load(newBlock, objectFile);
    if (addconsole) {
        auto consoleRecord = _machine.storage().installRecord(consoleObject.get());
        consoleRecord->type = SymbolType::Object;
        Console *console = new Console();
        consoleRecord->data = (uint64_t)console;
        _machine.replaceValuePtr(consoleObject, consoleRecord);
    }
    for (size_t i = 0; i < _deferredObjects.size(); i++) {
        auto theSymObject = _deferredSymbols[i];
        auto objRecord = _machine.storage().installRecord(theSymObject.get());
        objRecord->type = SymbolType::Object;
        objRecord->data = (uint64_t)_deferredObjects[i].first;
        _machine.replaceValuePtr(theSymObject, objRecord);
    }
    uint64_t addr = _machine.addressValueOf(consoleObject);
    ObjectRecord *rec = (ObjectRecord*)addr;
    ((Console*)rec->data)->setOutputStream(*_outStream);
    _machine.run();
}

} // namespace escript
