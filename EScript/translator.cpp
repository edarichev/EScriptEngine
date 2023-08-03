/**
 * @file translator.cpp
 * @brief Транслятор трёхадресного кода в целевой
 */
#include "stdafx.h"
#include "translator.h"

using std::cout;
using std::endl;

namespace escript {

Translator::Translator()
{

}

void Translator::translate(std::shared_ptr<Block> block,
                           const std::vector<TCode> &inputBuffer,
                           std::vector<uint8_t> &outBuffer)
{
    writeVariableSection(block, outBuffer);
    for (auto &c : inputBuffer) {
        switch (c.operation) {
        default:
            break;
        }
    }
}

void Translator::writeVariableSection(std::shared_ptr<Block> block,
                                      std::vector<uint8_t> &outBuffer)
{
    // создаём область данных для переменных в таблице символов блока
    // пока только одна таблица
    auto table = block->symbolTable();
    for (auto &record : *table) {
        auto symbol = record.second;
        switch (symbol->type()) {
        case SymbolType::Integer:
            cout << symbol->utf8Name() << endl;
            break;
        default:
            break;
        }
    }
}

} // namespace escript
