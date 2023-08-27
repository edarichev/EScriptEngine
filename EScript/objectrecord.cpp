/**
 * @file objectrecord.cpp
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
#include "stdafx.h"
#include "objectrecord.h"
#include "functionobject.h"
#include "arrayobject.h"

namespace escript {

ObjectRecord::ObjectRecord(ObjectRecord &&rhs)
{
    if (this == &rhs)
        return;
    counter = rhs.counter;
    type = rhs.type;
    data = rhs.data;
    rhs.counter = 0;
    rhs.data = 0;
    rhs.type = SymbolType::Undefined;
}

ObjectRecord::~ObjectRecord()
{
    if (data && managed) {
        switch (type) {
        case SymbolType::Array:
        case SymbolType::Function:
        case SymbolType::Object: {
            AutomationObject *obj = (AutomationObject*)data;
            if (!AutomationObject::exists(obj)) {
                std::cerr << "~ObjectRecord: The object does not exists\n";
                return;
            }
            obj->release();
            if (!obj->managed())
                return;
            if (obj->counter() == 0)
                delete obj;
            break;
        }
        default:
            break;
        }
    }
}


} // namespace escript
