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
            if (!AutomationObject::exists(obj)) {
                std::cerr << "~ObjectRecord: The object does not exists\n";
                return;
            }
            obj->release();
            if (!obj->destructible())
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
