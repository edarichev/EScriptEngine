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
    IfFalse,        // iffalse op1 метка op2.intValue
    Goto,           // Goto op1.intValue
    Label,          // op1.intValue
    Less,           // op1<op2
    LessOrEqual,    // op1<=op2
    Greater,        // op1>op2
    GreaterOrEqual, // op1>=op2
    Equal,          // op1==op2
};

union OperandRecord
{
    IntType intValue;
    RealType realValue;
    bool boolValue;
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
private:
    std::string operandToString(SymbolType operandType, OperandRecord op) const;
};

} // namespace escript

#endif // ESCRIPT_OPCODE_H
