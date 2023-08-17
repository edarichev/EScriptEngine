#include "stdafx.h"
#include "stackvalue.h"
#include "objectrecord.h"
#include "stringobject.h"
#include "array.h"

namespace escript {

const std::u32string StackValue::uString() const
{
    return to_u32string(type, value);
}



} // namespace escript
