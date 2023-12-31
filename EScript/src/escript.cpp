/**
 * @file escript.cpp
 * @brief Реализация класса скриптового движка.
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#include "stdafx.h"
#include "escript.h"
#include "parser.h"
#include "translator.h"
#include "storage.h"
#include "console.h"
#include "mathobject.h"
#include "numberobject.h"
#include "datetimeobject.h"

namespace escript {

static const char32_t *consoleId = U"console";
static const char32_t *mathId = U"Math";
static const char32_t *numberId = U"Number";
static const char32_t *dateId = U"DateTime";

EScript::EScript()
{
    registerClass(U"Array", Array::ctor);
    registerClass(U"DateTime", DateTimeObject::ctor);
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
    _standardObjectsAdded = false;
    _machine.clear();
}

ObjectRecord *EScript::getObjectRecord(std::shared_ptr<Symbol> symbol)
{
    return (ObjectRecord *)_machine.addressValueOf(symbol);
}

void EScript::eval(const std::u32string &strCode)
{
    compile(strCode);
    run();
}

void EScript::compile(const std::u32string &strCode)
{
    std::vector<TCode> buffer;
    // каждая единица трансляции находится в дочернем блоке главного блока
    // глобальная таблица символов находится в самом верхнем уровне,
    // из каждого нового блока глобальные переменные перемещаются
    // в глобальную таблицу символов (далее)
    auto newBlock = unit()->block()->addBlock();
    registerStandardObjects(newBlock);
    registerDeferredObjects(newBlock);
    Parser parser(newBlock, _machine.strings(), _classes, buffer);
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
    ((Console*) _standardObjects[consoleId])->setOutputStream(_outStream);
    _standardObjectsAdded = true;
}

void EScript::run()
{
    _machine.run();
}

void EScript::registerStandardObjects(std::shared_ptr<Block> &firstBlock)
{
    auto consoleObject = unit()->block()->symbolTable()->find(consoleId);
    if (!consoleObject)
        consoleObject = firstBlock->symbolTable()->add(consoleId);
    auto mathObject = unit()->block()->symbolTable()->find(mathId);
    if (!mathObject)
        mathObject = firstBlock->symbolTable()->add(mathId);
    auto numberObject = unit()->block()->symbolTable()->find(numberId);
    if (!numberObject)
        numberObject = firstBlock->symbolTable()->add(numberId);
    auto dateObject = unit()->block()->symbolTable()->find(dateId);
    if (!dateObject)
        dateObject = firstBlock->symbolTable()->add(dateId);
}

void EScript::addStandardObjects()
{
    if (_standardObjectsAdded)
        return;
    auto consoleSymbol = unit()->block()->symbolTable()->find(consoleId);
    auto consoleRecord = _machine.storage().installRecord(consoleSymbol.get());
    consoleRecord->type = SymbolType::Object;
    Console *console = new Console();
    console->addRef();
    _standardObjects[consoleId] = console;
    consoleRecord->data = (uint64_t)console;
    _machine.replaceValuePtr(consoleSymbol, consoleRecord);

    auto mathSymbol = unit()->block()->symbolTable()->find(mathId);
    auto mathRecord = _machine.storage().installRecord(mathSymbol.get());
    mathRecord->type = SymbolType::Object;
    Math *math = new Math();
    math->addRef();
    _standardObjects[mathId] = math;
    mathRecord->data = (uint64_t)math;
    _machine.replaceValuePtr(mathSymbol, mathRecord);

    auto numberSymbol = unit()->block()->symbolTable()->find(numberId);
    auto numberRecord = _machine.storage().installRecord(numberSymbol.get());
    numberRecord->type = SymbolType::Object;
    Number *number = new Number();
    number->addRef();
    _standardObjects[numberId] = number;
    numberRecord->data = (uint64_t)number;
    _machine.replaceValuePtr(numberSymbol, numberRecord);

    auto dateSymbol = unit()->block()->symbolTable()->find(dateId);
    auto dateRecord = _machine.storage().installRecord(dateSymbol.get());
    dateRecord->type = SymbolType::Object;
    DateTimeObject *date = new DateTimeObject();
    date->addRef();
    _standardObjects[dateId] = date;
    dateRecord->data = (uint64_t)date;
    _machine.replaceValuePtr(dateSymbol, dateRecord);
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

Machine *EScript::machine()
{
    return &_machine;
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
    // удалять из таблицы символов нет смысла - указатель зашит намертво в код.
    // его, правда, можно обнулить
    // убираем из таблицы объектов:
    _machine.storage().removeRecord(obj);
}

void EScript::registerClass(const std::u32string &className, ConstructorFunction pFn)
{
    _classes.insert_or_assign(className, pFn);
}


} // namespace escript
