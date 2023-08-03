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
    _block = block;
    _asm = std::make_unique<Assembler>(outBuffer);
    writeVariableSection(block, outBuffer);
    const char codeHeader[] = {'C', 'O', 'D', 'E'};
    outBuffer.insert(outBuffer.end(),
                     codeHeader, codeHeader + sizeof (codeHeader));
    uint32_t codeSegmentLengthPosition = outBuffer.size();
    uint32_t codeSegmentLength = 0;
    outBuffer.insert(outBuffer.end(),
                     (uint8_t*)&codeSegmentLength,
                     (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength));
    for (auto &c : inputBuffer) {
        translateOperation(c, outBuffer);
    }
    as().halt();
    // записать длину сегмента кода
    codeSegmentLength = outBuffer.size() - codeSegmentLengthPosition - sizeof (codeSegmentLength);
    std::copy((uint8_t*)&codeSegmentLength,
              (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength),
              outBuffer.begin() + codeSegmentLengthPosition);
    _asm.reset();   // больше не нужен
    _block.reset(); // тоже не нужен
}

void Translator::writeVariableSection(std::shared_ptr<Block> block,
                                      std::vector<uint8_t> &outBuffer)
{
    // создаём область данных для переменных в таблице символов блока
    // пока только одна таблица
    auto table = block->symbolTable();
    const char dataHeader[] = {'D', 'A', 'T', 'A'};
    outBuffer.insert(outBuffer.end(),
                     dataHeader, dataHeader + sizeof (dataHeader));
    uint32_t dataHeaderLength = sizeof(PtrIntType) * table->size();
    outBuffer.insert(outBuffer.end(),
                     (uint8_t*)&dataHeaderLength,
                     (uint8_t*)&dataHeaderLength + sizeof (dataHeaderLength));

    PtrIntType empty {};
    // начинаем с самого верха
    for (auto &record : *table) {
        // все записи в этой области представляют собой указатели
        // на объекты в спсике объектов
        // забиваем здесь место под размеры этих указателей
        auto symbol = record.second;
        // записать смещение
        symbol->setLocation(outBuffer.size());
        outBuffer.insert(outBuffer.end(),
                         (uint8_t*)&empty,
                         (uint8_t*)&empty + sizeof (empty));
    }

}

void Translator::translateOperation(const TCode &c, std::vector<uint8_t> &outBuffer)
{
    switch (c.operation) {
    case OperationType::Add:
        opAdd(c);
        break;
    case OperationType::Multiply:
        opMul(c);
        break;
    case OperationType::Assign:
        opAssign(c);
        break;
    case OperationType::UMinus:
        opUMinus(c);
        break;
    }
    cout << c.toString() << endl;
}

PtrIntType Translator::location(Symbol *symbol)
{
    // потом видно будет, что именно тут надо
    return symbol->location();
}

void Translator::opAdd(const TCode &c)
{
    // 1. Два целых операнда: сразу вычисляем
    // #tmp_1 := 3 + 2
    //
    Assembler &a = as();
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Integer) {
        int64_t sum = c.operand1.intValue + c.operand2.intValue;
        a.ldc_int64_data64(sum);
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Variable) {
        a.ldc_int64_data64(c.operand1.intValue);
        a.ldloc_m(location(c.operand2.variable));
        a.addst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Integer) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldc_int64_data64(c.operand2.intValue);
        a.addst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Variable) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldloc_m(location(c.operand2.variable));
        a.addst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
}

void Translator::opMul(const TCode &c)
{
    // 1. Два целых операнда: сразу вычисляем
    // #tmp_1 := 3 * 2
    //
    Assembler &a = as();
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Integer) {
        int64_t sum = c.operand1.intValue * c.operand2.intValue;
        a.ldc_int64_data64(sum);
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Variable) {
        a.ldc_int64_data64(c.operand1.intValue);
        a.ldloc_m(location(c.operand2.variable));
        a.mulst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Integer) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldc_int64_data64(c.operand2.intValue);
        a.mulst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Variable) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldloc_m(location(c.operand2.variable));
        a.mulst();
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
}

void Translator::opUMinus(const TCode &c)
{
    Assembler &a = as();
    if (c.operand1Type == SymbolType::Integer) {
        a.ldc_int64_data64(c.operand1.intValue);
        a.neg();
        a.stloc_m(location(c.lvalue));
        return;
    }
    if (c.operand1Type == SymbolType::Variable) {
        a.ldloc_m(location(c.operand1.variable));
        a.neg();
        a.stloc_m(location(c.lvalue));
        return;
    }
}

void Translator::opAssign(const TCode &c)
{
    Assembler &a = as();
    if (c.operand1Type == SymbolType::Integer) {
        a.ldc_int64_data64(c.operand1.intValue);
        a.stloc_m(location(c.lvalue));
        return;
    }
    if (c.operand1Type == SymbolType::Variable) {
        a.ldloc_m(location(c.operand1.variable));
        a.stloc_m(location(c.lvalue));
        return;
    }
}

} // namespace escript
