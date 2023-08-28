/**
 * @file token.cpp
 * @brief Реализация проверок токенов
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
#include "token.h"


namespace escript {

const char *TokenName::_names[] = {
"Eof", "Break", "Case",
"Catch", "Class", "Const",
"Continue", "Debugger", "Default",
"Delete", "Do", "Else",
"Enum", "Export", "Extends",
"False", "Finally", "For",
"Function", "If", "Implements",
"Import", "In", "Instanceof",
"Interface", "Let", "New",
"Null", "Package", "Private",
"Protected", "Public", "Return",
"Static", "Super", "Switch",
"This", "Throw", "Try",
"True", "Typeof", "Undefined",
"Var", "Void", "While",
"With", "Yield", "Dot",
"LeftParenth", "LeftBrace", "LeftBracket",
"RightParenth", "RightBrace", "RightBracket",
"Plus", "PlusPlus", "PlusAssign",
"Minus", "MinusMinus", "MinusAssign",
"Caret", "XorAssign", "Exclamation",
"NotEqual", "NotIdentity", "Asterisk",
"MulAssign", "Slash", "SlashAssign",
"Percent", "PercentAssign", "Less",
"LessEqual", "LShift", "LShiftAssign",
"Greater", "GreaterEqual", "RShift",
"RShiftAssign", "RShiftZero", "RShiftZeroAssign",
"Assign", "Equal", "Identity",
"Ampersand", "LogicalAnd", "BitAndAssign",
"Vertical", "LogicalOr", "BitOrAssign",
"BitNot", "Question",
"Colon", "Comma", "Semicolon",
"RealNumber", "IntegerNumber", "QuotedString",
"BackQuotedString", "Identifier", "NCO"};

const Token TokenName::_values[] = {
Token::Eof, Token::Break, Token::Case,
Token::Catch, Token::Class, Token::Const,
Token::Continue, Token::Debugger, Token::Default,
Token::Delete, Token::Do, Token::Else,
Token::Enum, Token::Export, Token::Extends,
Token::False, Token::Finally, Token::For,
Token::Function, Token::If, Token::Implements,
Token::Import, Token::In, Token::Instanceof,
Token::Interface, Token::Let, Token::New,
Token::Null, Token::Package, Token::Private,
Token::Protected, Token::Public, Token::Return,
Token::Static, Token::Super, Token::Switch,
Token::This, Token::Throw, Token::Try,
Token::True, Token::Typeof, Token::Undefined,
Token::Var, Token::Void, Token::While,
Token::With, Token::Yield, Token::Dot,
Token::LeftParenth, Token::LeftBrace, Token::LeftBracket,
Token::RightParenth, Token::RightBrace, Token::RightBracket,
Token::Plus, Token::PlusPlus, Token::PlusAssign,
Token::Minus, Token::MinusMinus, Token::MinusAssign,
Token::Caret, Token::XorAssign, Token::Exclamation,
Token::NotEqual, Token::NotIdentity, Token::Asterisk,
Token::MulAssign, Token::Slash, Token::SlashAssign,
Token::Percent, Token::PercentAssign, Token::Less,
Token::LessEqual, Token::LShift, Token::LShiftAssign,
Token::Greater, Token::GreaterEqual, Token::RShift,
Token::RShiftAssign, Token::RShiftZero, Token::RShiftZeroAssign,
Token::Assign, Token::Equal, Token::Identity,
Token::Ampersand, Token::LogicalAnd, Token::BitAndAssign,
Token::Vertical, Token::LogicalOr, Token::BitOrAssign,
Token::BitNot, Token::Question,
Token::Colon, Token::Comma, Token::Semicolon,
Token::RealNumber, Token::IntegerNumber, Token::QuotedString,
Token::BackQuotedString, Token::Identifier, Token::NCO};

std::string TokenName::toString(Token token)
{
    static std::map<Token, std::string> tokens;
    if (tokens.empty()) {
        const int m = sizeof (_names) / sizeof (char*);
        const int n = sizeof (_values) / sizeof (Token);
        static_assert (m == n);
        for (int i = 0; i < n; i++) {
            tokens[_values[i]] = _names[i];
        }
    }
    auto it = tokens.find(token);
    if (it == tokens.end())
        return "UNKNOWN";
    return it->second;
}

} // namespace escript
