/**
 * @file keyword.h
 * @brief Объявление класса KeyWord
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
#ifndef KEYWORD_H
#define KEYWORD_H

#include "token.h"
#include "EScript_global.h"

namespace escript {

/**
 * @brief Класс для определения ключевых и зарезервированных слов
 */
class ESCRIPT_EXPORT KeyWord
{
public:
    /**
     * @brief Определяет, является ли данная строка ключевым/зарезервированным
     *        словом
     * @param str определяемое строковое представление токена
     * @return Token::Eof, если это не ключевое слово, иначе -
     *         соответствующий токен.
     */
    static Token checkKeyWord(const std::u32string &str);
};

} // namespace escript

#endif // KEYWORD_H
