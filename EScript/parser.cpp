/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"

namespace escript {

Parser::Parser()
{

}

Parser::~Parser()
{

}

void Parser::parse(const std::u32string &strCode)
{
    _lexer = std::make_unique<Lexer>(strCode);
    while (_lexer->next() != Token::Eof) {

    }
}

} // namespace escript
