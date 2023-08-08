/**
 * @file parser.cpp
 * @brief Синтаксический анализатор - реализация
 */
#include "stdafx.h"
#include "parser.h"
#include "unit.h"

using namespace std;

namespace escript {

Parser::Parser(std::shared_ptr<Unit> &unit,
               std::shared_ptr<Block> &block,
               std::vector<TCode> &outBuffer)
    : _unit(unit), _rootBlock(block), _currentBlock(block)
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
    default:
        AssignStatement();
        // после AssignStatement всегда есть что-то, т.к. это выражение,
        // поэтому вытаскиваем из стека его результат
        popStackValue();
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

void Parser::AssignStatement()
{
    AssignExpression();
    match(Token::Semicolon);
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
    pushJumpLabels(startLabel, exitLabel); // для break/continue
    emitLabel(startLabel);
    Expression();                 // теперь в стеке что-то есть
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    match(Token::RightParenth);   // )
    OptionalStatement();          // тело цикла
    emitGoto(startLabel);         // возврат к условию
    emitLabel(exitLabel);         // выход
    popJumpLabels();             // убрать верхние метки
}

void Parser::DoWhileStattement()
{
    match(Token::Do);             // do
    match(Token::LeftBrace);      // {
    int startLabel = nextLabel(); // метка возврата в начало цикла
    int exitLabel = nextLabel();  // метка выхода
    pushJumpLabels(startLabel, exitLabel); // для break/continue
    emitLabel(startLabel);
    Statement();                  // тело цикла
    match(Token::RightBrace);     // }
    match(Token::While);          // while
    match(Token::LeftParenth);    // (
    Expression();                 // в стеке - условие продолжения
    match(Token::RightParenth);   // )
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    emitGoto(startLabel);         // возврат к условию
    emitLabel(exitLabel);         // выход
    popJumpLabels();             // убрать верхние метки
}

void Parser::ForStatement()
{
    match(Token::For);
    match(Token::LeftParenth);
    OptionalExpressionList();     // expr1
    match(Token::Semicolon);
    int startLabel = nextLabel(); // метка возврата в начало цикла
    int exitLabel = nextLabel();  // метка выхода
    pushJumpLabels(startLabel, exitLabel); // для break/continue
    emitLabel(startLabel);
    if (lookahead() != Token::Semicolon) // условия может и не быть
        Expression();                    // expr2, логическое условие
    else { // иначе это эквивалентно while (true), поэтому
        pushBoolean(true);               // помещаем в стек true
    }
    emitIfFalseHeader(exitLabel); // аналогично заголовку в if-else
    match(Token::Semicolon);
    _emitter->switchToTempBuffer();
    OptionalExpressionList();     // expr3, её вывести в конец
    _emitter->switchToMainBuffer();
    match(Token::RightParenth);
    OptionalStatement();
    _emitter->writeTempBuffer();  // вывести expr3
    emitGoto(startLabel);         // возврат к условию
    emitLabel(exitLabel);         // выход
    popJumpLabels();             // убрать верхние метки
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
    Variable();
    auto lvalueSymbol = _variables.top();
    popStackValue(); // результат в lvalueSymbol, просто вынем из стека
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
            AssignExpression();
            return;
        }
        // вернуть идентификатор (прямой порядок, это очередь)
        pushBack(token0, std::move(tokenText0));
        // перейти в SimpleExpression
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
}

void Parser::ShiftOrRelationExpression()
{
    SimpleOrShiftExpression();
}

void Parser::RelationOrEqualityExpression()
{
    ShiftOrRelationExpression();
    switch (lookahead()) {
    case Token::Less:
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(OperationType::Less);
        return;
    case Token::LessEqual:
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(OperationType::LessOrEqual);
        return;
    case Token::Greater:
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(OperationType::Greater);
        return;
    case Token::GreaterEqual:
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(OperationType::GreaterOrEqual);
        return;
    case Token::Equal:
        next();
        ShiftOrRelationExpression();
        emitBinaryOp(OperationType::Equal);
        return;
    default:
        break;
    }
}

void Parser::BitwiseAndOrEqualityExpression()
{
    RelationOrEqualityExpression();
}

void Parser::BitwiseXOROrAndExpression()
{
    BitwiseAndOrEqualityExpression();
}

void Parser::BitwiseOROrXORExpression()
{
    BitwiseXOROrAndExpression();
}

void Parser::LogicalAndOrBitORExpression()
{
    BitwiseOROrXORExpression();
}

void Parser::LogicalOrOrAndExpression()
{
    LogicalAndOrBitORExpression();
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
    case Token::True:
        pushBoolean(true);
        next();
        break;
    case Token::False:
        pushBoolean(false);
        next();
        break;
    default: // ошибка, нужен терминал в виде числа, идентификатора и т.п.
        expected(Token::Identifier);
    }
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
    auto valueType = _types.top();
    // зависит от того, что тут есть
    // если значение можно вычислить сейчас, то ветку false/true
    // можно выбросить и обойтись без ветвления
    // пока мы засунем во временную переменную
    Symbol *ptrValue = nullptr;
    if (valueType != SymbolType::Variable) {
        std::shared_ptr<Symbol> tmp;
        tmp = currentSymbolTable()->addTemp();
        emitAssign(tmp);
        ptrValue = tmp.get();
        pushVariable(tmp);
    } else {
        ptrValue = _variables.top().get();
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
    {
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
    _integers.push(value);
    _types.push(SymbolType::Integer);
}

void Parser::pushReal(RealType value)
{
    _reals.push(value);
    _types.push(SymbolType::Real);
}

void Parser::pushBoolean(bool value)
{
    _booleans.push(value);
    _types.push(SymbolType::Boolean);
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

std::shared_ptr<Block> Parser::currentBlock() const
{
    return _currentBlock;
}

void Parser::addAndEntrySubBlock()
{
    _currentBlock = _currentBlock->addBlock();
}

void Parser::exitToUpLevelBlock()
{
    if (!_currentBlock->parentBlock())
        throw std::domain_error("Can not exit from up-level block");
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
    case SymbolType::Boolean:
        rec.second.boolValue = _booleans.top();
        _booleans.pop();
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
