/**
 * @file storage.h
 * @brief Хранилище объектов
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
#ifndef STORAGE_H
#define STORAGE_H

#include "EScript_global.h"
#include <forward_list>
#include <list>
#include "types.h"
#include "objectrecord.h"

namespace escript {

class AutomationObject;

/**
 * @brief Хранилище объектов
 */
class ESCRIPT_EXPORT Storage
{
    // записи для ссылок на объекты в куче
    std::list<ObjectRecord> _records;
public:
    /**
     * @brief Создаёт новый экземпляр класса Storage
     */
    Storage();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Storage();
    /**
     * @brief Удаляет все записи об объектах
     */
    void clear();
    /**
     * @brief Добавить пустую запись
     */
    ObjectRecord *installRecord(Symbol *s)
    {
        return &_records.emplace_front(s);
    }
    ObjectRecord *findRecord(Symbol *symbol);
    ObjectRecord *findRecord(SymbolType type, uint64_t dataOrPointer);
    void removeRecord(AutomationObject *obj);
    void checkDuplicates() const;
    AutomationObject *tryCastToObject(const ObjectRecord &r) const;
};

} // namespace escript

#endif // STORAGE_H
