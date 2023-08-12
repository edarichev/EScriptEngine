#ifndef ESCRIPT_OPERANDRECORD_H
#define ESCRIPT_OPERANDRECORD_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;
class Block;
class StringObject;

struct ESCRIPT_EXPORT Operand
{
    SymbolType type = SymbolType::Undefined;
    union {
        IntType intValue = 0;
        RealType realValue;
        bool boolValue;
        Symbol *variable;
        Symbol *function;
        Block *block;
        StringObject *strValue;
    };
    Operand();
    explicit Operand(IntType value);
    explicit Operand(RealType value);
    explicit Operand(bool value);
    explicit Operand(Symbol *symbol, SymbolType stype);
    std::string toString() const;
};

} // namespace escript

#endif // ESCRIPT_OPERANDRECORD_H
