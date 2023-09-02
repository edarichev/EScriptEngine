/**
 * @file assembler.cpp
 * @date 15.08.2023 21:38:34
 * @brief Класс для записи инструкций в объектный файл
 * @warning Часть этого файла, отмеченная тегами "autogenerated",
 * генерируется скриптом на основе описания машинных кодов
 * (см. каталог codetemplates). Не пишите ничего между этими тегами.
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
#include "assembler.h"

namespace escript {

// <autogenerated>

// мнемоники
const std::string Assembler::_opCodeNames[] = {
    "NOP", "HALT", "LDC.UINT64 DATA64", "LDC.INT64 DATA64", 
    "LDLOC M", "LDC.DOUBLE DATA64", "ADDST", "MULST", 
    "DIVST", "SUBST", "STLOC M", "NEG", 
    "JMP M", "LDC.BOOL DATA8", "IFFALSE M", "STLESS", 
    "STLESS.EQ", "STGT", "STGT.EQ", "STEQ", 
    "LDARGS", "RET", "CALL", "LDSTRING", 
    "CALLM", "ALLOCARRAY", "INC", "DEC", 
    "POP", "RSH", "RSHZ", "LSH", 
    "BIT.AND", "BIT.OR", "BIT.XOR", "LOG.AND", 
    "LOG.OR", "BIT.NOT", "LOG.NOT", "MODST", 
    "ST.AR", "LD.AR", "LD.NULL", "STNOTEQ", 
    "STNCO", "CTOR", "CHTYPE", 
};

// все коды операций
OpCode Assembler::_opCodes[] = {
    OpCode::NOP, OpCode::HALT, OpCode::LDC_UINT64_DATA64, OpCode::LDC_INT64_DATA64, 
    OpCode::LDLOC_M, OpCode::LDC_DOUBLE_DATA64, OpCode::ADDST, OpCode::MULST, 
    OpCode::DIVST, OpCode::SUBST, OpCode::STLOC_M, OpCode::NEG, 
    OpCode::JMP_M, OpCode::LDC_BOOL_DATA8, OpCode::IFFALSE_M, OpCode::STLESS, 
    OpCode::STLESS_EQ, OpCode::STGT, OpCode::STGT_EQ, OpCode::STEQ, 
    OpCode::LDARGS, OpCode::RET, OpCode::CALL, OpCode::LDSTRING, 
    OpCode::CALLM, OpCode::ALLOCARRAY, OpCode::INC, OpCode::DEC, 
    OpCode::POP, OpCode::RSH, OpCode::RSHZ, OpCode::LSH, 
    OpCode::BIT_AND, OpCode::BIT_OR, OpCode::BIT_XOR, OpCode::LOG_AND, 
    OpCode::LOG_OR, OpCode::BIT_NOT, OpCode::LOG_NOT, OpCode::MODST, 
    OpCode::ST_AR, OpCode::LD_AR, OpCode::LD_NULL, OpCode::STNOTEQ, 
    OpCode::STNCO, OpCode::CTOR, OpCode::CHTYPE, 
};

// размер в байтах каждой команды
uint8_t Assembler::_opCodeSize[] = {
    1, 1, 9, 9, 
    9, 9, 1, 1, 
    1, 1, 9, 1, 
    9, 2, 9, 1, 
    1, 1, 1, 1, 
    1, 1, 9, 9, 
    1, 9, 1, 1, 
    1, 1, 1, 1, 
    1, 1, 1, 1, 
    1, 1, 1, 1, 
    9, 9, 1, 1, 
    1, 9, 2, 
};

// </autogenerated>

// ключ - код операции, значение - индекс в массивах
std::map<OpCode, int> Assembler::_opCodesMap;

void Assembler::setShowListing(bool newShowListing)
{
    _showListing = newShowListing;
}

const std::string &Assembler::mnemonics(OpCode opCode)
{
    auto it = _opCodesMap.find(opCode);
    if (it == _opCodesMap.end())
        throw std::out_of_range("Unknown OpCode");
    return _opCodeNames[it->second];
}

uint8_t Assembler::instructionSize(OpCode opCode)
{
    const constexpr int n = sizeof(_opCodes) / sizeof(OpCode);
    if ((OpCodeType)opCode >= n)
        throw std::out_of_range("Unknown OpCode");
    return _opCodeSize[(OpCodeType)opCode];
}

void Assembler::initOpCodesMap()
{
    if (!_opCodesMap.empty())
        return;
    for (int i = 0; i < (int)(sizeof (_opCodes) / sizeof (OpCode)); i++) {
        _opCodesMap[_opCodes[i]] = i;
    }
}

void Assembler::disassemble(std::ostream &out)
{
    size_t i = 0;
    const uint8_t *p = _buffer.data();
    // пропустить начальный JMP и 4 байта "DATA"
    uint32_t dataLength = *(uint32_t*)(p + 4 + Assembler::instructionSize(OpCode::JMP_M));
    i = Assembler::instructionSize(OpCode::JMP_M) +
            4 + 4 + dataLength + 4; // 4=DATA + dataLength + 4=CODE
    uint32_t codeLength = *(uint32_t*)(p + i);
    i += sizeof (codeLength);
    size_t n = i + codeLength;
    const char *fnMarker = "FUNC";
    while (i < n) {
        // читаем команду
        OpCodeType opCodeType = *(OpCodeType*)(p + i);
        OpCode opCode = (OpCode)opCodeType;
        try {
            uint8_t instrSize = instructionSize(opCode);
            i += instrSize;
            if (_showListing)
                out << mnemonics(opCode) << std::endl;
        } catch (const std::out_of_range &e) {
            if (*(p + i) == 'F') {
                if (strncmp((char*)p + i, fnMarker, 4) == 0) {
                    i += 4;
                    int32_t dataLen = *(int32_t*)(p + i);
                    i += 4 + 4 + dataLen;
                    continue;
                }
            }
            throw;
        }
    }
    assert (n == i);
}

} // namespace escript
