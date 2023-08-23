#include "stdafx.h"
#include "objectrecord.h"
#include "function.h"
#include "array.h"

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
        case SymbolType::Array:
        case SymbolType::Function:
        case SymbolType::Object: {
            AutomationObject *obj = (AutomationObject*)data;
            obj->release();
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
