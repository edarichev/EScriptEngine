/**
 * @file machine.h
 * @brief Исполняющая машина (реализация)
 */
#include "stdafx.h"
#include "machine.h"
#include "opcode.h"
#include "assembler.h"
#include "block.h"

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
    uint64_t currentPos = _memory.size();
    auto blockSymbolTable = block->symbolTable();
    // корректируем все символы в блоке, ставим действительный адрес
    block->addOffset(currentPos);
    // теперь перемещаем глобальную таблицу символов в главный блок
    auto globalSymbolTable = block->globalBlock()->symbolTable();
    globalSymbolTable->addRange(std::move(blockSymbolTable));

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
    // а эту часть нужно обработать, пока просто скопируем
    // нужно исправить все условные и безусловные переходы
    uint64_t fromPos = _memory.size();
    _memory.insert(_memory.end(), objectFile.begin() + codeOffset, objectFile.end());
    uint64_t offset = currentPos;
    replaceJMPAddresses(fromPos, offset);
}

void Machine::replaceJMPAddresses(uint64_t startPosition, uint64_t offset)
{
    // это версия с проходом по всему тексту
    // если быстрее будет через multimap - попробовать её
    uint64_t c = startPosition;
    uint64_t addr = 0;
    while (c < _memory.size()) {
        uint8_t *p = _memory.data() + c;
        OpCode opCode = (OpCode) *((OpCodeType*)p);
        auto shift = Assembler::instructionSize(opCode);
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
