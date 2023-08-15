
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

