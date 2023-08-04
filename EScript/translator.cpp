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
        translateOperation(c);
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
        // на объекты в списке объектов
        // забиваем здесь место под размеры этих указателей
        auto symbol = record.second;
        // записать смещение
        symbol->setLocation(outBuffer.size());
        outBuffer.insert(outBuffer.end(),
                         (uint8_t*)&empty,
                         (uint8_t*)&empty + sizeof (empty));
    }
}

void Translator::translateOperation(const TCode &c)
{
    switch (c.operation) {
    case OperationType::Add:
    case OperationType::Multiply:
        binaryOp(c);
        break;
    case OperationType::Assign:
        opAssign(c);
        break;
    case OperationType::UMinus:
        opUMinus(c);
        break;
    default:
        throw std::domain_error("Can not translate operation: " + c.toString());
    }
    cout << c.toString() << endl;
}

PtrIntType Translator::location(Symbol *symbol)
{
    return (PtrIntType)symbol;
}

void Translator::binaryOp(const TCode &c)
{
    // 1. Два целых операнда: сразу вычисляем
    // #tmp_1 := 3 операция 2
    //
    Assembler &a = as();
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Integer) {
        int64_t result = 0;
        switch (c.operation) {
        case OperationType::Add:
            result = c.operand1.intValue + c.operand2.intValue;
            break;
        case OperationType::Multiply:
            result = c.operand1.intValue * c.operand2.intValue;
            break;
        default:
            throw std::domain_error("Unsupported binary operation");
        }
        a.ldc_int64_data64(result);
        a.stloc_m(location(c.lvalue)); // смещение переменной
        return;
    }
    // 2. Целое и переменная
    // #tmp_1 := число операция #tmp_2
    if (c.operand1Type == SymbolType::Integer && c.operand2Type == SymbolType::Variable) {
        a.ldc_int64_data64(c.operand1.intValue);
        a.ldloc_m(location(c.operand2.variable));
    }
    // 3. переменная и целое
    // #tmp_1 := #tmp_2 операция число
    else if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Integer) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldc_int64_data64(c.operand2.intValue);
    }
    // 4. переменная и переменная
    // #tmp_1 := #tmp_2 операция #tmp_3
    else if (c.operand1Type == SymbolType::Variable && c.operand2Type == SymbolType::Variable) {
        a.ldloc_m(location(c.operand1.variable));
        a.ldloc_m(location(c.operand2.variable));
    }
    // действие над элементами
    switch (c.operation) {
    case OperationType::Add:
        a.addst();
        break;
    case OperationType::Multiply:
        a.mulst();
        break;
    default:
        throw std::domain_error("Unsupported binary operation");
    }
    // сохранить в значение переменную
    a.stloc_m(location(c.lvalue));
}

void Translator::opUMinus(const TCode &c)
{
    Assembler &a = as();
    switch (c.operand1Type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(c.operand1.intValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(c.operand1.variable));
        break;
    default:
        throw std::domain_error("Unsupported operand type for unary operation");
    }
    a.neg();
    a.stloc_m(location(c.lvalue));
}

void Translator::opAssign(const TCode &c)
{
    Assembler &a = as();
    switch (c.operand1Type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(c.operand1.intValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(c.operand1.variable));
        break;
    default:
        throw std::domain_error("Unsupported operand type for assign operation");
    }
    a.stloc_m(location(c.lvalue));
}

} // namespace escript
