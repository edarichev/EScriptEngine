/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"
#include "unit.h"

using namespace std;

namespace escript {

Parser::Parser(std::shared_ptr<Unit> &unit, std::vector<TCode> &outBuffer)
    : _unit(unit)
{
    _emitter = std::make_unique<ICodeEmitter>(outBuffer);
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
            match(Token::Semicolon);
            break;
        }
    } while (true);
}

void Parser::Statement()
{
    AssignStatement();
    // после AssignStatement всегда есть что-то, т.к. это выражение,
    // поэтому вытаскиваем из стека его результат
    popStackValue();
}

void Parser::AssignStatement()
{
    Variable();
    auto lvalueSymbol = _variables.top();
    popStackValue(); // пока всегда SymbolType::Variable
    match(Token::Assign);
    Expression();
    emitAssign(lvalueSymbol);
    // поскольку присваивание само является выражением, нужно поместить в стек
    // результат вычислений
    pushVariable(lvalueSymbol);
}

void Parser::Variable()
{ // это правило исключительно для l-value
    if (lookahead() == Token::Identifier) {
        auto id = tokenText();
        // отыскиваем переменную в таблице символов
        auto symbol = currentSymbolTable()->find(id);
        if (!symbol) // или добавляем новую запись
            symbol = currentSymbolTable()->add(id);
        pushVariable(symbol);
        next();
    } else {
        // может быть составной идентификатор: obj.property.method
        match(Token::Identifier);
    }
}

void Parser::Expression()
{
    if (lookahead() == Token::Identifier) {
        // проверяем наличие идентификатора
        auto tokenText0 = tokenText();
        auto token0 = lookahead();
        next();
        // если знак присваивания, то в ветку AssignStatement
        if (Lexer::isAssignOp(lookahead())) {
            // перейти в ветку присваивания
            pushBack(token0, std::move(tokenText0));
            AssignStatement();
            return;
        }
        // вернуть идентификатор (прямой порядок, это очередь)
        pushBack(token0, std::move(tokenText0));
        // перейти в SimpleExpression
    }
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
            emitBinaryOp(OperationType::Add);
            continue;
        case Token::Minus:
            next();
            Term();
            emitBinaryOp(OperationType::Minus);
            continue;
        default:
            break;
        }
        break;
    } while (true);
}

void Parser::Term()
{
    // временная переменная, нужна для генерации инструкции и помещения её в стек
    Factor();
    // в стеке находится результат вызова Factor(), например, число.
    do {
        switch (lookahead()) {
        case Token::Asterisk: // умножение
            next();
            Factor();
            emitBinaryOp(OperationType::Multiply);
            continue;
        case Token::Slash: // деление
            next();
            Factor();
            emitBinaryOp(OperationType::Div);
            continue;
        default:
            break;
        }
        break;
    } while (true);
}

void Parser::Factor()
{
    std::shared_ptr<Symbol> symbol;
    switch (lookahead()) {
    case Token::LeftParenth:
        // круглые скобки
        next();
        SimpleExpression();
        match(Token::RightParenth);
        return;
    case Token::Plus:
        // унарный плюс, ничего не делаем
        next();
        Factor();
        return;
    case Token::Minus:
        next();
        // унарный минус
        Factor();
        emitUnaryOp(OperationType::UMinus);
        return;
    case Token::Identifier:
        // это правая часть, здесь - только ранее объявленный id
        symbol = currentSymbolTable()->find(tokenText());
        if (!symbol)
            undeclaredIdentifier();
        pushVariable(symbol);
        next();
        break;
    case Token::IntegerNumber:
        pushInt(_lexer->lastIntegerNumber());
        next();
        break;
    case Token::RealNumber:
        pushReal(_lexer->lastRealNumber());
        next();
        break;
    default: // ошибка, нужен терминал в виде числа, идентификатора и т.п.
        expected(Token::Identifier);
    }
}

//////////////////////// работа с символами  /////////////////////////////

void Parser::match(Token t)
{
    if (lookahead() != t)
        expected(t);
    next();
}

void Parser::next()
{
    if (!_tokensQueue.empty()) {
        _tokensQueue.pop_front();
        _tokenTextsQueue.pop_front();
        return;
    }
    _lexer->next();
}

Token Parser::lookahead()
{
    if (!_tokensQueue.empty()) {
        return _tokensQueue.front();
    }
    return _lexer->currentToken();
}

const u32string &Parser::tokenText() const
{
    if (!_tokenTextsQueue.empty()) {
        return _tokenTextsQueue.front();
    }
    return _lexer->tokenText();
}

void Parser::pushBack(Token t, const std::u32string &str)
{
    _tokensQueue.push_back(t);
    _tokenTextsQueue.push_back(str);
}

void Parser::pushBack(Token t, std::u32string &&str)
{
    _tokensQueue.push_back(t);
    _tokenTextsQueue.push_back(std::move(str));
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

void Parser::pushReal(RealType value)
{
    _reals.push(value);
    _types.push(SymbolType::Real);
}

void Parser::pushVariable(std::shared_ptr<Symbol> &variable)
{
    _variables.push(variable);
    _types.push(SymbolType::Variable);
}

IntType Parser::popInt()
{
    IntType lastInt = _integers.top();
    _integers.pop();
    _types.pop();
    return lastInt;
}

IntType Parser::popReal()
{
    RealType lastInt = _reals.top();
    _reals.pop();
    _types.pop();
    return lastInt;
}

void Parser::emitBinaryOp(OperationType opType)
{
    switch (opType) {
    case OperationType::Add:
    case OperationType::Div:
    case OperationType::Minus:
    case OperationType::Multiply: {
        auto opRecord2 = popStackValue();
        auto opRecord1 = popStackValue();
        std::shared_ptr<Symbol> tmp = currentSymbolTable()->addTemp();
        _emitter->binaryOp(opType, tmp.get(),
                           opRecord1.first, opRecord1.second,
                           opRecord2.first, opRecord2.second);
        pushVariable(tmp);
        break;
    }
    default:
        throw std::domain_error("Invalid binary operation");
    }
}

void Parser::emitUnaryOp(OperationType opType)
{
    std::shared_ptr<Symbol> symbol;
    // смотря что положили
    IntType lastInt;
    RealType lastReal;
    switch (_types.top()) {
    case SymbolType::Integer:
        lastInt = popInt();
        // не нужно тут никакого кода, сразу выполняем действие
        switch (opType) {
        case OperationType::UMinus:
            lastInt = -lastInt;
            break;
        default:
            throw std::domain_error("Invalid unary operation");
        }
        pushInt(lastInt);
        break;
    case SymbolType::Real:
        lastReal = popReal();
        // не нужно тут никакого кода, сразу выполняем действие
        switch (opType) {
        case OperationType::UMinus:
            lastReal = -lastReal;
            break;
        default:
            throw std::domain_error("Invalid unary operation");
        }
        pushReal(lastReal);
        break;
    case SymbolType::Variable: {
        symbol = _variables.top();
        auto opRecord1 = popStackValue();
        _emitter->unaryOp(opType, opRecord1.second.variable,
                           opRecord1.first, opRecord1.second);
        pushVariable(symbol); // ту же самую
        break;
    }
    default:
        error("Unsupported SymbolType/Factor");
    }

}

void Parser::emitAssign(std::shared_ptr<Symbol> &lvalue)
{
    // после Expression всегда что-то есть в стеке
    auto rec = popStackValue();
    _emitter->assign(lvalue.get(), rec.first, rec.second);
}

std::pair<SymbolType, OperandRecord> Parser::popStackValue()
{
    std::pair<SymbolType, OperandRecord> rec;
    auto valueType = _types.top();
    rec.first = valueType;
    _types.pop();
    switch (valueType) {
    case SymbolType::Variable:
        rec.second.variable = _variables.top().get();
        _variables.pop();
        break;
    case SymbolType::Integer:
        rec.second.intValue = _integers.top();
        _integers.pop();
        break;
    case SymbolType::Real:
        rec.second.realValue = _reals.top();
        _reals.pop();
        break;
    default:
        throw std::domain_error("Unsupported SymbolType");
    }
    return rec;
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

void Parser::undeclaredIdentifier()
{
    error("Undeclared identifier: " + toUtf8(tokenText()));
}

string Parser::toUtf8(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(s);
}

} // namespace escript
