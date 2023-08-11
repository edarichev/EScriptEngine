/**
 * @file parser.h
 * @brief Синтаксический анализатор - объявление
 */
#ifndef PARSER_H
#define PARSER_H

#include "EScript_global.h"
#include "lexer.h"
#include "icodeemitter.h"
#include <stack>
#include <deque>
#include "tcode.h"
#include "pvalue.h"

namespace escript {

class Unit;
class SymbolTable;
class Symbol;
class Block;

/**
 * @brief Cинтаксический анализатор
 */
class ESCRIPT_EXPORT Parser
{
private:
    std::unique_ptr<Lexer> _lexer;
    std::shared_ptr<Unit> _unit;
    std::shared_ptr<Block> _rootBlock;
    std::shared_ptr<Block> _currentBlock;
    // стек для типа обнаруженного символа
    std::stack<SymbolType> _types;
    // стек для обнаруженных переменных в выражениях
    std::stack<std::shared_ptr<Symbol> > _variables;
    std::stack<std::shared_ptr<Symbol> > _functions;
    // стек для обнаруженных целых чисел в выражениях
    std::stack<IntType> _integers;
    std::stack<RealType> _reals;
    std::stack<bool> _booleans;
    std::stack<StringType*> _strings;
    std::unique_ptr<ICodeEmitter> _emitter;
    // типы лексем для возврата
    std::deque<Token> _tokensQueue;
    // тексты лексем для возврата
    std::deque<std::u32string> _tokenTextsQueue;
    // счётчик меток
    int _lableCounter = 0;
    // стеки меток для поддержки break/continue
    std::stack<int> _startLabels;
    std::stack<int> _exitLabels;
    // стек для значений
    std::stack<PValue> _valueStack;
    // для подсчёта аргументов в ArgumentsList
    std::stack<int> _argumentsCountStack;
    // для return - функции, return - к ближайшей функции
    std::stack<std::shared_ptr<Symbol> > _returnStack;
public:
    /**
     * @brief Создаёт новый экземпляр класса Parser
     */
    Parser(std::shared_ptr<Unit> &unit,
           std::shared_ptr<Block> &block,
           std::vector<TCode> &outBuffer);
    /**
     * @brief производит синтаксический разбор строки кода
     * @param strCode строка кода
     */
    void parse(const std::u32string &strCode);
    // правила: см. файл грамматики
    
private:
    void StatementList();
    void Statement();
    void CompoundStatement();
    void IfElseStatement();
    void WhileStatement();
    void DoWhileStattement();
    void ForStatement();
    void OptionalStatement();
    void BreakStatement();
    void ContinueStatement();
    void FunctionDeclStatement();
    void OptionalParameterDeclList();
    void OptionalExpressionList();
    void ParameterDeclList();
    void ExpressionList();
    void AssignExpression();
    void FunctionDeclExpression();
    void Variable();
    void Expression();
    void SimpleExpression();
    void SimpleOrShiftExpression();
    void ShiftOrRelationExpression();
    void RelationOrEqualityExpression();
    void BitwiseAndOrEqualityExpression();
    void BitwiseXOROrAndExpression();
    void BitwiseOROrXORExpression();
    void LogicalAndOrBitORExpression();
    void LogicalOrOrAndExpression();
    void LogicalOrNCOExpression();
    void Term();
    void Factor();
    void FunctionCallExpression();
    void ArgumentList();
    void AnyStatement();
    void ReturnStatement();
    // перемещение по потоку
private:
    /**
     * @brief Проверяет на совпадение текущий токен с указанным токеном
     *        и в случае ошибки выбрасывает исключение, в случае
     *        успеха считывает следующий токен.
     * @param t токен, с которым должен совпадать текущий символ
     */
    void match(Token t);
    /**
     * @brief Читает следующий токен
     */
    void next();
    /**
     * @brief Возвращает текущий токен лексического анализатора.
     */
    Token lookahead();
    /**
     * @brief Возвращает текущий токен
     */
    const std::u32string &tokenText() const;
    // emitter
private:
    /**
     * @brief Возвращает id следующей метки.
     * @return
     */
    int nextLabel();
    void pushJumpLabels(int startLabelId, int exitLabelId);
    void popJumpLabels();
    /**
     * @brief Выводит инструкцию для if с меткой.
     *        Применяется к переменной, находящейся наверху стека.
     * @param exitOrFalseLabelId метка для ветки false или выхода,
     *        в зависимости то того, есть ли далее блок "else"
     */
    void emitIfFalseHeader(int exitOrFalseLabelId);
    void emitGoto(int labelId);
    void emitLabel(int labelId);
    void emitBinaryOp(OperationType opType);
    void emitBreak();
    void emitContinue();
    /**
     * @brief Унарная операция. Применяется к переменной, находящейся наверху
     *        стека.
     * @param opType тип операции
     */
    void emitUnaryOp(OperationType opType);
    void emitAssign(std::shared_ptr<Symbol> &lvalue);
    /**
     * @brief Записывает начало функции.
     * Текст текущего токена - имя функции в сигнатуре.
     */
    void emitFnStart(std::shared_ptr<Symbol> &func);
    /**
     * @brief Текст текущего токена - имя параметра функции в сигнатуре.
     */
    void emitFnArg();
    /**
     * @brief При генерации кода будет использован адрес записи в таблице
     *        симоволов для связи объект Function и стартового адреса.
     * @param func
     */
    void emitFnCode(std::shared_ptr<Symbol> &func);
    void emitLoadFnArgs();
    void emitPush(); // для return
    void emitRet(); // для return
    void emitEmptyReturn();
    void emitReturn();
    void emitCall(std::shared_ptr<Symbol> &func, int nArgs, std::shared_ptr<Symbol> &resultVariable);
    void emitFnEnd();
    // работа с символами
private:
    /**
     * @brief Возвращает текущую таблицу символов
     */
    std::shared_ptr<SymbolTable> currentSymbolTable();
    void pushInt(IntType value);
    void pushReal(RealType value);
    void pushBoolean(bool value);
    void pushVariable(std::shared_ptr<Symbol> &variable);
    void pushFunction(std::shared_ptr<Symbol> &func);
    std::pair<SymbolType, OperandRecord> popStackValue();
    std::pair<SymbolType, OperandRecord> stackValue();
    void pushBack(Token t, const std::u32string &str);
    void pushBack(Token t, std::u32string &&str);
    IntType popInt();
    IntType popReal();

    std::shared_ptr<Block> currentBlock() const;
    /**
     * @brief Добавляет дочерний блок и переходит в него
     * @return
     */
    void addAndEntrySubBlock();
    /**
     * @brief Выходит из блока в родительский блок
     */
    void exitToUpLevelBlock();
    // обработка ошибок
private:
    void error(const std::string &msg);
    void expected(Token expectedToken);
    void unexpected(Token unexpectedToken);
    void undeclaredIdentifier();
    static std::string toUtf8(const std::u32string &s);
};

} // namespace escript

#endif // PARSER_H
