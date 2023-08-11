#include "stdafx.h"
#include "objectrecord.h"
#include "function.h"

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
