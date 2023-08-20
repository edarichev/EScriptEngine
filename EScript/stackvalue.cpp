#include "stdafx.h"
#include "stackvalue.h"
#include "objectrecord.h"
#include "stringobject.h"
#include "array.h"
#include <cmath>
#include <utility>

namespace escript {

const std::u32string StackValue::uString() const
{
    return to_u32string(type, value);
}

int64_t to_int64(const std::u32string &s)
{
    auto it = s.begin();
    for (; it != s.end(); ++it) {
        if (*it >= '0' && *it <= 9)
            continue;
        switch (*it) {
        case '.':
        case '+':
        case '-':
        case 'e':
        case 'E':
            continue;
        default:
            break;
        }
        break;
    }
    std::string sd(s.begin(), it);
    if (sd.empty())
        return 0;
    return std::stoll(sd);
}

int64_t intValue(SymbolType type, uint64_t value)
{
    switch (type) {
    case SymbolType::Boolean:
        return value ? true : false;
    case SymbolType::Integer:
        return (int64_t)value;
    case SymbolType::Real:
        return (int64_t)bit_cast<double>(value);
    case SymbolType::String:
        return to_int64(((StringObject*)value)->uString());
    case SymbolType::Variable:
        return intValue(((ObjectRecord*)value)->type, ((ObjectRecord*)value)->data);
    default:
        throw std::domain_error("Can not convert value to Integer");
    }
}

int64_t StackValue::getIntValue() const
{
    return intValue(type, value);
}



} // namespace escript
