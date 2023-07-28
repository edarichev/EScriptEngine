/**
 * @file lexer.h
 * @brief Лексический анализатор - объявление
 */
#ifndef LEXER_H
#define LEXER_H

#include "EScript_global.h"
#include "token.h"

namespace escript {

/**
 * @brief Лексический анализатор
 */
class ESCRIPT_EXPORT Lexer
{
private:
    // разбираемый текст
    const std::u32string &_text;
    // текущая позиция в строке
    int _pos { 0 };
    // последний разобранный токен
    Token _token = Token::Eof;
    // строковое представление последнего разобранного токена
    std::u32string _tokenText;
public:
    /**
     * @brief Создаёт новый экземпляр класса Lexer
     */
    Lexer(const std::u32string &strCode);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Lexer();
    /**
     * @brief Возвращает следующий токен из входного потока
     * @return Token::Eof в случе достижения конца потока
     */
    Token next();
    /**
     * @brief Возвращает последний разобранный токен
     * @return
     */
    Token token() const;
    /**
     * @brief Возвращает строковое представление последнего разобранного токена
     * @return
     */
    const std::u32string &tokenText() const;
};

} // namespace escript

#endif // LEXER_H
