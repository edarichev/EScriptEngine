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


} // namespace escript
