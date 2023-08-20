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

static const char32_t *consoleId = U"console";

EScript::EScript()
{

}

EScript::~EScript()
{
    clear();
}

std::shared_ptr<Unit> EScript::unit()
{
    if (!_unit)
        _unit = std::make_shared<Unit>();
    return _unit;
}

void EScript::clear()
{
    _standardObjects.clear();
    if (_unit) {
        _unit->clear();
        _unit.reset();
    }
    _isAlreadyRunned = false;
    _machine.clear();
}

ObjectRecord *EScript::getObjectRecord(std::shared_ptr<Symbol> symbol)
{
    return (ObjectRecord *)_machine.addressValueOf(symbol);
}

void EScript::eval(const std::u32string &strCode)
{
    std::vector<TCode> buffer;
    // каждая единица трансляции находится в дочернем блоке главного блока
    // глобальная таблица символов находится в самом верхнем уровне,
    // из каждого нового блока глобальные переменные перемещаются
    // в глобальную таблицу символов (далее)
    auto newBlock = unit()->block()->addBlock();
    registerStandardObjects(newBlock);
    registerDeferredObjects(newBlock);
    Parser parser(newBlock, _machine.strings(), buffer);
    parser.parse(strCode);
    // TODO: оптимизатор промежуточного кода будет находиться здесь
    Translator translator(_machine.storage());
    translator.setShowListing(_showTCode);
    std::vector<uint8_t> objectFile;
    // пока нет вложенных блоков, передаём глобальный блок
    translator.translate(newBlock, buffer, objectFile);
    // TODO: оптимизатор целевого кода будет находиться здесь
    Assembler a(objectFile);
    a.setShowListing(_showDisassembleListing);
    if (_showDisassembleListing)
        std::cout << "=== DISASSEMBLING ===" << std::endl;
    a.disassemble(std::cout);
    _machine.load(newBlock, objectFile);
    addStandardObjects();
    addDeferredObjects();
    ((Console*) _standardObjects[consoleId])->setOutputStream(*_outStream);
    _machine.run();
    _isAlreadyRunned = true;
}

void EScript::registerStandardObjects(std::shared_ptr<Block> &firstBlock)
{
    auto consoleObject = unit()->block()->symbolTable()->find(consoleId);
    if (!consoleObject)
        consoleObject = firstBlock->symbolTable()->add(consoleId);
}

void EScript::addStandardObjects()
{
    if (_isAlreadyRunned)
        return;
    auto consoleSymbol = unit()->block()->symbolTable()->find(consoleId);
    auto consoleRecord = _machine.storage().installRecord(consoleSymbol.get());
    consoleRecord->type = SymbolType::Object;
    Console *console = new Console();
    _standardObjects[consoleId] = console;
    consoleRecord->data = (uint64_t)console;
    _machine.replaceValuePtr(consoleSymbol, consoleRecord);
}

void EScript::registerDeferredObjects(std::shared_ptr<Block> &objectFileBlock)
{
    for (auto &&o : _deferredObjects) {
        auto newSymbol = objectFileBlock->symbolTable()->add(o.second);
        _deferredSymbols.push_back(newSymbol);
    }
}

void EScript::addDeferredObjects()
{
    for (size_t i = 0; i < _deferredObjects.size(); i++) {
        auto theSymObject = _deferredSymbols[i];
        auto objRecord = _machine.storage().installRecord(theSymObject.get());
        objRecord->type = SymbolType::Object;
        objRecord->data = (uint64_t)_deferredObjects[i].first;
        _machine.replaceValuePtr(theSymObject, objRecord);
        _deferredObjects[i].first = nullptr;
    }
    _deferredObjects.clear();
}

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
    _deferredObjects.push_back(std::make_pair(obj, name));
}

void EScript::detachObject(AutomationObject *obj)
{
    // если в отложенных, то ещё не добавлено в таблицу символов
    for (auto it = _deferredObjects.begin(); it != _deferredObjects.end(); ++it) {
        if ((*it).first == obj) {
            (*it).first = nullptr;
            _deferredObjects.erase(it);
            return;
        }
    }
    // удалять из таблицы символов нет смысла - указтель зашит намертво в код.
    // его, правда, можно обнулить
    // убираем из таблицы объектов:
    _machine.storage().removeRecord(obj);
}


} // namespace escript
