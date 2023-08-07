/**
 * @file icodeemitter.cpp
 * @brief Генератор промежуточного кода - реализация
 */
#include "stdafx.h"
#include "icodeemitter.h"

using std::cout;
using std::endl;

namespace escript {

ICodeEmitter::ICodeEmitter(std::vector<TCode> &buffer)
    : _mainBuffer(&buffer), _buffer(&buffer)
{

}

void ICodeEmitter::binaryOp(OperationType operationType,
                            Symbol *resultVariable,
                            SymbolType operand1Type,
                            const OperandRecord &operand1,
                            SymbolType operand2Type,
                            const OperandRecord &operand2)
{
    switch (operationType) {
    case OperationType::Multiply:
    case OperationType::Add:
    case OperationType::Div:
    case OperationType::Minus:
    case OperationType::Less:
    case OperationType::LessOrEqual:
    case OperationType::Greater:
    case OperationType::GreaterOrEqual:
    case OperationType::Equal:
        break;
    default:
        throw std::domain_error("Invalid binary operation");
    }
    TCode code;
    code.lvalue = resultVariable;
    code.operand1Type = operand1Type;
    code.operand1 = operand1;
    code.operand2Type = operand2Type;
    code.operand2 = operand2;
    code.operation = operationType;
    _buffer->push_back(code);
}

void ICodeEmitter::assign(Symbol *lvalue, SymbolType rvalueType,
                          const OperandRecord &operand1)
{
    TCode code;
    code.lvalue = lvalue;
    code.operation = OperationType::Assign;
    code.operand1Type = rvalueType;
    code.operand1 = operand1;
    _buffer->push_back(code);
}

void ICodeEmitter::unaryOp(OperationType operationType, Symbol *resultVariable,
                           SymbolType operand1Type, const OperandRecord &operand1)
{
    switch (operationType) {
    case OperationType::UMinus:
        break;
    default:
        throw std::domain_error("Invalid unary operation");
    }
    TCode code;
    code.lvalue = resultVariable;
    code.operand1Type = operand1Type;
    code.operand1 = operand1;
    code.operation = operationType;
    _buffer->push_back(code);
}

void ICodeEmitter::iffalse(Symbol *variableToTest, int exitOrFalseLabelId)
{
    TCode code;
    code.operation = OperationType::IfFalse;
    code.operand1Type = SymbolType::Variable;
    code.operand1.variable = variableToTest;
    // номер метки:
    code.operand2Type = SymbolType::Integer;
    code.operand2.intValue = exitOrFalseLabelId;
    _buffer->push_back(code);
}

void ICodeEmitter::goToLabel(int labelId)
{
    TCode code;
    code.operation = OperationType::Goto;
    code.operand1Type = SymbolType::Integer;
    code.operand1.intValue = labelId;
    _buffer->push_back(code);
}

void ICodeEmitter::label(int labelId)
{
    TCode code;
    code.operation = OperationType::Label;
    code.operand1Type = SymbolType::Integer;
    code.operand1.intValue = labelId;
    _buffer->push_back(code);
}

void ICodeEmitter::switchToTempBuffer()
{
    _buffer = &_tmpBuffer;
}

void ICodeEmitter::switchToMainBuffer()
{
    _buffer = _mainBuffer;
}

void ICodeEmitter::writeTempBuffer()
{
    _buffer->insert(_buffer->end(), _tmpBuffer.begin(), _tmpBuffer.end());
    _tmpBuffer.clear();
}

} // namespace escript

