/**
 * @file storage.cpp
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
#include "stdafx.h"
#include "storage.h"
#include "functionobject.h"

namespace escript {


Storage::Storage()
{

}

Storage::~Storage()
{
    clear();
}

void Storage::clear()
{
    checkDuplicates();
    for (auto &r : _records) {
        auto p = tryCastToObject(r);
        if (p == nullptr)
            continue;
        if (r.type == SymbolType::String) {
            // таблица строк сама всё удаляет
            continue;
        }
        if (!r.managed)
            continue;
        if (!AutomationObject::exists(p)) {
            // а такого быть не должно
            throw std::domain_error("Storage::clear: The object does not exists");
        }
        while (p->counter() > 1)
            p->release();
    }
    _records.clear();
}

ObjectRecord *Storage::findRecord(Symbol *symbol)
{
    for (ObjectRecord &r : _records) {
        if (r.symbol == symbol)
            return &r;
    }
    return nullptr;
}

ObjectRecord *Storage::findRecord(SymbolType type, uint64_t dataOrPointer)
{
    for (ObjectRecord &r : _records) {
        if (r.data == dataOrPointer && r.type == type)
            return &r;
    }
    return nullptr;
}

void Storage::removeRecord(AutomationObject *obj)
{
    for (auto it = _records.begin(); it != _records.end(); ++it) {
        ObjectRecord &r = *it;
        if (r.type == SymbolType::Object &&
                r.data == (uint64_t)obj) {
            r.data = 0;
            _records.erase(it);
            return;
        }
    }
}

void Storage::checkDuplicates() const
{
    for (auto it = _records.begin(); it != prev(prev(_records.end())); ++it) {
        switch ((*it).type) {
        case SymbolType::Array:
        case SymbolType::Function:
        case SymbolType::Object: {
            AutomationObject *pObj1 = (AutomationObject *)(*it).data;
            for (auto it2 = next(it); it2 != _records.end(); ++it2) {
                switch ((*it).type) {
                case SymbolType::Array:
                case SymbolType::Function:
                case SymbolType::Object: {
                    AutomationObject *pObj2 = (AutomationObject *)(*it2).data;
                    auto d1 = std::distance(_records.begin(), it);
                    auto d2 = std::distance(_records.begin(), it2);
                    if (pObj1 == pObj2) {
                        std::cerr << "Duplicate records: pos1=" << d1 << ", pos2=" << d2 << std::endl;
                    }
                    assert(pObj1 != pObj2);
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

AutomationObject *Storage::tryCastToObject(const ObjectRecord &r) const
{
    switch (r.type) {
    case SymbolType::Array:
    case SymbolType::String:
    case SymbolType::Function:
    case SymbolType::Object:
        return (AutomationObject*)r.data;
    default:
        break;
    }
    return nullptr;
}


} // namespace escript
