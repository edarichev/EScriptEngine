/**
 * @file translator.cpp
 * @brief Транслятор трёхадресного кода в целевой
 */
#include "stdafx.h"
#include "translator.h"
#include "pvalue.h"

using std::cout;
using std::endl;

namespace escript {

// для конвертации операций
std::map<OperationType, ArithmeticOperation> Translator::optypes;

Translator::Translator()
{
    if (optypes.empty()) {
        optypes = {
            {OperationType::Add, ArithmeticOperation::Add },
            {OperationType::Minus, ArithmeticOperation::Sub },
            {OperationType::Multiply, ArithmeticOperation::Mul },
            {OperationType::Div, ArithmeticOperation::Div }
        };
    }
}

void Translator::translate(std::shared_ptr<Block> block,
                           const std::vector<TCode> &inputBuffer,
                           std::vector<uint8_t> &outBuffer)
{
    _block = block;
    _asm = std::make_unique<Assembler>(outBuffer);
    Assembler &a = as();
    a.jmp_m(0); // сюда запишем смещение в конце
    writeVariableSection(block, outBuffer);

    const char codeHeader[] = {'C', 'O', 'D', 'E'};
    outBuffer.insert(outBuffer.end(),
                     codeHeader, codeHeader + sizeof (codeHeader));
    uint32_t codeSegmentLengthPosition = outBuffer.size();
    uint32_t codeSegmentLength = 0;
    outBuffer.insert(outBuffer.end(),
                     (uint8_t*)&codeSegmentLength,
                     (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength));
    uint64_t startPosition = outBuffer.size();
    for (auto &c : inputBuffer) {
        translateOperation(c);
    }
    as().halt();
    // записать длину сегмента кода
    codeSegmentLength = outBuffer.size() - codeSegmentLengthPosition - sizeof (codeSegmentLength);
    std::copy((uint8_t*)&codeSegmentLength,
              (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength),
              outBuffer.begin() + codeSegmentLengthPosition);
    // записать адрес перехода в начале объектного файла
    std::copy((uint8_t*)&startPosition,
              (uint8_t*)&startPosition + sizeof (startPosition),
              outBuffer.begin() + sizeof(OpCodeType));
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
    case OperationType::Div:
    case OperationType::Minus:
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
    if (tryCalcBinaryOp(c))
        return;
    Assembler &a = as();
    emit_ldc(c.operand1Type, c.operand1);
    emit_ldc(c.operand2Type, c.operand2);

    // действие над элементами
    switch (c.operation) {
    case OperationType::Add:
        a.addst();
        break;
    case OperationType::Multiply:
        a.mulst();
        break;
    case OperationType::Minus:
        a.subst();
        break;
    case OperationType::Div:
        a.divst();
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
    case SymbolType::Real:
        a.ldc_double_data64(c.operand1.realValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(c.operand1.variable));
        break;
    default:
        throw std::domain_error("Unsupported operand type for assign operation");
    }
    a.stloc_m(location(c.lvalue));
}

void Translator::emit_ldc(SymbolType type, const OperandRecord &operand)
{
    Assembler &a = as();
    switch (type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(operand.intValue);
        break;
    case SymbolType::Real:
        a.ldc_double_data64(operand.realValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(operand.variable));
        break;
    default:
        throw std::domain_error("Unsupported type");
    }
}

bool Translator::tryCalcBinaryOp(const TCode &c)
{
    Assembler &a = as();
    // во второй части чаще переменные, поэтому сначала проверим второй операнд
    PValue value1, value2;
    switch (c.operand2Type) {
    case SymbolType::Integer:
        value2 = c.operand2.intValue;
        break;
    case SymbolType::Real:
        value2 = c.operand2.realValue;
        break;
        break;
    default:
        return false;
    }
    switch (c.operand1Type) {
    case SymbolType::Integer:
        value1 = c.operand1.intValue;
        break;
    case SymbolType::Real:
        value1 = c.operand1.realValue;
        break;
    default:
        return false;
    }
    // это должно быть гарантировано, если это не так, пусть вылетит
    ArithmeticOperation op = optypes.find(c.operation)->second;
    // эта часть почти совпадает с void Processor::binaryStackOp(OpCode opCode)
    PValue result = PValue::binaryOpValues(value1, value2, op);
    switch (result.type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(result.intValue);
        break;
    case SymbolType::Real:
        a.ldc_double_data64(result.realValue);
        break;
    default:
        break; // уже проверено
    }
    a.stloc_m(location(c.lvalue));
    return true;
}

} // namespace escript
