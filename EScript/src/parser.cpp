/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
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
#include "parser.h"
#include "unit.h"
#include "stringcontainer.h"
#include "automationobject.h"
#include "mathobject.h"
#include "engineerrors.h"

using namespace std;

namespace escript {

Parser::Parser(std::shared_ptr<Block> &block,
               StringContainer &strContainer,
               const std::map<u32string, ConstructorFunction> &classes,
               std::vector<TCode> &outBuffer)
    : _rootBlock(block),
      _currentBlock(block),
      _strings(strContainer),
      _classes(classes)
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
        case Token::RightBrace:
            return; // блок закрыт
        case Token::Case: // в блоке switch, выходим
        case Token::Default:
            return;
        default:
            Statement();
            break;
        }
    } while (true);
}

void Parser::Statement()
{
    switch (lookahead()) {
    case Token::LeftBrace:
        CompoundStatement();
        break;
    case Token::If:
        IfElseStatement();
        break;
    case Token::While:
        WhileStatement();
        break;
    case Token::For:
        ForStatement();
        break;
    case Token::Do:
        DoWhileStattement();
        break;
    case Token::Break:
        BreakStatement();
        break;
    case Token::Continue:
        ContinueStatement();
        break;
    case Token::Function:
        FunctionDeclStatement();
        break;
    case Token::Return:
        ReturnStatement();
        break;
    case Token::Var:
        VariableDeclBlock();
        match(Token::Semicolon);
        break;
    case Token::Switch:
        SwitchStatement();
        break;
    default:
        AnyStatement();
        break;
    }
}

void Parser::CompoundStatement()
{
    match(Token::LeftBrace);
    if (lookahead() == Token::RightBrace) {
        match(Token::RightBrace);
        return; // пустой блок, не создаём подблок
    }
    addAndEntrySubBlock();
    StatementList();
    exitToUpLevelBlock();
    match(Token::RightBrace);
}

void Parser::IfElseStatement()
{
    match(Token::If);
    match(Token::LeftParenth);
    Expression();                  // теперь в стеке что-то есть
    match(Token::RightParenth);
    int falseLabel = nextLabel();
    emitIfFalseHeader(falseLabel); // if_false tmp_var метка_false
    OptionalStatement();           // ветка true
    if (lookahead() == Token::Else) {
        next();
        int exitLabel = nextLabel();
        emitGoto(exitLabel);
        emitLabel(falseLabel);
        OptionalStatement();       // ветка false
        emitLabel(exitLabel);
    } else {
        emitLabel(falseLabel);
    }
}

void Parser::WhileStatement()
{
    match(Token::While);          // while
    match(Token::LeftParenth);    // (
    int startLabel = nextLabel(); // метка возврата в начало цикла
    int exitLabel = nextLabel();  // метка выхода
    pushJumpLabels(startLabel, exitLabel); // сохранить метки для break/continue
    emitLabel(startLabel);
    Expression();                 // теперь в стеке что-то есть
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    match(Token::RightParenth);   // )
    OptionalStatement();          // тело цикла
    emitGoto(startLabel);         // возврат к условию
    emitLabel(exitLabel);         // выход
    popJumpLabels();              // убрать вышеуказанные метки
}

void Parser::DoWhileStattement()
{
    match(Token::Do);             // do
    int startLabel = nextLabel(); // метка возврата в начало цикла
    int exitLabel = nextLabel();  // метка выхода
    pushJumpLabels(startLabel, exitLabel); // сохранить метки для break/continue
    emitLabel(startLabel);
    CompoundStatement();          // { тело цикла }
    match(Token::While);          // while
    match(Token::LeftParenth);    // (
    Expression();                 // в стеке - условие продолжения
    match(Token::RightParenth);   // )
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    emitGoto(startLabel);         // возврат к условию
    emitLabel(exitLabel);         // выход
    popJumpLabels();              // убрать вышеуказанные метки
}

void Parser::ForStatement()
{
    match(Token::For);            // for (expr1; expr2; expr3) stmt;
    match(Token::LeftParenth);
    OptionalExpressionList();     // expr1
    match(Token::Semicolon);
    int startLabel = nextLabel(); // метка возврата в начало цикла
    int exitLabel = nextLabel();  // метка выхода
    // в этом цикле expr3 находится в конце, поэтому переход
    // по continue должен идти не назад, а вперёд до expr3
    int nextIterationLabel = nextLabel(); // метка expr3
    pushJumpLabels(nextIterationLabel, exitLabel); // для break/continue
    emitLabel(startLabel);
    if (lookahead() != Token::Semicolon) // условия может и не быть
        Expression();             // expr2, логическое условие
    else { // иначе это эквивалентно while (true), поэтому
        pushBoolean(true);        // помещаем в стек true
    }
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    match(Token::Semicolon);
    _emitter->switchToTempBuffer();
    OptionalExpressionList();     // expr3, её вывести в конец
    _emitter->switchToMainBuffer();
    match(Token::RightParenth);
    OptionalStatement();
    emitLabel(nextIterationLabel); // expr3
    _emitter->writeTempBuffer();   // вывести expr3
    emitGoto(startLabel);          // возврат к условию
    emitLabel(exitLabel);          // выход
    popJumpLabels();               // убрать верхние метки
}

void Parser::OptionalStatement()
{
    switch (lookahead()) {
    case Token::Semicolon:
        next();
        return;
    default:
        break;
    }
    Statement();
}

void Parser::BreakStatement()
{
    match(Token::Break);
    match(Token::Semicolon);
    if (_exitLabels.empty())
        error("[break] must be inside of do/while/for/switch statement");
    emitBreak();
}

void Parser::ContinueStatement()
{
    match(Token::Continue);
    match(Token::Semicolon);
    if (_startLabels.empty())
        error("[continue] must be inside of do/while/for statement");
    emitContinue();
}

void Parser::FunctionDeclStatement()
{
    FunctionDeclExpression();
    popStackValue();
}

void Parser::OptionalParameterDeclList()
{
    if (lookahead() == Token::RightParenth)
        return;
    ParameterDeclList();
}

void Parser::ParameterDeclList()
{
    do {
        switch (lookahead()) {
        case Token::Comma:
            next();
            continue;
        case Token::RightParenth:
            return;
        case Token::Identifier: // имя параметра
            emitFnArg();
            next();
            break;
        default:
            expected(Token::Identifier);
        }
    } while (lookahead() != Token::Eof);
}

void Parser::OptionalExpressionList()
{
    switch (lookahead()) {
    case Token::Semicolon:
    case Token::RightParenth:
        return;
    default:
        break;
    }
    ExpressionList();
}

void Parser::ExpressionList()
{
    do {
        Expression();
        popStackValue(); // убрать всё, т.к. присвоить результат невозможно
                         // или всё же оставить результат последнего выражения?
        switch (lookahead()) {
        case Token::Comma:
            next();
            continue;
        // все разделители:
        case Token::Semicolon:
        case Token::RightParenth:
            return;
        default:
            break;
        }
    } while (lookahead() != Token::Eof);
}

void Parser::AssignExpression()
{
    // здесь пока только переменные
    assert(lookahead() == Token::Identifier);
    auto id = tokenText();
    auto symbol = currentSymbolTable()->find(id); // для += должен существовать
    Variable();
    auto lvalueSymbol = _values.top().variable;
    popStackValue(); // результат в lvalueSymbol, просто вынем из стека

    OperationType op; // выбрать оператор присваивания
    switch (lookahead()) {
    case Token::Assign: {
        match(Token::Assign);
        Expression();
        emitAssign(lvalueSymbol);
        // поскольку присваивание само является выражением,
        // нужно поместить в стек результат вычислений
        pushVariable(lvalueSymbol);
        return;
    }
    case Token::PlusAssign:
        op = OperationType::Add;
        break;
    case Token::MinusAssign:
        op = OperationType::Minus;
        break;
    case Token::MulAssign:
        op = OperationType::Multiply;
        break;
    case Token::SlashAssign:
        op = OperationType::Div;
        break;
    case Token::BitAndAssign:
        op = OperationType::BitAND;
        break;
    case Token::BitOrAssign:
        op = OperationType::BitOR;
        break;
    case Token::XorAssign:
        op = OperationType::BitXOR;
        break;
    case Token::LShiftAssign:
        op = OperationType::LShift;
        break;
    case Token::RShiftAssign:
        op = OperationType::RShift;
        break;
    case Token::RShiftZeroAssign:
        op = OperationType::RShiftZero;
        break;
    case Token::PercentAssign:
        op = OperationType::Mod;
        break;
    default:
        expected(Token::Assign);
        return;
    }
    next();
    // здесь оператор присваивания с вычислением
    // левая часть должна существовать
    if (!symbol)
        undeclaredIdentifier();
    // поместить в стек левую часть и выполнить Expression,
    pushVariable(symbol);
    Expression(); // правая часть
    emitBinaryOp(op);
    emitAssign(lvalueSymbol);
    pushVariable(lvalueSymbol);
}

void Parser::FunctionDeclExpression()
{
    // функция может располагаться в любом месте объектного файла,
    // поэтому нужна метка для прыжка через функцию, если
    // идёт последовательное выполнение кода
    int labelEnd = nextLabel();
    emitGoto(labelEnd);
    match(Token::Function);
    // временная переменная для функции
    std::shared_ptr<Symbol> func;
    if (lookahead() == Token::Identifier) {
        func = currentSymbolTable()->add(tokenText());
        match(Token::Identifier);
    } else if (lookahead() == Token::LeftParenth)
        func = currentSymbolTable()->addTemp();
    else
        expected(Token::Identifier);

    _returnStack.push(func);
    addAndEntrySubBlock();
    emitFnStart(func);
    match(Token::LeftParenth);
    OptionalParameterDeclList();
    match(Token::RightParenth);
    emitFnCode(func);
    emitLoadFnArgs();
    CompoundStatement();
    // явный выход, если не было return;
    emitEmptyReturn();
    emitFnEnd();
    exitToUpLevelBlock();
    emitLabel(labelEnd);
    _returnStack.pop();
}

void Parser::Variable()
{ // это правило исключительно для l-value
    if (lookahead() == Token::Identifier) {
        auto id = tokenText();
        // отыскиваем переменную в таблице символов
        auto symbol = currentSymbolTable()->find(id);
        if (!symbol) // или добавляем новую запись
            symbol = currentSymbolTable()->add(id);
        pushVariable(symbol.get());
    }
    match(Token::Identifier);
}

void Parser::Expression()
{
    switch (lookahead()) {
    case Token::Identifier: {
        // проверяем наличие идентификатора
        auto tokenText0 = tokenText();
        auto token0 = lookahead();
        next();
        // если знак присваивания, то в ветку AssignStatement
        if (Lexer::isAssignOp(lookahead())) {
            // одно из присваиваний или обращение к элементу массива
            // перейти в ветку присваивания
            pushBack(token0, std::move(tokenText0));
            AssignExpression();
            return;
        }
        // вернуть идентификатор (прямой порядок, это очередь)
        pushBack(token0, std::move(tokenText0));
        break;
    }
    case Token::Function:
        FunctionDeclExpression();
        return;
    case Token::LeftBracket:
        ArrayDeclExpression();
        return;
    case Token::New:
        NewExpression();
        return;
    default:
        break;
    }
    LogicalOrNCOExpression();
    TernaryExpression();
}

void Parser::TernaryExpression()
{
    if (lookahead() != Token::Question)
        return;
    // тернарный условный оператор
    assert(!_values.empty());       // в стеке - первое выражение, которое
    match(Token::Question);         // рассматривается как логическое условие
    int falseLabel = nextLabel();   // сделаем как if-else, результат каждой
    int exitLabel = nextLabel();    // ветки поместим во временную переменную
                                    // на выходе из правила эта переменная будет
                                    // использоваться как результат выражения
    auto tmp = currentSymbolTable()->addTemp();
    emitIfFalseHeader(falseLabel);
    popStackValue();                // это - логическое значение, убираем его
    Expression();                   // true-ветка
    emitAssign(tmp);
    match(Token::Colon);
    emitGoto(exitLabel);
    emitLabel(falseLabel);
    Expression();                   // false-ветка
    emitAssign(tmp);
    emitLabel(exitLabel);
    pushVariable(tmp);              // результат - либо из true, либо из false-веток
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

void Parser::SimpleOrShiftExpression()
{
    SimpleExpression();
    OperationType op;
    switch (lookahead()) {
    case Token::LShift:
        op = OperationType::LShift;
        break;
    case Token::RShift:
        op = OperationType::RShift;
        break;
    case Token::RShiftZero:
        op = OperationType::RShiftZero;
        break;
    default:
        return;
    }
    next();
    SimpleExpression();
    emitBinaryOp(op);
}

void Parser::ShiftOrRelationExpression()
{
    SimpleOrShiftExpression();
}

void Parser::RelationOrEqualityExpression()
{
    ShiftOrRelationExpression();
    OperationType opType;
    do {
        switch (lookahead()) {
        case Token::Less:
            opType = OperationType::Less;
            break;
        case Token::LessEqual:
            opType = OperationType::LessOrEqual;
            break;
        case Token::Greater:
            opType = OperationType::Greater;
            break;
        case Token::GreaterEqual:
            opType = OperationType::GreaterOrEqual;
            break;
        case Token::Equal:
            opType = OperationType::Equal;
            break;
        case Token::NotEqual:
            opType = OperationType::NotEqual;
            break;
        default:
            return;
        }
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(opType);
    } while (lookahead() != Token::Eof);
}

void Parser::BitwiseAndOrEqualityExpression()
{
    RelationOrEqualityExpression();
    do {
        if (lookahead() == Token::Ampersand) {
            // побитовое AND
            next();
            RelationOrEqualityExpression();
            emitBinaryOp(OperationType::BitAND);
            continue;
        }
        break;
    } while (lookahead() != Token::Eof);
}

void Parser::BitwiseXOROrAndExpression()
{
    BitwiseAndOrEqualityExpression();
    do {
        if (lookahead() == Token::Caret) {
            // побитовое XOR
            next();
            BitwiseAndOrEqualityExpression();
            emitBinaryOp(OperationType::BitXOR);
            continue;
        }
        break;
    } while (lookahead() != Token::Eof);
}

void Parser::BitwiseOROrXORExpression()
{
    BitwiseXOROrAndExpression();
    do {
        if (lookahead() == Token::Vertical) {
            // побитовое OR
            next();
            BitwiseXOROrAndExpression();
            emitBinaryOp(OperationType::BitOR);
            continue;
        }
        break;
    } while (lookahead() != Token::Eof);
}

void Parser::LogicalAndOrBitORExpression()
{
    BitwiseOROrXORExpression();
    do {
        if (lookahead() == Token::LogicalAnd) {
            // логическое AND
            next();
            BitwiseOROrXORExpression();
            emitBinaryOp(OperationType::LogAND);
            continue;
        }
        break;
    } while (lookahead() != Token::Eof);
}

void Parser::LogicalOrOrAndExpression()
{
    LogicalAndOrBitORExpression();
    do {
        if (lookahead() == Token::LogicalOr) {
            // логическое OR
            next();
            LogicalAndOrBitORExpression();
            emitBinaryOp(OperationType::LogOR);
            continue;
        }
        break;
    } while (lookahead() != Token::Eof);
}

void Parser::LogicalOrNCOExpression()
{
    LogicalOrOrAndExpression();
    if (lookahead() == Token::NCO) {
        next();
        LogicalOrOrAndExpression();
        emitBinaryOp(OperationType::NCO);
    }
}

void Parser::Term()
{
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
        case Token::Percent: // деление по модулю
            next();
            Factor();
            emitBinaryOp(OperationType::Mod);
            continue;
        default:
            break;
        }
        break;
    } while (true);
}

void Parser::Factor()
{ // Здесь объединены правила Literals, Grouping, CallOrAccess, PostfixOperation
    auto t = lookahead(); // понадобится ниже
    switch (t) {
    case Token::LeftParenth:
        // круглые скобки
        next();
        Expression();
        match(Token::RightParenth);
        OptionalDotOrBracketExpression();
        break;
    case Token::Plus:
        // унарный плюс, ничего не делаем
        next();
        Factor();
        break;
    case Token::Minus:
        // унарный минус
        next();
        Factor();
        emitUnaryOp(OperationType::UMinus);
        break;
    case Token::Exclamation: // логическое НЕ
        next();
        Factor();
        emitUnaryOp(OperationType::LogNOT);
        break;
    case Token::BitNot: // побитовое НЕ
        next();
        Factor();
        emitUnaryOp(OperationType::BitNOT);
        break;
    case Token::PlusPlus:
        next();
        Factor();
        emitIncrement();
        break;
    case Token::MinusMinus:
        next();
        Factor();
        emitDecrement();
        break;
    case Token::Identifier: {
        // это правая часть, здесь - только ранее объявленный id
        auto idText = tokenText();
        next();
        switch (lookahead()) {
        case Token::LeftParenth:
            // это вызов функции
            pushBack(Token::Identifier, idText);
            FunctionCallExpression();
            OptionalDotOrBracketExpression();
            break;
        case Token::LeftBracket: {
            // обращение к элементу массива
            auto id = tokenText();
            auto arrValue = currentSymbolTable()->find(idText);
            if (!arrValue)
                undeclaredIdentifier(idText);
            pushVariable(arrValue);
            OptionalDotOrBracketExpression();
            break;
        }
        case Token::Dot: {
            // DotOperation: здесь важно различить: это слева от = или справа
            if (idText == U"Math") {
                if (resolveMathConstant())
                    return;
            }
            auto opValue = currentSymbolTable()->find(idText);
            if (!opValue)
                undeclaredIdentifier(idText);
            pushVariable(opValue);
            OptionalDotOrBracketExpression();
            break;
        }
        default:
            auto symbol = currentSymbolTable()->find(idText);
            if (!symbol)
                undeclaredIdentifier(idText);
            pushVariable(symbol);
            break; // просто идентификатор
        }
        break;
    }
    case Token::IntegerNumber:
        pushInt(_lexer->lastIntegerNumber());
        next();
        break;
    case Token::NaN:
        pushReal(std::numeric_limits<double>::signaling_NaN());
        next();
        break;
    case Token::RealNumber:
        pushReal(_lexer->lastRealNumber());
        next();
        break;
    case Token::True:
        pushBoolean(true);
        next();
        break;
    case Token::False:
        pushBoolean(false);
        next();
        break;
    case Token::Null:
        next();
        pushNull();
        break;
    case Token::QuotedString:
    case Token::BackQuotedString: {
        auto str = tokenText();
        next();
        if (lookahead() == Token::Dot) {
            pushBack(t, str);
            DotOperation();
            return;
        } else if (lookahead() == Token::LeftBracket) {
            auto tmpStr = currentSymbolTable()->addTemp();
            pushString(str);
            emitAssign(tmpStr);
            pushVariable(tmpStr);
            OptionalDotOrBracketExpression();
            return;
        } else {
            pushString(str);
        }
        break;
    }
    default: // ошибка, нужен терминал в виде числа, идентификатора и т.п.
        expected(Token::Identifier);
    }
    PostfixOperation();
}

void Parser::FunctionCallExpression()
{
    auto func = currentSymbolTable()->find(tokenText());
    if (!func)
        undeclaredIdentifier();
    match(Token::Identifier);
    pushFunction(func); // нужен для FunctionCallArgs()
    FunctionCallArgs();
}

void Parser::OptionalFunctorCall()
{
    if (lookahead() != Token::LeftParenth)
        return;
    if (_values.empty())
        error("Expected expression before () operation");
    switch (_values.top().type) {
    case SymbolType::Variable:
        break; // здесь обязана быть некая временная переменная
    default:
        error("Expected variable before () operation");
    }
    FunctionCallArgs();
}

void Parser::FunctionCallArgs()
{
    // функция должна быть помещена в стек парсера
    auto func = popStackValue().variable;
    match(Token::LeftParenth);
    int nArgs = 0;
    if (lookahead() != Token::RightParenth) {
        _argumentsCountStack.push(0);
        ArgumentList();
        nArgs = _argumentsCountStack.top();
        _argumentsCountStack.pop();
    }
    match(Token::RightParenth);
    auto resultVariable = currentSymbolTable()->addTemp();
    emitCall(func, nArgs, resultVariable);
    pushVariable(resultVariable);
}

void Parser::ArgumentList()
{
    do {
        // выражение вставляет в стек очередной аргумент
        Expression();
        _argumentsCountStack.top()++;
        emitPush();
        popStackValue(); // не нужно больше
        switch (lookahead()) {
        case Token::Comma:
            next();
            continue;
        case Token::RightParenth:
            return;
        default:
            expected(Token::RightParenth);
        }
    } while (true);
}

void Parser::AnyStatement()
{
    Expression();
    match(Token::Semicolon);
    // после Expression всегда есть что-то, т.к. это выражение,
    // поэтому вытаскиваем из стека его результат
    popStackValue();
}

void Parser::ReturnStatement()
{
    if (_returnStack.empty())
        error("The [return] keyword allowed only inside of function");
    match(Token::Return);
    if (lookahead() == Token::Semicolon) {
        emitEmptyReturn();
        return;
    }
    Expression();
    emitReturn();
    match(Token::Semicolon);
}

void Parser::ArrayDeclExpression()
{
    match(Token::LeftBracket);
    auto arrValue = currentSymbolTable()->addTemp();
    pushArray(arrValue);
    emitAllocArray(arrValue);
    if (lookahead() == Token::RightBracket) {
        // пустой массив
        pushVariable(arrValue);
        next();
        return;
    }
    // ArrayDeclItems располагаем здесь, не в отдельном правиле
    do {
        if (lookahead() == Token::RightBracket)
            break;
        Expression();
        auto e = currentSymbolTable()->addTemp();
        emitAssign(e);
        pushVariable(e);
        emitPush();
        // в результате вызова всегда что-то есть, даже 0
        auto resultVariable = currentSymbolTable()->addTemp();
        emitCallAOMethod(arrValue.get(), U"add", resultVariable, 1);
        pushVariable(resultVariable);
        if (lookahead() == Token::RightBracket)
            break;
        match(Token::Comma);
    } while (lookahead() != Token::Eof);
    match(Token::RightBracket);
    pushVariable(arrValue);
    OptionalDotOrBracketExpression();
}

void Parser::ArrayItemRefExpression()
{
    auto id = tokenText();
    auto arrValue = currentSymbolTable()->find(id);
    match(Token::Identifier);
    match(Token::LeftBracket);
    Expression(); // индекс, он же ключ элемента
    emitPush();
    popStackValue(); // убрать индекс элемента
    match(Token::RightBracket);
    auto resultVariable = currentSymbolTable()->addTemp();
    std::u32string methodName;
    int nArgs = 0;
    if (lookahead() == Token::Assign) {
        // присваивание элементу массива
        next();
        Expression(); // новое значение
        emitPush();
        popStackValue(); // убрать индекс элемента
        methodName = U"set";
        nArgs = 2;
    } else {
        // получение значения элемента массива
        methodName = U"get";
        nArgs = 1;
    }
    emitCallAOMethod(arrValue.get(), methodName, resultVariable, nArgs);
    pushVariable(resultVariable);
}

void Parser::OptionalArrayItemRefExpression()
{
    // это необязательное выражение, поэтому выходим
    if (lookahead() != Token::LeftBracket)
        return;
    if (_values.empty())
        error("Expected expression before [] operation");
    switch (_values.top().type) {
    case SymbolType::Variable:
        break; // здесь должна быть некая временная переменная
    default:
        error("Expected variable before [] operation");
    }
    do {
        auto arrValue = _values.top().variable;
        _values.pop();
        match(Token::LeftBracket);
        Expression(); // индекс, он же ключ элемента
        emitPush();
        popStackValue(); // убрать индекс элемента
        match(Token::RightBracket);
        auto resultVariable = currentSymbolTable()->addTemp();
        std::u32string methodName;
        int nArgs = 0;
        if (lookahead() == Token::Assign) {
            // присваивание элементу массива
            next();
            Expression(); // новое значение
            emitPush();
            popStackValue(); // убрать индекс элемента
            methodName = U"set";
            nArgs = 2;
        } else {
            // получение значения элемента массива
            methodName = U"get";
            nArgs = 1;
        }
        emitCallAOMethod(arrValue, methodName, resultVariable, nArgs);
        pushVariable(resultVariable);
        // если можно несколько раз применить [], то делаем, пока не закончатся
        // такое возможно, если это массив массивов или массив строк
        if (lookahead() != Token::LeftBracket)
            break;
    } while (lookahead() != Token::Eof);
}

void Parser::DotOperation()
{
    // пока только одноуровневые: id.свойство, а не id.prop1.prop2.prop3
    auto variableName = tokenText();
    if (lookahead() == Token::QuotedString ||
            lookahead() == Token::BackQuotedString) {
        // вызываем метод у строкового литерала, поэтому нужно сначала
        // создать временную переменную
        auto tmpStrValue = currentSymbolTable()->addTemp();
        pushString(tokenText());
        emitAssign(tmpStrValue);
        variableName = tmpStrValue->name();
        next();
    } else {
        match(Token::Identifier);
    }
    auto symbol = currentSymbolTable()->find(variableName);
    if (!symbol)
        undeclaredIdentifier();
    match(Token::Dot);
    auto methodName = tokenText();
    match(Token::Identifier);
    if (variableName == U"Math") {
        double v;
        if (Math::getConstant(methodName, v)) {
            // можно вставить сразу константу
            pushReal(v);
            return;
        }
    }
    auto resultVariable = currentSymbolTable()->addTemp();
    int nArgs = 0;
    if (lookahead() == Token::LeftParenth) {
        // это метод
        match(Token::LeftParenth);
        _argumentsCountStack.push(0);
        if (lookahead() != Token::RightParenth)
            ArgumentList();
        nArgs = _argumentsCountStack.top();
        _argumentsCountStack.pop();
        match(Token::RightParenth);
    } else {
        // это свойство; какое: get/set?
        if (lookahead() == Token::Assign) {
            // setter
            methodName = U"set_" + methodName;
            next();
            Expression(); // вычислить выражение для параметра метода
            emitPush();   // результат - 1 аргумент поместить в стек
            nArgs = 1;
            popStackValue(); // убрать выражение разбора
        } else {
            // getter
            methodName = U"get_" + methodName;
        }
    }
    emitCallAOMethod(symbol.get(), methodName, resultVariable, nArgs);
    pushVariable(resultVariable);
}

void Parser::OptionalDotOperation()
{
    if (lookahead() != Token::Dot)
        return;
    if (_values.empty())
        error("Expected expression before [] operation");
    switch (_values.top().type) {
    case SymbolType::Variable:
        break; // здесь должна быть некая временная переменная
    case SymbolType::Integer:
    case SymbolType::Real: {
        // если это литерал, то создать временную переменную
        auto tmpLiteralVar = currentSymbolTable()->addTemp();
        emitAssign(tmpLiteralVar);
        pushVariable(tmpLiteralVar);
        break;
    }
    default:
        error("Expected variable before [] operation");
    }
    match(Token::Dot);
    auto methodName = tokenText();
    match(Token::Identifier);
    auto symbol = _values.top().variable;
    _values.pop();
    auto resultVariable = currentSymbolTable()->addTemp();
    int nArgs = 0;
    if (lookahead() == Token::LeftParenth) {
        // это метод
        match(Token::LeftParenth);
        _argumentsCountStack.push(0);
        if (lookahead() != Token::RightParenth)
            ArgumentList();
        nArgs = _argumentsCountStack.top();
        _argumentsCountStack.pop();
        match(Token::RightParenth);
    } else {
        // это свойство; какое: get/set?
        if (lookahead() == Token::Assign) {
            // setter
            methodName = U"set_" + methodName;
            next();
            Expression(); // вычислить выражение для параметра метода
            emitPush();   // результат - 1 аргумент поместить в стек
            nArgs = 1;
            popStackValue(); // убрать выражение разбора
        } else {
            // getter
            methodName = U"get_" + methodName;
        }
    }
    emitCallAOMethod(symbol, methodName, resultVariable, nArgs);
    pushVariable(resultVariable);
}

void Parser::PostfixOperation()
{
    void (Parser::*pfn)(); // объявим указатель и выберем нужную функцию
    switch (lookahead()) {
    case Token::PlusPlus:
        pfn = &Parser::emitIncrement;
        break;
    case Token::MinusMinus:
        pfn = &Parser::emitDecrement;
        break;
    default:
        return;
    }
    next();
    // postfix
    // вынуть из стека, создать временную переменную == копию вынутого,
    // увеличить временную переменную, затолкать её в стек вместо первой
    auto top = stackValue();
    auto tmp = currentSymbolTable()->addTemp();
    emitAssign(tmp);
    switch (top.type) {
    case SymbolType::Integer:
        pushInt(top.intValue);
        break;
    case SymbolType::Real:
        pushInt(top.realValue);
        break;
    case SymbolType::Variable:
        pushVariable(top.variable);
        break;
    default:
        error("INC/DEC not supported here");
    }
    emitPush();
    (this->*pfn)();    // inc/dec
    emitPop();         // убрать результат inc/dec
    popStackValue();
    pushVariable(tmp);
}

void Parser::VariableDeclBlock()
{
    // три правила в одном: VariableDeclBlock, VariableDeclStatementList,
    // VariableDeclStatement
    match(Token::Var); // 1 var на весь блок объявлений
    do {
        if (lookahead() == Token::Identifier) {
            auto idText = tokenText();
            auto symbol = currentSymbolTable()->findCurrentScopeOnly(idText);
            if (symbol) // в текущем блоке не должно быть с тем же именем
                duplicateIdentifier(idText);
            symbol = currentSymbolTable()->add(idText);
            next();
            if (lookahead() == Token::Comma) {
                next();
                continue;
            }
            if (lookahead() == Token::Assign) {
                pushBack(Token::Identifier, idText);
                AssignExpression();
                popStackValue();
                continue;
            }
        }
        if (lookahead() == Token::Comma) {
            next(); // не страшно, пропустить несколько запятых
            continue;
        }
        if (lookahead() == Token::Semicolon)
            break;
        error("Syntax error");
    } while (lookahead() != Token::Eof);
}

void Parser::SwitchStatement()
{
    match(Token::Switch);
    int exitLabel = nextLabel();  // метка выхода
    _exitLabels.push(exitLabel);
    match(Token::LeftParenth);
    Expression();
    // следующую переменную будем сравнивать со всеми значениями в case exprN:
    auto switchValue = currentSymbolTable()->addTemp(); // switch (expr1)
    emitAssign(switchValue);
    match(Token::RightParenth);
    match(Token::LeftBrace);
    // для прохода (fall-through) из одного case в другой, если нет break
    int labelFallThroughToNextCase = nextLabel();
    int nextComparisonLabel = nextLabel();
    bool defaultEmitted = false; // если default уже был, то true
    int defaultLabel = nextLabel();
    emitGoto(nextComparisonLabel); // чтобы не попасть в default, если он сверху
    do {
        switch (lookahead()) {
        case Token::Case: {
            match(Token::Case);
            emitLabel(nextComparisonLabel);
            nextComparisonLabel = nextLabel();
            Expression(); // можно всё, что угодно, лишь бы что-то возвращало
            pushVariable(switchValue);
            emitBinaryOp(OperationType::Equal);
            emitIfFalseHeader(nextComparisonLabel);
            popStackValue(); // убрать результат сравнения - он больше не нужен
            match(Token::Colon);
            emitLabel(labelFallThroughToNextCase);
            labelFallThroughToNextCase = nextLabel();
            StatementList();
            // здесь ожидается break
            // в этом месте можно выдать предупреждение
            // однако проход дальше допускается, поэтому,
            // если выше был break, то на этот goto мы просто не попадём,
            // иначе совершаем прыжок в тело следующего case или default
            emitGoto(labelFallThroughToNextCase);
            break;
        }
        case Token::Default:
            if (defaultEmitted)
                error("Duplicate [default] block.");
            defaultEmitted = true;
            match(Token::Default);
            match(Token::Colon);
            emitLabel(defaultLabel);
            emitLabel(labelFallThroughToNextCase);
            labelFallThroughToNextCase = nextLabel();
            StatementList();
            emitGoto(labelFallThroughToNextCase);
            break;
        case Token::RightBrace:
            break;
        default:
            unexpected(lookahead());
        }
        if (lookahead() == Token::RightBrace)
            break; // } закрывающая скобка, завершение блока switch
    } while (lookahead() != Token::Eof);
    match(Token::RightBrace); // }
    // здесь должен быть прыжок в default
    if (defaultEmitted) // только если вообще был default
        emitGoto(defaultLabel);
    // это метка IFFALSE последнего сравнения,
    // она укажет на выход, как и exitLabel
    emitLabel(nextComparisonLabel);
    if (defaultEmitted) {
        // только один default
        emitGoto(defaultLabel);
    }
    emitLabel(labelFallThroughToNextCase);
    emitLabel(_exitLabels.top());
    _exitLabels.pop();
}

void Parser::OptionalDotOrBracketExpression()
{
    // наивысший приоритет
    // Операции 'точка' или 'обращение к элементу[]'
    // Это необязательное правило и применяется в особых местах,
    // перед этим в стек должна быть помещена переменная,
    // но только если сейчас токены "." или "["
    do {
        switch (lookahead()) {
        case Token::Dot:
            OptionalDotOperation();
            break;
        case Token::LeftBracket:
            OptionalArrayItemRefExpression();
            break;
        case Token::LeftParenth: // вызов чего угодно как функции
            OptionalFunctorCall();
            break;
        default:
            return;
        }
    } while (lookahead() != Token::Eof);
}

void Parser::NewExpression()
{
    match(Token::New);
    auto className = tokenText();
    match(Token::Identifier);
    auto classRecord = _classes.find(className);
    if (classRecord == _classes.end())
        undeclaredIdentifier(className);
    match(Token::LeftParenth);
    int nArgs = 0;
    if (lookahead() != Token::RightParenth) {
        _argumentsCountStack.push(0);
        ArgumentList();
        nArgs = _argumentsCountStack.top();
        _argumentsCountStack.pop();
    }
    match(Token::RightParenth);
    auto resultVariable = currentSymbolTable()->addTemp();
    emitNewObject(className, classRecord->second, resultVariable, nArgs);
    pushVariable(resultVariable);
}

bool Parser::resolveMathConstant()
{
    match(Token::Dot);
    auto methodName = tokenText();
    double v;
    if (Math::getConstant(methodName, v)) {
        // можно вставить сразу константу
        pushReal(v);
        next();
        return true;
    } else {
        pushBack(Token::Dot, U".");
    }
    return false;
}

//////////////////////// перемещение по потоку  /////////////////////////////

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

////////////////////////// emitter //////////////////////////////////////////

void Parser::emitIfFalseHeader(int exitOrFalseLabelId)
{
    auto valueType = _values.top().type;
    // TODO: зависит от того, что тут есть:
    // если значение можно вычислить сейчас, то ветку false/true
    // можно выбросить и обойтись без ветвления
    // пока мы засунем во временную переменную
    Symbol *ptrValue = nullptr;
    if (valueType != SymbolType::Variable) {
        std::shared_ptr<Symbol> tmp;
        tmp = currentSymbolTable()->addTemp();
        ptrValue = tmp.get();
        emitAssign(ptrValue);
    } else {
        ptrValue = _values.top().variable;
    }
    // здесь должна быть только переменная в стеке,
    // иначе должна быть оптимизация, и сюда мы не доходим
    _emitter->iffalse(ptrValue, exitOrFalseLabelId);
}

void Parser::emitGoto(int labelId)
{
    _emitter->goToLabel(labelId);
}

void Parser::emitLabel(int labelId)
{
    _emitter->label(labelId);
}

void Parser::emitBinaryOp(OperationType opType)
{
    auto opRecord2 = popStackValue();
    auto opRecord1 = popStackValue();
    std::shared_ptr<Symbol> tmp = currentSymbolTable()->addTemp();
    // правильная операция будет проверена внутри,
    // чтобы не дублировать проверки:
    _emitter->binaryOp(opType, tmp.get(), opRecord1, opRecord2);
    pushVariable(tmp);
}

void Parser::emitBreak()
{
    emitGoto(_exitLabels.top());
}

void Parser::emitContinue()
{
    emitGoto(_startLabels.top());
}

void Parser::emitUnaryOp(OperationType opType)
{
    // смотря что положили
    IntType lastInt;
    RealType lastReal;
    bool lastBool;
    switch (_values.top().type) {
    case SymbolType::Integer:
        lastInt = popInt();
        // не нужно тут никакого кода, сразу выполняем действие
        switch (opType) {
        case OperationType::UMinus:
            pushInt(-lastInt);
            break;
        case OperationType::BitNOT:
            pushInt(~lastInt);
            break;
        case OperationType::LogNOT:
            pushBoolean(lastInt ? false : true);
            break;
        default:
            throw std::domain_error("Invalid unary operation");
        }
        break;
    case SymbolType::Real:
        lastReal = popReal();
        // не нужно тут никакого кода, сразу выполняем действие
        switch (opType) {
        case OperationType::UMinus:
            lastReal = -lastReal;
            pushReal(lastReal);
            break;
        case OperationType::LogNOT: // инвертируем относительно 0
            pushBoolean(lastReal == 0 ? true : false);
            break;
        default:
            throw std::domain_error("Invalid unary operation");
        }
        break;
    case SymbolType::Boolean:
        lastBool = popBoolean();
        switch (opType) {
        case OperationType::LogNOT: // инвертируем относительно 0
            pushBoolean(!lastBool);
            break;
        default:
            throw std::domain_error("Invalid unary operation");
        }
        break;
    case SymbolType::Variable: {
        Symbol *symbol = _values.top().variable;
        auto opRecord1 = popStackValue();
        _emitter->unaryOp(opType, opRecord1.variable, opRecord1);
        pushVariable(symbol); // ту же самую
        break;
    }
    default:
        error("Unsupported SymbolType/Factor");
    }

}

void Parser::emitAssign(Symbol *lvalue)
{
    // после Expression всегда что-то есть в стеке
    auto rec = popStackValue();
    _emitter->assign(lvalue, rec);
}

void Parser::emitAssign(std::shared_ptr<Symbol> &lvalue)
{
    emitAssign(lvalue.get());
}

void Parser::emitFnStart(std::shared_ptr<Symbol> &func)
{
    _emitter->fnStart(func);
    pushFunction(func);
    // переменнная arguments
    std::shared_ptr<Symbol> arg = currentSymbolTable()->add(U"arguments");
    _emitter->fnArg(arg);
}

void Parser::emitFnArg()
{
    std::shared_ptr<Symbol> arg = currentSymbolTable()->add(tokenText());
    _emitter->fnArg(arg);
}

void Parser::emitFnCode(std::shared_ptr<Symbol> &func)
{
    _emitter->fnCode(func);
}

void Parser::emitLoadFnArgs()
{
    _emitter->fnLoadArgs();
}

void Parser::emitPush()
{
    auto top = stackValue();
    _emitter->push(top);
}

void Parser::emitEmptyReturn()
{
    // поскольку функция может вернуть что угодно, возвращаемое значение
    // всегда должно быть, по умолчанию пусть будет 0.
    pushInt(0);
    emitReturn();
}

void Parser::emitReturn()
{
    // в стеке должен быть результат
    auto top = popStackValue();
    _emitter->push(top);
    _emitter->fnReturn(_returnStack.top());
}

void Parser::emitCall(std::shared_ptr<Symbol> &func, int nArgs,
                      std::shared_ptr<Symbol> &resultVariable)
{
    emitCall(func.get(), nArgs, resultVariable);
}

void Parser::emitCall(Symbol *func, int nArgs, std::shared_ptr<Symbol> &resultVariable)
{
    // вызов любого объекта типа Variable/Function как функции
    _emitter->call(func, nArgs, resultVariable);
}

void Parser::emitFnEnd()
{
    _emitter->fnEnd();
}

void Parser::emitCallAOMethod(Symbol *leftVariable,
                              const std::u32string &propName,
                              std::shared_ptr<Symbol> &resultVariable,
                              int nArgs)
{
    // в стеке должны находиться аргументы, например,
    // в результате применения правила ArgumentList
    _emitter->callAOMethod(leftVariable, _strings.add(propName),
                           resultVariable, nArgs);
}

void Parser::emitAllocArray(std::shared_ptr<Symbol> &arrVariable)
{
    _emitter->allocArray(arrVariable);
}

void Parser::emitIncrement()
{
    auto top = popStackValue();
    // попробовать вычислить, если это число
    if (top.type == SymbolType::Integer) {
        top.intValue++;
        pushInt(top.intValue);
    } else if (top.type == SymbolType::Real) {
        top.realValue++;
        pushReal(top.realValue);
    } else if (top.type == SymbolType::Variable) {
        _emitter->push(top);
        _emitter->increment();
        pushVariable(top.variable);
    }
}

void Parser::emitDecrement()
{
    auto top = popStackValue();
    // попробовать вычислить, если это число
    if (top.type == SymbolType::Integer) {
        top.intValue--;
        pushInt(top.intValue);
    } else if (top.type == SymbolType::Real) {
        top.realValue--;
        pushReal(top.realValue);
    } else if (top.type == SymbolType::Variable) {
        _emitter->push(top);
        _emitter->decrement();
        pushVariable(top.variable);
    }
}

void Parser::emitPop()
{
    _emitter->pop();
}

void Parser::emitNewObject(
        const std::u32string &className,
        ConstructorFunction pFn, std::shared_ptr<Symbol> &resultVariable, int nArgs)
{
    // решаем здесь, нельзя ли привести к известным типам, например, Array, String и т.п.
    // по умолчанию всегда Object
    SymbolType resultType = SymbolType::Object;
    if (className == U"Array")
        resultType = SymbolType::Array;
    _emitter->ctor(pFn, resultVariable, resultType, nArgs);
}


////////////////////// работа с символами ///////////////////////////////////

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
    return _currentBlock->symbolTable();
}

void Parser::pushInt(IntType value)
{
    _values.emplace(value);
}

void Parser::pushReal(RealType value)
{
    _values.emplace(value);
}

void Parser::pushBoolean(bool value)
{
    _values.emplace(value);
}

void Parser::pushVariable(std::shared_ptr<Symbol> &variable)
{
    pushVariable(variable.get());
}

void Parser::pushVariable(Symbol *variable)
{
    _values.emplace(variable, SymbolType::Variable);
}

void Parser::pushFunction(std::shared_ptr<Symbol> &func)
{
    _values.emplace(func.get(), SymbolType::Function);
}

void Parser::pushString(const std::u32string &s)
{
    Operand rec;
    rec.type = SymbolType::String;
    rec.strValue = _strings.add(s);
    _values.push(rec);
}

void Parser::pushArray(std::shared_ptr<Symbol> &arrVariable)
{
    Operand rec;
    rec.type = SymbolType::Array;
    rec.variable = arrVariable.get();
    _values.push(rec);
}

void Parser::pushNull()
{
    Operand rec;
    rec.type = SymbolType::Null;
    _values.push(rec);
}

IntType Parser::popInt()
{
    IntType lastInt = _values.top().intValue;
    _values.pop();
    return lastInt;
}

bool Parser::popBoolean()
{
    bool lastBool = _values.top().boolValue;
    _values.pop();
    return lastBool;
}

RealType Parser::popReal()
{
    RealType lastReal = _values.top().realValue;
    _values.pop();
    return lastReal;
}

std::shared_ptr<Block> Parser::currentBlock() const
{
    return _currentBlock;
}

void Parser::addAndEntrySubBlock()
{
    _currentBlock = _currentBlock->addBlock();
    _emitter->startBlock(_currentBlock);
}

void Parser::exitToUpLevelBlock()
{
    if (!_currentBlock->parentBlock())
        throw std::domain_error("Can not exit from up-level block");
    _emitter->endBlock(_currentBlock);
    _currentBlock = _currentBlock->parentBlock();
}

int Parser::nextLabel()
{
    return _labelCounter++;
}

void Parser::pushJumpLabels(int startLabelId, int exitLabelId)
{
    _startLabels.push(startLabelId);
    _exitLabels.push(exitLabelId);
}

void Parser::popJumpLabels()
{
    _startLabels.pop();
    _exitLabels.pop();
}


Operand Parser::popStackValue()
{
    Operand rec = stackValue();
    _values.pop();
    return rec;
}

Operand Parser::stackValue()
{
    return _values.top();
}

/////////////////////// обработка ошибок //////////////////////////////////////

void Parser::error(const std::string &msg)
{
    throw SyntaxError(_lexer->line(), _lexer->pos(), msg);
}

void Parser::expected(Token expectedToken) noexcept(false)
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
    undeclaredIdentifier(tokenText());
}

void Parser::undeclaredIdentifier(const std::u32string &s)
{
    error("Undeclared identifier: " + to_utf8(s));
}

void Parser::duplicateIdentifier(const std::u32string &id)
{
    error("Duplicate identifier: " + to_utf8(id));
}

} // namespace escript
