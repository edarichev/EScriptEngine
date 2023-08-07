#include "stdafx.h"
#include "pvalue.h"

namespace escript {

bool operator<(const PValue &lhs, const PValue &rhs)
{
    switch (lhs.type) {
    case SymbolType::Boolean:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return lhs.boolValue < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.boolValue < rhs.intValue ? true : false;
        case SymbolType::Real:
            return lhs.boolValue < rhs.realValue ? true : false;
        default:
            return false;
        }
        break;
    case SymbolType::Integer:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return (bool)lhs.intValue < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.intValue < rhs.intValue;
        case SymbolType::Real:
            return lhs.intValue < rhs.realValue;
        default:
            return false;
        }
        break;
    case SymbolType::Real:
        switch (rhs.type) {
        case SymbolType::Boolean:
            return lhs.realValue ? 1 : 0 < rhs.boolValue;
        case SymbolType::Integer:
            return lhs.realValue < rhs.intValue;
        case SymbolType::Real:
            return lhs.realValue < rhs.realValue;
        default:
            return false;
        }
        break;
    default:
        return false;
    }
    return false;
}
} // namespace escript
