/**
 * @file parser.h
 * @brief Синтаксический анализатор - объявление
 */
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

namespace escript {

#include "EScript_global.h"

class ESCRIPT_EXPORT Parser
{
private:
    std::unique_ptr<Lexer> _lexer;
public:
    /**
     * @brief Создаёт новый экземпляр класса Parser
     */
    Parser();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Parser();
    /**
     * @brief производит синтаксический разбор строки кода
     * @param strCode строка кода
     */
    void parse(const std::u32string &strCode);
    // правила: см. файл грамматики
private:
    void StatementList();
    void Statement();
    void AssignStatement();
    void Variable();
    void Expression();
    void SimpleExpression();
    void Term();
    void Factor();
    // перемещение
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
    // обработка ошибок
private:
    void error(const std::string &msg);
    void expected(Token expectedToken);
    void unexpected(Token unexpectedToken);
};

} // namespace escript

#endif // PARSER_H
