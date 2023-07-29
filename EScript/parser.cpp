/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"

using namespace std;

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
        wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
        cout << converter.to_bytes(_lexer->tokenText()) << " ";
    }
}

} // namespace escript
