/**
 * @file machine.h
 * @brief Исполняющая машина (реализация)
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
#include "machine.h"
#include "opcode.h"
#include "assembler.h"
#include "block.h"
#include "functionobject.h"

namespace escript {

Processor *Machine::cpu()
{
    return &_cpu;
}

uint64_t Machine::addressValueOf(std::shared_ptr<Symbol> &symbol) const
{
    assert(symbol != nullptr);
    return *(uint64_t*)(_memory.data() + symbol->location());
}

void Machine::replaceValuePtr(std::shared_ptr<Symbol> &symbol, ObjectRecord *rec)
{
    *(uint64_t*)(_memory.data() + symbol->location()) = (uint64_t)rec;
}

void Machine::replaceValuePtr(std::shared_ptr<Symbol> &symbol, StringObject *s)
{
    *(uint64_t*)(_memory.data() + symbol->location()) = (uint64_t)s;
}

Machine::Machine()
    : _cpu(this)
{
    _cpu.setStrings(&_strings);
}

Machine::~Machine()
{

}

void Machine::clear()
{
    _memory.clear();
    _strings.clear();
    _storage.clear();
}

Storage &Machine::storage()
{
    return _storage;
}

StringContainer &Machine::strings()
{
    return _strings;
}

void Machine::load(std::shared_ptr<Block> block,
                   const std::vector<uint8_t> &objectFile)
{
    uint64_t currentPos = _memory.size();
    auto blockSymbolTable = block->symbolTable();
    // корректируем все символы в блоке, ставим действительный адрес
    block->addOffset(currentPos);

    // первый JMP
    _memory.insert(_memory.end(), objectFile.begin(), objectFile.begin() + sizeof(OpCodeType));
    uint64_t jumpTo = *(uint64_t*)(objectFile.data() + sizeof(OpCodeType));
    uint64_t codeOffset = jumpTo;
    jumpTo += currentPos;
    _memory.insert(_memory.end(), (uint8_t*)&jumpTo, (uint8_t*)&jumpTo + sizeof (uint64_t));
    // скопировать до сегмента кода - это таблица символов с маркерами DATA/CODE
    _memory.insert(_memory.end(),
                   objectFile.begin() + (sizeof(OpCodeType) + sizeof (uint64_t)),
                   objectFile.begin() + codeOffset);
    uint64_t fromPos = _memory.size();
    _memory.insert(_memory.end(), objectFile.begin() + codeOffset, objectFile.end());
    uint64_t offset = currentPos;
    addCallAddressOffsetToFunctions(block, currentPos);
    // теперь перемещаем глобальную таблицу символов в главный блок
    auto globalSymbolTable = block->globalBlock()->symbolTable();
    globalSymbolTable->addRange(std::move(blockSymbolTable));
    // нужно исправить все условные и безусловные переходы
    replaceJMPAddresses(fromPos, offset);
}

void Machine::replaceJMPAddresses(uint64_t startPosition, uint64_t offset)
{
    // это версия с проходом по всему тексту
    // если быстрее будет через multimap - попробовать её
    uint64_t c = startPosition;
    uint64_t addr = 0;
    const char *fnMarker = "FUNC";
    while (c < _memory.size()) {
        uint8_t *p = _memory.data() + c;
        OpCode opCode = (OpCode) *((OpCodeType*)p);
        auto shift = 0;
        if (*p == 'F') { // TODO: код повторяется
            if (strncmp((char*)p, fnMarker, 4) == 0) {
                p += 4;
                c += 4;
                int32_t dataLen = *(int32_t*)p;
                c += 4 + 4 + dataLen;
                continue;
            }
        }
        shift = Assembler::instructionSize(opCode);
        switch (opCode) {
        case OpCode::IFFALSE_M:
        case OpCode::JMP_M:
            c += sizeof (OpCodeType); // размер кода команды
            p += sizeof (OpCodeType);
            addr = *(uint16_t*)p;
            addr += offset;
            std::copy((uint8_t*)&addr,
                      (uint8_t*)&addr + sizeof (addr),
                      _memory.begin() + c);
            c += sizeof (addr);
            break;
        default:
            c += shift;
            break;
        }
    }
}

void Machine::addCallAddressOffsetToFunctions(
        std::shared_ptr<Block> &block, uint64_t offset)
{
    auto symTable = block->symbolTable();
    for (auto &c : *symTable) {
        uint64_t addr = *(uint64_t*)(_memory.data() + c->location());
        if (!addr)
            continue;
        // у функций всегда здесь задан адрес, у других переменных - 0
        ObjectRecord *rec = (ObjectRecord*)addr;
        if (rec->type != SymbolType::Function)
            continue;
        Function *fnPtr = (Function*)rec->data;
        fnPtr->addOffset(offset);
    }
    for (auto &b : block->blocks()) {
        addCallAddressOffsetToFunctions(b, offset);
    }
}

size_t Machine::offsetOfStartPoint(const std::vector<uint8_t> &objectFile)
{
    // TODO: убрать повторяющийся код: см. assembler.cpp
    size_t i = 0;
    const uint8_t *p = objectFile.data();
    // пропустить 4 байта "DATA"
    uint32_t dataLength = *(uint32_t*)(p + 4);
    i = 4 + 4 + dataLength + 4 + 4; // 4=DATA + dataLength + 4=CODE
    return i;
}

void Machine::run()
{
    _cpu.setMemory(_memory.data(), _memory.size(), &_storage);
    _cpu.run();
}


} // namespace escript
