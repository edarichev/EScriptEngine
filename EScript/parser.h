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
    // стек для обнаруженных целых чисел в выражениях
    std::stack<IntType> _integers;
    std::stack<RealType> _reals;
    std::stack<StringType*> _strings;
    std::unique_ptr<ICodeEmitter> _emitter;
    // типы лексем для возврата
    std::deque<Token> _tokensQueue;
    // тексты лексем для возврата
    std::deque<std::u32string> _tokenTextsQueue;
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
    void AssignStatement();
    void AssignExpression();
    void Variable();
    void Expression();
    void SimpleExpression();
    void Term();
    void Factor();
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
    // работа с символами
private:
    /**
     * @brief Возвращает текущую таблицу символов
     */
    std::shared_ptr<SymbolTable> currentSymbolTable();
    void pushInt(IntType value);
    void pushReal(RealType value);
    void pushVariable(std::shared_ptr<Symbol> &variable);
    void emitBinaryOp(OperationType opType);
    /**
     * @brief Унарная операция. Применяется к переменной, находящейся наверху
     *        стека.
     * @param opType тип операции
     */
    void emitUnaryOp(OperationType opType);
    void emitAssign(std::shared_ptr<Symbol> &lvalue);
    std::pair<SymbolType, OperandRecord> popStackValue();
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
