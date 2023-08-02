/**
 * @file opcode.cpp
 * @brief Инструкция трёхадресного кода
 */
#include "stdafx.h"
#include "tcode.h"
#include "symbol.h"
#include "types.h"

namespace escript {

std::string TCode::toString() const
{
    std::string op;
    int numberOfOperands = 0;
    std::string opSign;
    std::string assign = ":=";
    switch (operation) {
    case OperationType::None:
        op = "NONE";
        break;
    case OperationType::Add:
        op = "+";
        opSign = assign;
        numberOfOperands = 2;
        break;
    case OperationType::Multiply:
        op = "*";
        numberOfOperands = 2;
        opSign = assign;
        break;
    case OperationType::Assign:
        numberOfOperands = 1;
        opSign = assign;
        break;
    case OperationType::UMinus:
        numberOfOperands = 1;
        opSign = assign + " UNIMUS";
        break;
    }
    std::string result;
    if (lvalue)
        result.append(lvalue->utf8Name());
    if (opSign.length()) {
        result.append(" ").append(opSign).append(" ");
    }
    if (numberOfOperands > 0) {
        result.append(operandToString(operand1Type, operand1));
        if (numberOfOperands > 1) {
            result.push_back(' ');
            result.append(op);
            result.push_back(' ');
            result.append(operandToString(operand2Type, operand2));
        }
    }
    return result;
}

void TCode::setOperand1(SymbolType operandType, void *value)
{
    setOperand(operand1Type, operand1, operandType, value);
}

void TCode::setOperand2(SymbolType operandType, void *value)
{
    setOperand(operand2Type, operand2, operandType, value);
}

void TCode::setOperand(SymbolType &operandType, OperandRecord &op,
                        SymbolType newOperandType, void *value)
{
    operandType = newOperandType;
    switch (operandType) {
    case SymbolType::Integer:
        op.intValue = *(IntType*)value;
        break;
    case SymbolType::Real:
        op.realValue = *(RealType*)value;
        break;
    case SymbolType::Variable:
        op.variable = (Symbol*)value;
        break;
    default:
        throw std::domain_error("Unsupported SymbolType");
    }
}

std::string TCode::operandToString(SymbolType operandType, OperandRecord op) const
{
    std::string opStr;
    switch (operandType) {
    case SymbolType::Integer:
        opStr = std::to_string(op.intValue);
        break;
    case SymbolType::Real:
        opStr = std::to_string(op.realValue);
        break;
    case SymbolType::Variable:
        opStr = op.variable->utf8Name();
        break;
    default:
        throw std::domain_error("Unsupported SymbolType");
    }
    return opStr;
}



} // namespace escript
