/**
 * @file opcode.h
 * @brief Инструкция трёхадресного кода
 */
#ifndef ESCRIPT_OPCODE_H
#define ESCRIPT_OPCODE_H

#include "EScript_global.h"
#include "operandrecord.h"

namespace escript {


/**
 * @brief Тип трёхадресной операции
 */
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
    FunctionStart,  // FNSTART - начало блока функции, op1==Symbol*
    FunctionArgument, // аргумент функции+Symbol*
    FunctionCode,   // начало блока кода (стартовая точка запуска) функции
    LoadArguments,  // загрузка аргументов из стека на входе в функцию
    Push,           // push op1
    Pop,            // pop op1
    Ret,            // возврат из функции
    Call,           // вызов функции, addr==op1.intValue
    FunctionEnd,    // конец функции
    BlockStart,     // op1==Block*
    BlockEnd,       // op1==Block*
};

/**
 * @brief Инструкция трёхадресного кода
 */
struct ESCRIPT_EXPORT TCode
{
public:
    OperationType operation = OperationType::None;
    Symbol *lvalue = nullptr;
    OperandRecord operand1 {};
    OperandRecord operand2 {};
    std::string toString() const;
};

} // namespace escript

#endif // ESCRIPT_OPCODE_H
