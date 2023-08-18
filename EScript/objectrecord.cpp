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
            if (!reference) {
                delete (Array*)data;
            }
            break;
        case SymbolType::Function:
            if (!reference) {
                delete (Function*)data;
            }
            break;
        case SymbolType::Object:
            if (!reference) {
                AutomationObject *obj = (AutomationObject*)data;
                obj->release();
            }
        default:
            break;
        }
    }
}


} // namespace escript
