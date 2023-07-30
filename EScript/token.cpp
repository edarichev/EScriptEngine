/**
 * @file token.cpp
 * @brief Реализация проверок токенов
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
"Finally", "For", "Function",
"If", "Implements", "Import",
"In", "Instanceof", "Interface",
"Let", "New", "Package",
"Private", "Protected", "Public",
"Return", "Static", "Super",
"Switch", "This", "Throw",
"Try", "Typeof", "Var",
"Void", "While", "With",
"Yield", "Dot", "LeftParenth",
"LeftBrace", "LeftBracket", "RightParenth",
"RightBrace", "RightBracket", "Plus",
"PlusPlus", "PlusAssign", "Minus",
"MinusMinus", "MinusAssign", "Caret",
"XorAssign", "Exclamation", "NotEqua",
"NotIdentity", "Asterisk", "MulAssign",
"Slash", "SlashAssign", "Percent",
"PercentAssign", "Less", "LessEqua",
"LShift", "LShiftAssign", "Greater",
"GreaterEqua", "RShift", "RShiftAssign",
"RShiftZero", "RShiftZeroAssign", "Assign",
"Equa", "Identity", "Ampersand",
"LogicalAnd", "BitAndAssign", "Vertica",
"LogicalOr", "BitOrAssign", "BitNot",
"BitNotAssign", "Question", "Colon",
"Comma", "Semicolon", "RealNumber",
"IntegerNumber", "QuotedString", "BackQuotedString",
"Identifier"};

const Token TokenName::_values[] = {
Token::Eof, Token::Break, Token::Case,
Token::Catch, Token::Class, Token::Const,
Token::Continue, Token::Debugger, Token::Default,
Token::Delete, Token::Do, Token::Else,
Token::Enum, Token::Export, Token::Extends,
Token::Finally, Token::For, Token::Function,
Token::If, Token::Implements, Token::Import,
Token::In, Token::Instanceof, Token::Interface,
Token::Let, Token::New, Token::Package,
Token::Private, Token::Protected, Token::Public,
Token::Return, Token::Static, Token::Super,
Token::Switch, Token::This, Token::Throw,
Token::Try, Token::Typeof, Token::Var,
Token::Void, Token::While, Token::With,
Token::Yield, Token::Dot, Token::LeftParenth,
Token::LeftBrace, Token::LeftBracket, Token::RightParenth,
Token::RightBrace, Token::RightBracket, Token::Plus,
Token::PlusPlus, Token::PlusAssign, Token::Minus,
Token::MinusMinus, Token::MinusAssign, Token::Caret,
Token::XorAssign, Token::Exclamation, Token::NotEqual,
Token::NotIdentity, Token::Asterisk, Token::MulAssign,
Token::Slash, Token::SlashAssign, Token::Percent,
Token::PercentAssign, Token::Less, Token::LessEqual,
Token::LShift, Token::LShiftAssign, Token::Greater,
Token::GreaterEqual, Token::RShift, Token::RShiftAssign,
Token::RShiftZero, Token::RShiftZeroAssign, Token::Assign,
Token::Equal, Token::Identity, Token::Ampersand,
Token::LogicalAnd, Token::BitAndAssign, Token::Vertical,
Token::LogicalOr, Token::BitOrAssign, Token::BitNot,
Token::BitNotAssign, Token::Question, Token::Colon,
Token::Comma, Token::Semicolon, Token::RealNumber,
Token::IntegerNumber, Token::QuotedString, Token::BackQuotedString,
Token::Identifier};

std::string TokenName::toString(Token token)
{
    static std::map<Token, std::string> tokens;
    if (tokens.empty()) {
        int n = sizeof (_values) / sizeof (Token);
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
