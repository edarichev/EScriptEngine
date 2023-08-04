/**
 * @file opcode.h
 * @brief Инструкция трёхадресного кода
 */
#ifndef ESCRIPT_OPCODE_H
#define ESCRIPT_OPCODE_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;

enum class OperationType
{
    None,
    Assign,         // lvalue = op1
    Multiply,       // op1*op2
    Add,            // op1+op2
    Div,            // op1/op2
    Minus,          // op1-op2
    UMinus,         // -op1
};

union OperandRecord
{
    IntType intValue;
    RealType realValue;
    Symbol *variable;
};

/**
 * @brief Инструкция трёхадресного кода
 */
struct ESCRIPT_EXPORT TCode
{
public:
    OperationType operation = OperationType::None;
    Symbol *lvalue = nullptr;
    SymbolType operand1Type = SymbolType::Undefined;
    SymbolType operand2Type = SymbolType::Undefined;
    OperandRecord operand1 {};
    OperandRecord operand2 {};
    std::string toString() const;
    void setOperand1(SymbolType operandType, void *value);
    void setOperand2(SymbolType operandType, void *value);
private:
    void setOperand(SymbolType &operandType, OperandRecord &op, SymbolType newOperandType, void *value);
    std::string operandToString(SymbolType operandType, OperandRecord op) const;
};

} // namespace escript

#endif // ESCRIPT_OPCODE_H
