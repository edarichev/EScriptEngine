/**
 * @file icodeemitter.h
 * @brief Генератор промежуточного кода
 */
#ifndef ICODEEMITTER_H
#define ICODEEMITTER_H

#include "EScript_global.h"
#include "symbol.h"

namespace escript {

enum class OperationType
{
    Multiply,       // op1*op2
    Add,            // op1+op2
};

/**
 * @brief Генератор промежуточного кода
 * @details (I == intermediate), не интерфейс.
 */
class ESCRIPT_EXPORT ICodeEmitter
{
public:
    void binaryOp(OperationType operationType,
                  std::shared_ptr<Symbol> resultVariable,
                  SymbolType operand1Type, void *operand1,
                  SymbolType operand2Type, void *operand2);
    void assign(Symbol *lvalue, SymbolType rvalueType, void *rvalue);
private:
    void multiply(std::shared_ptr<Symbol> resultVariable,
                  SymbolType operand1Type, void *operand1,
                  SymbolType operand2Type, void *operand2);
    void add(std::shared_ptr<Symbol> resultVariable,
                  SymbolType operand1Type, void *operand1,
                  SymbolType operand2Type, void *operand2);
};

} // namespace escript

#endif // ICODEEMITTER_H
