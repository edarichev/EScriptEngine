/**
 * @file lexer.cpp
 * @brief Лексический анализатор - реализация
 */
#include "stdafx.h"
#include "lexer.h"

namespace escript {

Lexer::Lexer(const std::u32string &strCode)
    : _text(strCode)
{

}

Lexer::~Lexer()
{

}

Token Lexer::next()
{
    return Token::Eof;
}

Token Lexer::token() const
{
    return _token;
}

const std::u32string &Lexer::tokenText() const
{
    return _tokenText;
}

} // namespace escript
