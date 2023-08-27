/**
 * @file objectrecord.h
 * @brief Класс записи в таблице объектов.
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
#ifndef ESCRIPT_OBJECTRECORD_H
#define ESCRIPT_OBJECTRECORD_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;
/**
 * @brief Запись для таблицы объектов
 */
struct ESCRIPT_EXPORT ObjectRecord
{
public:
    /**
     * @brief В момент присваивания: если объект является внешним,
     *        и у него установлено свойство desctructible, то
     *        это поле (managed) должно быть установлено в false.
     *        Это означает, что все объекты по умолчанию (при managed == false)
     *        будут уничтожены при разрушении движка. Объекты, у которых
     *        установлено managed == false, разрушены не будут, т.к.
     *        за их существование отвечает внешняя по отношению ко скрипту
     *        программа.
     */
    bool managed = true;
    /**
     * @brief Cчётчик ссылок
     */
    int counter = 0;
    /**
     * @brief Тип хранимых данных
     */
    SymbolType type = SymbolType::Null;
    /**
     * @brief Если тип простой, то это сами данные. Если строка или объект -
     *        указатель на объект в куче.
     */
    PtrIntType data = 0;
    /**
     * @brief Необязательно: Обратная ссылка на запись в таблице символов.
     */
    Symbol *symbol = nullptr;
    ObjectRecord() = default;
    ObjectRecord(const ObjectRecord &rhs) = default;
    ObjectRecord(ObjectRecord &&rhs);
    ObjectRecord(Symbol *s) : symbol(s) {}
    virtual ~ObjectRecord();
};

} // namespace escript

#endif // ESCRIPT_OBJECTRECORD_H
