/**
 * @file opcode.h
 * @brief Инструкция трёхадресного кода
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#ifndef ESCRIPT_OPCODE_H
#define ESCRIPT_OPCODE_H

#include "EScript_global.h"
#include "operand.h"

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
    PopTo,          // pop op1
    Pop,            // просто извлечь из стека
    Ret,            // возврат из функции
    Call,           // вызов функции, addr==op1.intValue
    FunctionEnd,    // конец функции
    BlockStart,     // op1==Block*
    BlockEnd,       // op1==Block*
    CallM,          // Вызов метода объекта автоматизации
    AllocArray,     // выделение памяти под массив
    Increment,      // pop, int, push
    Decrement,      // pop, int, push
    LShift,         // <<
    RShift,         // >>
    RShiftZero,     // >>>
    BitAND,         // &
    BitOR,          // |
    BitXOR,         // ^
    LogAND,         // &&
    LogOR,          // ||
    BitNOT,         // ~
    LogNOT,         // !
    Mod,            // %
    StoreActivationRecord,
    LoadActivationRecord,
    NotEqual,       // op1!=op2
    NCO,            // op1 == null ? op2 : op1
    CTOR,           // new className(), op1.ptr = ConstructorFunction
    CHTYPE,         // type->op1.type
};

/**
 * @brief Инструкция трёхадресного кода
 */
struct ESCRIPT_EXPORT TCode
{
public:
    OperationType operation = OperationType::None;
    Symbol *lvalue = nullptr;
    Operand operand1 {};
    Operand operand2 {};
    std::string toString() const;
};

} // namespace escript

#endif // ESCRIPT_OPCODE_H
