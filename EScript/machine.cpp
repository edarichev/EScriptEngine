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

const Processor &Machine::cpu() const
{
    return _cpu;
}

uint64_t Machine::addressValueOf(std::shared_ptr<Symbol> &symbol) const
{
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
{
    _cpu.setStrings(&_strings);
}

Machine::~Machine()
{

}

Storage &Machine::storage()
{
    return _storage;
}

StringContainer &Machine::strings()
{
    return _strings;
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
    uint64_t fromPos = _memory.size();
    _memory.insert(_memory.end(), objectFile.begin() + codeOffset, objectFile.end());
    uint64_t offset = currentPos;
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
        case OpCode::CALL:
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
