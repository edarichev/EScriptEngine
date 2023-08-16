/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"
#include "unit.h"
#include "stringcontainer.h"
#include "automationobject.h"

using namespace std;

namespace escript {

Parser::Parser(std::shared_ptr<Block> &block, StringContainer &strContainer,
               std::vector<TCode> &outBuffer)
    : _rootBlock(block), _currentBlock(block), _strings(strContainer)
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
    Expression(); // теперь в стеке что-то есть
    match(Token::RightParenth);
    int falseLabel = nextLabel();
    emitIfFalseHeader(falseLabel); // if_false tmp_var метка_false
    OptionalStatement(); // ветка true
    if (lookahead() == Token::Else) {
        next();
        int exitLabel = nextLabel();
        emitGoto(exitLabel);
        emitLabel(falseLabel);
        OptionalStatement(); // ветка false
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
    match(Token::For);
    match(Token::LeftParenth);
    OptionalExpressionList();      // expr1
    match(Token::Semicolon);
    int startLabel = nextLabel();  // метка возврата в начало цикла
    int exitLabel = nextLabel();   // метка выхода
    // в этом цикле expr3 находится в конце, поэтому переход
    // по continue должен идти не назад, а вперёд до expr3
    int nextIterationLabel = nextLabel();
    pushJumpLabels(nextIterationLabel, exitLabel); // для break/continue
    emitLabel(startLabel);
    if (lookahead() != Token::Semicolon) // условия может и не быть
        Expression();                    // expr2, логическое условие
    else { // иначе это эквивалентно while (true), поэтому
        pushBoolean(true);               // помещаем в стек true
    }
    emitIfFalseHeader(exitLabel);  // аналогично заголовку в if-else
    match(Token::Semicolon);
    _emitter->switchToTempBuffer();
    OptionalExpressionList();      // expr3, её вывести в конец
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
                         // или оставить результат последнего выражения?
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
    Variable();
    auto lvalueSymbol = _values.top().variable;
    popStackValue(); // результат в lvalueSymbol, просто вынем из стека
    if (lookahead() == Token::Assign) {
        match(Token::Assign);
        Expression();
        emitAssign(lvalueSymbol);
        // поскольку присваивание само является выражением, нужно поместить в стек
        // результат вычислений
        pushVariable(lvalueSymbol);
    } else {
        expected(Token::Assign);
    }
}

void Parser::FunctionDeclExpression()
{
    // метка для прыжка через функцию, если
    // идёт последовательное выполнение кода
    int labelEnd = nextLabel();
    emitGoto(labelEnd);
    match(Token::Function);
    // временная переменная для функции
    std::shared_ptr<Symbol> func = currentSymbolTable()->add(tokenText());
    _returnStack.push(func);
    addAndEntrySubBlock();
    emitFnStart(func);
    match(Token::Identifier);
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
        // перейти в SimpleExpression
        break;
    }
    case Token::Function:
        FunctionDeclExpression();
        return;
    case Token::LeftBracket:
        ArrayDeclExpression();
        return;
    default:
        break;
    }
    LogicalOrNCOExpression();
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
    default:
        return;
    }
    next();
    ShiftOrRelationExpression();
    emitBinaryOp(opType);
}

void Parser::BitwiseAndOrEqualityExpression()
{
    RelationOrEqualityExpression();
    if (lookahead() == Token::Ampersand) {
        // побитовое AND
        next();
        RelationOrEqualityExpression();
        emitBinaryOp(OperationType::BitAND);
    }
}

void Parser::BitwiseXOROrAndExpression()
{
    BitwiseAndOrEqualityExpression();
    if (lookahead() == Token::Caret) {
        // побитовое XOR
        next();
        BitwiseAndOrEqualityExpression();
        emitBinaryOp(OperationType::BitXOR);
    }
}

void Parser::BitwiseOROrXORExpression()
{
    BitwiseXOROrAndExpression();
    if (lookahead() == Token::Vertical) {
        // побитовое OR
        next();
        BitwiseXOROrAndExpression();
        emitBinaryOp(OperationType::BitOR);
    }
}

void Parser::LogicalAndOrBitORExpression()
{
    BitwiseOROrXORExpression();
    if (lookahead() == Token::LogicalAnd) {
        // логическое AND
        next();
        BitwiseOROrXORExpression();
        emitBinaryOp(OperationType::LogAND);
    }
}

void Parser::LogicalOrOrAndExpression()
{
    LogicalAndOrBitORExpression();
    if (lookahead() == Token::LogicalOr) {
        // логическое OR
        next();
        LogicalAndOrBitORExpression();
        emitBinaryOp(OperationType::LogOR);
    }
}

void Parser::LogicalOrNCOExpression()
{
    LogicalOrOrAndExpression();
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
{ // Здесь объединены правила Literals, Grouping, CallOrAccess, PostfixOperation
    switch (lookahead()) {
    case Token::LeftParenth:
        // круглые скобки
        next();
        Expression();
        match(Token::RightParenth);
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
            break;
        case Token::LeftBracket:
            // обращение к элементу массива
            pushBack(Token::Identifier, idText);
            ArrayItemRefExpression();
            break;
        case Token::Dot:
            // DotOperation: здесь важно различить: это слева от = или справа
            pushBack(Token::Identifier, idText);
            DotOperation();
            break;
        default:
            auto symbol = currentSymbolTable()->find(idText);
            if (!symbol)
                undeclaredIdentifier();
            pushVariable(symbol);
            break; // просто идентификатор
        }
        break;
    }
    case Token::IntegerNumber:
        pushInt(_lexer->lastIntegerNumber());
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
    case Token::QuotedString:
        pushString(_lexer->tokenText());
        next();
        break;
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
        // выражение вставляет в стек аргумент
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
        error("return allowed only inside of function");
    match(Token::Return);
    if (lookahead() == Token::Semicolon) {
        emitEmptyReturn();
        return;
    }
    Expression();
    emitReturn();
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
    // ArrayDeclItems - здесь, не в отдельном правиле
    do {
        if (lookahead() == Token::RightBracket)
            break;
        Expression();
        auto e = currentSymbolTable()->addTemp();
        emitAssign(e.get());
        pushVariable(e);
        emitPush();
        // в результате вызова всегда что-то есть, даже 0
        auto resultVariable = currentSymbolTable()->addTemp();
        emitCallAOMethod(arrValue, U"add", resultVariable, 1);
        pushVariable(resultVariable);
        if (lookahead() == Token::RightBracket)
            break;
        match(Token::Comma);
    } while (lookahead() != Token::Eof);
    match(Token::RightBracket);
    pushVariable(arrValue);
}

void Parser::ArrayItemRefExpression()
{
    auto id = tokenText();
    auto arrValue = currentSymbolTable()->find(id);
    match(Token::Identifier);
    match(Token::LeftBracket);
    Expression(); // индекс элемента
    emitPush();
    match(Token::RightBracket);
    if (lookahead() == Token::Assign) {
        // присваивание элементу массива
        next();
        Expression(); // новое значение
        emitPush();
        auto resultVariable = currentSymbolTable()->addTemp();
        emitCallAOMethod(arrValue, U"set", resultVariable, 2);
        pushVariable(resultVariable);
    } else {
        // получение значения элемента массива
        auto resultVariable = currentSymbolTable()->addTemp();
        emitCallAOMethod(arrValue, U"get", resultVariable, 1);
        pushVariable(resultVariable);
    }
}

void Parser::DotOperation()
{
    // пока только одноуровневые: id.свойство, а не id.prop1.prop2.prop3
    auto variableName = tokenText();
    match(Token::Identifier);
    auto symbol = currentSymbolTable()->find(variableName);
    if (!symbol)
        undeclaredIdentifier();
    match(Token::Dot);
    auto methodName = tokenText();
    match(Token::Identifier);
    auto resultVariable = currentSymbolTable()->addTemp();
    int nArgs = 0;
    if (lookahead() == Token::LeftParenth) {
        // это метод
        match(Token::LeftParenth);
        _argumentsCountStack.push(0);
        ArgumentList();
        nArgs = _argumentsCountStack.top();
        _argumentsCountStack.pop();
        match(Token::RightParenth);
    } else {
        // это свойство
        methodName = U"get_" + methodName;
    }
    emitCallAOMethod(symbol, methodName, resultVariable, nArgs);
    pushVariable(resultVariable);
}

void Parser::PostfixOperation()
{
    void (Parser::*pfn)();
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
    emitAssign(tmp.get());
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
    // зависит от того, что тут есть
    // если значение можно вычислить сейчас, то ветку false/true
    // можно выбросить и обойтись без ветвления
    // пока мы засунем во временную переменную
    Symbol *ptrValue = nullptr;
    if (valueType != SymbolType::Variable) {
        std::shared_ptr<Symbol> tmp;
        tmp = currentSymbolTable()->addTemp();
        ptrValue = tmp.get();
        emitAssign(ptrValue);
        pushVariable(tmp);
    } else {
        ptrValue = _values.top().variable;
    }
    // здесь должна быть только переменная в стеке
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
    switch (opType) {
    case OperationType::Add:
    case OperationType::Div:
    case OperationType::Minus:
    case OperationType::Multiply:
    case OperationType::Less:
    case OperationType::LessOrEqual:
    case OperationType::Greater:
    case OperationType::GreaterOrEqual:
    case OperationType::Equal:
    case OperationType::LShift:
    case OperationType::RShift:
    case OperationType::RShiftZero:
    case OperationType::BitAND:
    case OperationType::BitOR:
    case OperationType::BitXOR:
    case OperationType::LogAND:
    case OperationType::LogOR:
    {
        auto opRecord2 = popStackValue();
        auto opRecord1 = popStackValue();
        std::shared_ptr<Symbol> tmp = currentSymbolTable()->addTemp();
        _emitter->binaryOp(opType, tmp.get(), opRecord1, opRecord2);
        pushVariable(tmp);
        break;
    }
    default:
        throw std::domain_error("Invalid binary operation");
    }
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
            lastInt = -lastInt;
            pushInt(lastInt);
            break;
        case OperationType::BitNOT:
            lastInt = ~lastInt;
            pushInt(lastInt);
            break;
        case OperationType::LogNOT:
            lastInt = !lastInt;
            pushBoolean(lastInt ? true : false);
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
    _emitter->call(func, nArgs, resultVariable);
}

void Parser::emitFnEnd()
{
    _emitter->fnEnd();
}

void Parser::emitCallAOMethod(std::shared_ptr<Symbol> &leftVariable,
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

IntType Parser::popReal()
{
    RealType lastInt = _values.top().realValue;
    _values.pop();
    return lastInt;
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
    return _lableCounter++;
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
