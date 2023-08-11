/**
 * @file storage.cpp
 * @brief Хранилище объектов
 */
#include "stdafx.h"
#include "storage.h"
#include "function.h"

namespace escript {


Storage::Storage()
{

}

Storage::~Storage()
{

}

ObjectRecord *Storage::findRecord(Symbol *symbol)
{
    for (ObjectRecord &r : _records) {
        if (r.symbol == symbol)
            return &r;
    }
    return nullptr;
}


} // namespace escript
