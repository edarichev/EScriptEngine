/**
 * @file storage.cpp
 * @brief Хранилище объектов
 */
#include "stdafx.h"
#include "storage.h"

namespace escript {


Storage::Storage()
{

}

Storage::~Storage()
{

}

ObjectRecord *Storage::findRecord(Symbol *symbol)
{
    for (auto &r : _records) {
        if (r.symbol == symbol)
            return &r;
    }
    return nullptr;
}

} // namespace escript
