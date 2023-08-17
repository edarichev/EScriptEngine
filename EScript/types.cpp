#include "stdafx.h"
#include "types.h"
#include "objectrecord.h"
#include "stringobject.h"
#include "array.h"

using namespace std;

namespace escript {

std::u32string to_u32string(const SymbolType &t, uint64_t v)
{
    std::ostringstream strs;
    std::string s1;
    switch (t) {
    case SymbolType::Integer:
        strs << (int64_t)v;
        s1 = strs.str();
        break;
    case SymbolType::Boolean:
        s1 = v ? "true" : "false";
        break;
    case SymbolType::Real: {
        strs << std::defaultfloat << bit_cast<double>(v);
        s1 = strs.str();
        break;
    }
    case SymbolType::String:
        return ((StringObject*)v)->uString();
    case SymbolType::Array:
        return ((Array*)v)->uString();
    case SymbolType::Function:
        return U"function";
    case SymbolType::Null:
        return U"null";
    case SymbolType::Undefined:
        return U"undefined";
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)v;
        return to_u32string(rec->type, rec->data);
    }
    default:
        throw std::domain_error("Conversion from SymbolType to u32string not supported");
    }
    return std::u32string(s1.begin(), s1.end());
}


} // namespace escript
