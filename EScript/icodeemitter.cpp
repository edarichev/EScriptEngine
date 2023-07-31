/**
 * @file icodeemitter.cpp
 * @brief Генератор промежуточного кода - реализация
 */
#include "stdafx.h"
#include "icodeemitter.h"

using std::cout;
using std::endl;

namespace escript {

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
    OpCode code;
    code.lvalue = resultVariable;
    code.setOperand1(operand1Type, operand1);
    code.setOperand2(operand2Type, operand2);
    code.operation = operationType;
    cout << code.toString() << endl;
}

void ICodeEmitter::assign(Symbol *lvalue, SymbolType rvalueType, void *rvalue)
{
    OpCode code;
    code.lvalue = lvalue;
    code.operation = OperationType::Assign;
    code.setOperand1(rvalueType, rvalue);
    cout << code.toString() << endl;
}

} // namespace escript

