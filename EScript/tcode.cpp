/**
 * @file opcode.cpp
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
#include "stdafx.h"
#include "tcode.h"
#include "symbol.h"
#include "types.h"

namespace escript {

std::string TCode::toString() const
{
    std::string op;
    // только для форматированного вывода, не реальное число
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
    case OperationType::Minus:
        op = "-";
        numberOfOperands = 2;
        opSign = assign;
        break;
    case OperationType::Div:
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
    case OperationType::Goto:
        numberOfOperands = 1;
        opSign = "GOTO";
        break;
    case OperationType::Label:
        numberOfOperands = 1;
        opSign = "LABEL";
        break;
    case OperationType::IfFalse:
        numberOfOperands = 2;
        opSign = "IFFALSE";
        break;
    case OperationType::Less:
        numberOfOperands = 2;
        opSign = "<";
        break;
    case OperationType::LessOrEqual:
        numberOfOperands = 2;
        opSign = "<=";
        break;
    case OperationType::Greater:
        numberOfOperands = 2;
        opSign = ">";
        break;
    case OperationType::GreaterOrEqual:
        numberOfOperands = 2;
        opSign = ">=";
        break;
    case OperationType::Equal:
        numberOfOperands = 2;
        opSign = "==";
        break;
    case OperationType::NotEqual:
        numberOfOperands = 2;
        opSign = "!=";
        break;
    case OperationType::FunctionStart:  // FNSTART - начало блока функции, op1==Symbol*
        numberOfOperands = 1;
        opSign = "FNSTART";
        break;
    case OperationType::FunctionArgument: // аргумент функции+Symbol*
        numberOfOperands = 1;
        opSign = "FNARG";
        break;
    case OperationType::FunctionCode:   // начало блока кода (стартовая точка запуска) функции
        numberOfOperands = 0;
        opSign = "FNCODE";
        break;
    case OperationType::LoadArguments:  // загрузка аргументов из стека на входе в функцию
        numberOfOperands = 0;
        opSign = "LDARGS";
        break;
    case OperationType::Push:           // push op1
        numberOfOperands = 1;
        opSign = "PUSH";
        break;
    case OperationType::PopTo:           // pop op1
        numberOfOperands = 1;
        opSign = "POP";
        break;
    case OperationType::Ret:            // возврат из функции
        numberOfOperands = 2;
        opSign = "RET";
        break;
    case OperationType::Call:           // вызов функции, addr==op1.intValue
        numberOfOperands = 1;
        opSign = "CALL";
        break;
    case OperationType::FunctionEnd:    // конец функции
        numberOfOperands = 0;
        opSign = "FNEND";
        break;
    case OperationType::BlockStart:
        numberOfOperands = 0;
        opSign = "BLOCK START";
        break;
    case OperationType::BlockEnd:
        numberOfOperands = 0;
        opSign = "BLOCK END";
        break;
    case OperationType::CallM:
        numberOfOperands = 0;
        opSign = "CALLM";
        break;
    case OperationType::AllocArray:
        numberOfOperands = 0;
        opSign = "ALLOCARRAY";
        break;
    case OperationType::Increment:
        numberOfOperands = 0;
        opSign = "INC";
        break;
    case OperationType::Decrement:
        numberOfOperands = 0;
        opSign = "DEC";
        break;
    case OperationType::Pop:
        numberOfOperands = 0;
        opSign = "POP";
        break;
    case OperationType::LShift:
        numberOfOperands = 0;
        opSign = "LSH";
        break;
    case OperationType::RShift:
        numberOfOperands = 0;
        opSign = "RSH";
        break;
    case OperationType::RShiftZero:
        numberOfOperands = 0;
        opSign = "RSHZ";
        break;
    case OperationType::BitAND:
        numberOfOperands = 0;
        opSign = "BIT.AND";
        break;
    case OperationType::BitOR:
        numberOfOperands = 0;
        opSign = "BIT.OR";
        break;
    case OperationType::BitXOR:
        numberOfOperands = 0;
        opSign = "BIT.XOR";
        break;
    case OperationType::BitNOT:
        numberOfOperands = 0;
        opSign = "BIT.NOT";
        break;
    case OperationType::LogAND:
        numberOfOperands = 0;
        opSign = "LOG.AND";
        break;
    case OperationType::LogOR:
        numberOfOperands = 0;
        opSign = "LOG.OR";
        break;
    case OperationType::LogNOT:
        numberOfOperands = 0;
        opSign = "LOG.NOT";
        break;
    case OperationType::Mod:
        numberOfOperands = 0;
        opSign = "MOD";
        break;
    case OperationType::StoreActivationRecord:
        numberOfOperands = 0;
        opSign = "ST.AR";
        break;
    case OperationType::LoadActivationRecord:
        numberOfOperands = 0;
        opSign = "LD.AR";
        break;
    }
    std::string result;
    if (lvalue)
        result.append(lvalue->utf8Name());
    if (opSign.length()) {
        if (result.length())
            result.append(" ");
        result.append(opSign).append(" ");
    }
    if (numberOfOperands > 0) {
        result.append(operand1.toString());
        if (numberOfOperands > 1) {
            result.push_back(' ');
            result.append(op);
            result.push_back(' ');
            result.append(operand2.toString());
        }
    }
    return result;
}





} // namespace escript
