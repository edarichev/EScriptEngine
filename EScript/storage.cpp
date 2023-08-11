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
    if (data) {
        switch (type) {
        case SymbolType::Function:
            delete (Function*)data;
            break;
        default:
            break;
        }
    }
}

} // namespace escript
