/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"
#include "unit.h"

using namespace std;

namespace escript {

Parser::Parser(std::shared_ptr<Unit> &unit)
    : _unit(unit)
{
    _emitter = std::make_unique<ICodeEmitter>();
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
    _types.pop(); // пока всегда SymbolType::Variable
    auto lvalue = _variables.top();
    _variables.pop();
    match(Token::Assign);
    Expression();
    // после Expression всегда что-то есть в стеке
    void *rvalue = nullptr;
    auto rvalueType = _types.top();
    _types.pop();
    IntType irvalue;
    switch (rvalueType) {
    case SymbolType::Variable:
        rvalue = _variables.top().get();
        _variables.pop();
        break;
    case SymbolType::Integer:
        irvalue = _integers.top();
        rvalue = &irvalue;
        _integers.pop();
        break;
    }
    emitAssign(lvalue, rvalueType, rvalue);
}

void Parser::Variable()
{ // это правило исключительно для l-value
    expected(Token::Identifier);
    auto id = tokenText();
    // отыскиваем переменную в таблице символов
    auto symbol = currentSymbolTable()->find(id);
    if (!symbol) // или добавляем новую запись
        symbol = currentSymbolTable()->add(id);
    pushVariable(symbol);
    match(Token::Identifier);
}

void Parser::Expression()
{
    SimpleExpression();
}

void Parser::SimpleExpression()
{
    std::shared_ptr<Symbol> tmp;
    Term();
    do {
        switch (lookahead()) {
        case Token::Plus:
            next();
            Term();
            tmp = currentSymbolTable()->addTemp();
            emitBinaryOp(OperationType::Add, tmp);
            pushVariable(tmp);
            continue;
        case Token::Minus:
            next();
            Term();
            continue;
        default:
            break;
        }
    } while (false);
}

void Parser::Term()
{
    // временная переменная, нужна для генерации инструкции и помещения её в стек
    std::shared_ptr<Symbol> tmp;
    Factor();
    // в стеке находится результат вызова Factor(), например, число.
    do {
        switch (lookahead()) {
        case Token::Asterisk: // умножение
            next();
            Factor();
            tmp = currentSymbolTable()->addTemp();
            emitBinaryOp(OperationType::Multiply, tmp);
            pushVariable(tmp);
            continue;
        case Token::Slash: // деление
            next();
            Factor();
            continue;
        default:
            break;
        }
    } while (false);
}

void Parser::Factor()
{
    switch (lookahead()) {
    case Token::Identifier:
        next();
        break;
    case Token::IntegerNumber:
        pushInt(_lexer->lastIntegerNumber());
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

const u32string &Parser::tokenText() const
{
    return _lexer->tokenText();
}

std::shared_ptr<SymbolTable> Parser::currentSymbolTable()
{
    return _unit->block()->symbolTable();
}

void Parser::pushInt(IntType value)
{
    _integers.push(value);
    _types.push(SymbolType::Integer);
}

void Parser::pushVariable(std::shared_ptr<Symbol> &variable)
{
    _variables.push(variable);
    _types.push(SymbolType::Variable);
}

void Parser::emitBinaryOp(OperationType opType, std::shared_ptr<Symbol> &tmpVariable)
{
    switch (opType) {
    case OperationType::Add:
    case OperationType::Multiply: {
        // binaryop
        // что в стеке терминалов?
        auto op2Type = _types.top();
        _types.pop();
        auto op1Type = _types.top();
        _types.pop();
        // здесь проверка типов и по возможности результирующий тип
        // если нет, то это отдаётся на время выполнения
        void *op1ptr = nullptr;
        void *op2ptr = nullptr;
        IntType iop1, iop2;
        // обратный порядок
        switch (op2Type) {
        case SymbolType::Integer:
            // нужно временно поместить число куда-нибудь
            iop2 = _integers.top();
            op2ptr = &iop2;
            _integers.pop();
            break;
        case SymbolType::Variable:
            // запись в таблице символов
            op2ptr = _variables.top().get();
            _variables.pop();
            break;
        }
        switch (op1Type) {
        case SymbolType::Integer:
            // нужно временно поместить число куда-нибудь
            iop1 = _integers.top();
            op1ptr = &iop1;
            _integers.pop();
            break;
        case SymbolType::Variable:
            // запись в таблице символов
            op1ptr = _variables.top().get();
            _variables.pop();
            break;
        }
        _emitter->binaryOp(opType, tmpVariable,
                           op1Type, op1ptr, op2Type, op2ptr);
        break;
    }
    }
}

void Parser::emitAssign(std::shared_ptr<Symbol> &lvalue, SymbolType rvalueType, void *rvalue)
{
    _emitter->assign(lvalue.get(), rvalueType, rvalue);
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
    if (lookahead() != expectedToken)
        error("Expected " + TokenName::toString(expectedToken));
}

void Parser::unexpected(Token unexpectedToken)
{
    error("Unexpected " + TokenName::toString(unexpectedToken));
}

string Parser::toUtf8(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(s);
}

} // namespace escript
