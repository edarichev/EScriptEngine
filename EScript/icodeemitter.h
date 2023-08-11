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

class Block;

/**
 * @brief Генератор промежуточного кода
 * @details (I == intermediate), не интерфейс.
 */
class ESCRIPT_EXPORT ICodeEmitter
{
private:
    std::vector<TCode> *_mainBuffer;
    std::vector<TCode> *_buffer;
    std::vector<TCode> _tmpBuffer;
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
    void fnStart(std::shared_ptr<Symbol> &func);
    void fnCode(std::shared_ptr<Symbol> &func);
    void fnLoadArgs();
    void fnEnd();
    void fnArg(std::shared_ptr<Symbol> &argument);
    void ret();
    void emptyReturn(std::shared_ptr<Symbol> &func);
    void call(std::shared_ptr<Symbol> &func, int nArgs);
    void startBlock(std::shared_ptr<Block> &block);
    void endBlock(std::shared_ptr<Block> &block);
    void push(std::pair<SymbolType, OperandRecord> &value);
    void switchToTempBuffer();
    void switchToMainBuffer();
    void writeTempBuffer();
};

} // namespace escript

#endif // ICODEEMITTER_H
