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
    : _buffer(buffer)
{

}

void ICodeEmitter::binaryOp(OperationType operationType,
                            Symbol *resultVariable,
                            SymbolType operand1Type, void *operand1,
                            SymbolType operand2Type, void *operand2)
{

    switch (operationType) {
    case OperationType::Multiply:
    case OperationType::Add:
        break;
    default:
        throw std::domain_error("Invalid binary operation");
    }
    TCode code;
    code.lvalue = resultVariable;
    code.setOperand1(operand1Type, operand1);
    code.setOperand2(operand2Type, operand2);
    code.operation = operationType;
    _buffer.push_back(code);
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
    _buffer.push_back(code);
}

void ICodeEmitter::assign(Symbol *lvalue, SymbolType rvalueType,
                          const OperandRecord &operand1)
{
    TCode code;
    code.lvalue = lvalue;
    code.operation = OperationType::Assign;
    code.operand1Type = rvalueType;
    code.operand1 = operand1;
    _buffer.push_back(code);
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
    _buffer.push_back(code);
}

} // namespace escript

