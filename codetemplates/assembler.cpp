/**
 * AUTOGENERATED
 * @file assembler.cpp
 * @date 15.08.2023 21:31:27
 * @brief Для записи инструкций в объектный файл
 */
#include "stdafx.h"
#include "assembler.h"

namespace escript {

// мнемоники
const std::string Assembler::_opCodeNames[] = {
    "NOP", "HALT", "LDC.UINT64 DATA64", "LDC.INT64 DATA64", 
    "LDLOC M", "LDC.DOUBLE DATA64", "ADDST", "MULST", 
    "DIVST", "SUBST", "STLOC M", "NEG", 
    "JMP M", "LDC.BOOL DATA8", "IFFALSE M", "STLESS", 
    "STLESS.EQ", "STGT", "STGT.EQ", "STEQ", 
    "LDARGS", "RET", "CALL", "LDSTRING", 
    "CALLM", "ALLOCARRAY", 
};
// все коды операций
OpCode Assembler::_opCodes[] = {
    OpCode::NOP, OpCode::HALT, OpCode::LDC_UINT64_DATA64, OpCode::LDC_INT64_DATA64, 
    OpCode::LDLOC_M, OpCode::LDC_DOUBLE_DATA64, OpCode::ADDST, OpCode::MULST, 
    OpCode::DIVST, OpCode::SUBST, OpCode::STLOC_M, OpCode::NEG, 
    OpCode::JMP_M, OpCode::LDC_BOOL_DATA8, OpCode::IFFALSE_M, OpCode::STLESS, 
    OpCode::STLESS_EQ, OpCode::STGT, OpCode::STGT_EQ, OpCode::STEQ, 
    OpCode::LDARGS, OpCode::RET, OpCode::CALL, OpCode::LDSTRING, 
    OpCode::CALLM, OpCode::ALLOCARRAY, 
};
// размер в байтах каждой команды
uint8_t Assembler::_opCodeSize[] = {
    1, 1, 9, 9, 
    9, 9, 1, 1, 
    1, 1, 9, 1, 
    9, 2, 9, 1, 
    1, 1, 1, 1, 
    1, 1, 9, 9, 
    1, 9, 
};
// ключ - код операции, значение - индекс в массивах
std::map<OpCode, int> Assembler::_opCodesMap;


const std::string &Assembler::mnemonics(OpCode opCode)
{
    auto it = _opCodesMap.find(opCode);
    if (it == _opCodesMap.end())
        throw std::out_of_range("Unknown OpCode");
    return _opCodeNames[it->second];
}

uint8_t Assembler::instructionSize(OpCode opCode)
{
    auto it = _opCodesMap.find(opCode);
    if (it == _opCodesMap.end())
        throw std::out_of_range("Unknown OpCode");
    return _opCodeSize[it->second];
}

void Assembler::initOpCodesMap()
{
    if (!_opCodesMap.empty())
        return;
    for (int i = 0; i < (int)(sizeof (_opCodes) / sizeof (OpCode)); i++) {
        _opCodesMap[_opCodes[i]] = i;
    }
}

void Assembler::disassemble(const std::vector<ContainerElementType> &objectFile, std::ostream &out)
{
    size_t i = 0;
    const uint8_t *p = objectFile.data();
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
            out << mnemonics(opCode) << std::endl;
        } catch (const std::out_of_range &e) {
            if (*(p + i) == 'F') {
                if (strncmp((char*)p + i, fnMarker, 4) == 0) {
                    i += 4;
                    int32_t dataLen = *(int32_t*)(p + i);
                    i += dataLen;
                    continue;
                }
            }
            throw;
        }
    }
    assert (n == i);
}

} // namespace escript