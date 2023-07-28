/**
 * @file keyword.h
 * @brief Объявление класса KeyWord
 */
#ifndef KEYWORD_H
#define KEYWORD_H

#include "token.h"

namespace escript {

/**
 * @brief Класс для определения ключевых слов
 */
class KeyWord
{
public:
    /**
     * @brief Определяет, является ли данная строка ключевым словом
     * @param str определяемое строковое представление токена
     * @return Token::Eof, если это не ключевое слово, иначе -
     *         соответствующий токен.
     */
    static Token checkKeyWord(const std::u32string &str);
};

} // namespace escript

#endif // KEYWORD_H
