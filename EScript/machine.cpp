/**
 * @file machine.h
 * @brief Исполняющая машина (реализация)
 */
#include "stdafx.h"
#include "machine.h"
#include "opcode.h"
#include "assembler.h"

namespace escript {

Machine::Machine()
{

}

Machine::~Machine()
{

}

const Storage &Machine::storage() const
{
    return _storage;
}

void Machine::load([[maybe_unused]] std::shared_ptr<Block> block,
                   const std::vector<uint8_t> &objectFile)
{
    // нужно скорректировать таблицу символов
    // пока просто копируем
    _memory.insert(_memory.end(), objectFile.begin(), objectFile.end());
    _cpu.setPC(_cpu.PC() + startOffsetOf(objectFile));
}

size_t Machine::startOffsetOf(const std::vector<uint8_t> &objectFile)
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
