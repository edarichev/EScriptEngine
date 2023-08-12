#include "stdafx.h"
#include "operandrecord.h"
#include "symbol.h"

namespace escript {

OperandRecord::OperandRecord(){}

OperandRecord::OperandRecord(IntType value)
{
    intValue = value;
    type = SymbolType::Integer;
}

OperandRecord::OperandRecord(RealType value)
{
    realValue = value;
    type = SymbolType::Real;
}

OperandRecord::OperandRecord(bool value)
{
    boolValue = value;
    type = SymbolType::Boolean;
}

OperandRecord::OperandRecord(Symbol *symbol, SymbolType stype)
{
    switch (stype) {
    case SymbolType::Function:
        type = stype;
        function = symbol;
        break;
    case SymbolType::Variable:
        type = stype;
        variable = symbol;
        break;
    default:
        throw std::domain_error("Not supported type for Operand");
    }
}

std::string OperandRecord::toString() const
{
    std::string opStr;
    switch (type) {
    case SymbolType::Integer:
        opStr = std::to_string(intValue);
        break;
    case SymbolType::Real:
        opStr = std::to_string(realValue);
        break;
    case SymbolType::Variable:
        opStr = variable->utf8Name();
        break;
    case SymbolType::Boolean:
        opStr = boolValue ? "true" : "false";
        break;
    case SymbolType::Function:
        opStr = "function";
        break;
    case SymbolType::Undefined:
        opStr = "undefined";
        break;
    default:
        throw std::domain_error("Unsupported SymbolType");
    }
    return opStr;
}



} // namespace escript
