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
        StatementList();
    }
}


//////////////////////// Синтаксические правила ///////////////////////////////

void Parser::StatementList()
{
    do {
        switch (lookahead()) {
        case Token::Eof:
            return;
        case Token::Semicolon:
            next();
            continue;
        default:
            Statement();
            break;
        }
    } while (true);
}

void Parser::Statement()
{
    AssignStatement();
}

void Parser::AssignStatement()
{
    Variable();
    match(Token::Assign);
    Expression();
}

void Parser::Variable()
{
    match(Token::Identifier);
}

void Parser::Expression()
{
    SimpleExpression();
}

void Parser::SimpleExpression()
{
    Term();
    do {
        switch (lookahead()) {
        case Token::Plus:
            next();
            Term();
            break;
        case Token::Minus:
            next();
            Term();
            break;
        default:
            return;
        }
    } while (true);
}

void Parser::Term()
{
    Factor();
    do {
        switch (lookahead()) {
        case Token::Asterisk: // умножение
            next();
            Factor();
            break;
        case Token::Slash: // деление
            next();
            Factor();
            break;
        default:
            return;
        }
    } while (true);
}

void Parser::Factor()
{
    switch (lookahead()) {
    case Token::Identifier:
        next();
        break;
    case Token::IntegerNumber:
        next();
        break;
    default: // ошибка, нужен терминал в виде числа, идентификатора и т.п.
        expected(Token::Identifier);
    }
}

void Parser::match(Token t)
{
    if (lookahead() != t)
        expected(t);
    next();
}

void Parser::next()
{
    _lexer->next();
}

Token Parser::lookahead()
{
    return _lexer->currentToken();
}

/////////////////////// обработка ошибок //////////////////////////////////////

void Parser::error(const std::string &msg)
{
    std::cout << "Line: " << _lexer->line()
              << ", position: " << _lexer->pos()
              << ": ";
    std::cout << msg << std::endl;
    exit(1);
}

void Parser::expected(Token expectedToken)
{
    error("Expected " + TokenName::toString(expectedToken));
}

void Parser::unexpected(Token unexpectedToken)
{
    error("Unexpected " + TokenName::toString(unexpectedToken));
}

} // namespace escript
