#ifndef ESCRIPT_OPERANDRECORD_H
#define ESCRIPT_OPERANDRECORD_H

#include "EScript_global.h"
#include "types.h"

namespace escript {

class Symbol;
class Block;

struct ESCRIPT_EXPORT OperandRecord
{
    SymbolType type = SymbolType::Undefined;
    union {
        IntType intValue = 0;
        RealType realValue;
        bool boolValue;
        Symbol *variable;
        Symbol *function;
        Block *block;
    };
    OperandRecord();
    explicit OperandRecord(IntType value);
    explicit OperandRecord(RealType value);
    explicit OperandRecord(bool value);
    explicit OperandRecord(Symbol *symbol, SymbolType stype);
    std::string toString() const;
};

} // namespace escript

#endif // ESCRIPT_OPERANDRECORD_H
