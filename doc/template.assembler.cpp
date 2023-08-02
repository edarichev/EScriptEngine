/**
 * AUTOGENERATED
 * @file assembler.cpp
 * @date $(Date)
 * @brief Для записи инструкций в объектный файл
 */
#include "stdafx.h"
#include "assembler.h"

namespace escript {

// мнемоники
const std::string Assembler::_opCodeNames[] = {
    $(OpCodeNamesList)
};
// все коды операций
OpCode Assembler::_opCodes[] = {
    $(OpCodesList)
};
// размер в байтах каждой команды
uint8_t Assembler::_opCodeSize[] = {
    $(OpCodesSizeList)
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

} // namespace escript
