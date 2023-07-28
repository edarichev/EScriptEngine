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
};

} // namespace escript

#endif // PARSER_H
