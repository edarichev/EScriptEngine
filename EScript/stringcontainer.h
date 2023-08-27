/**
 * @file stringcontainer.h
 * @brief Хранилище строк
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
#ifndef ESCRIPT_STRINGCONTAINER_H
#define ESCRIPT_STRINGCONTAINER_H

#include "EScript_global.h"
#include <list>

namespace escript {

class StringObject;

/**
 * @brief Хранилище строк
 */
class ESCRIPT_EXPORT StringContainer
{
    std::list<StringObject*> _strings;
public:
    /**
     * @brief Создаёт новый экземпляр класса StringContainer
     */
    StringContainer();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~StringContainer();
    /**
     * @brief Удаляет все строки
     */
    void clear();
    /**
     * @brief Добавляет строку и возвращает её индекс
     * @param s
     * @return
     */
    StringObject *add(const std::u32string &s);
    StringObject *add(StringObject *s);
    void checkDuplicates() const;
private:
    StringObject *findString(const std::u32string &s) const;
};

} // namespace escript

#endif // ESCRIPT_STRINGCONTAINER_H
