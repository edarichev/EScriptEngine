/**
 * @file icodeemitter.h
 * @brief Генератор промежуточного кода
 */
#ifndef ICODEEMITTER_H
#define ICODEEMITTER_H

#include "EScript_global.h"
#include "symbol.h"
#include "tcode.h"
#include <vector>

namespace escript {

/**
 * @brief Генератор промежуточного кода
 * @details (I == intermediate), не интерфейс.
 */
class ESCRIPT_EXPORT ICodeEmitter
{
private:
    std::vector<TCode> &_buffer;
public:
    ICodeEmitter(std::vector<TCode> &buffer);
    void binaryOp(OperationType operationType,
                  Symbol *resultVariable,
                  SymbolType operand1Type, const OperandRecord &operand1,
                  SymbolType operand2Type, const OperandRecord &operand2);
    void assign(Symbol *lvalue, SymbolType rvalueType,
                const OperandRecord &operand1);
    void unaryOp(OperationType operationType,
                 Symbol *resultVariable,
                 SymbolType operand1Type, const OperandRecord &operand1);
    void iffalse(Symbol *variableToTest, int exitOrFalseLabelId);
    void goToLabel(int labelId);
    void label(int labelId);
};

} // namespace escript

#endif // ICODEEMITTER_H
