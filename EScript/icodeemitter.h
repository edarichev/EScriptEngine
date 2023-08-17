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
    std::stack<std::vector<TCode>> _stackOfTmpBuffers;
public:
    ICodeEmitter(std::vector<TCode> &buffer);
    void binaryOp(OperationType operationType,
                  Symbol *resultVariable,
                  const Operand &operand1,
                  const Operand &operand2);
    void assign(Symbol *lvalue, const Operand &operand1);
    void unaryOp(OperationType operationType,
                 Symbol *resultVariable,
                 const Operand &operand1);
    void iffalse(Symbol *variableToTest, int exitOrFalseLabelId);
    void goToLabel(int labelId);
    void label(int labelId);
    void fnStart(std::shared_ptr<Symbol> &func);
    void fnCode(std::shared_ptr<Symbol> &func);
    void fnLoadArgs();
    void fnEnd();
    void fnArg(std::shared_ptr<Symbol> &argument);
    void fnReturn(std::shared_ptr<Symbol> &func);
    void fnEmptyReturn(std::shared_ptr<Symbol> &func);
    void call(std::shared_ptr<Symbol> &func, int nArgs, std::shared_ptr<Symbol> &resultVariable);
    void startBlock(std::shared_ptr<Block> &block);
    void endBlock(std::shared_ptr<Block> &block);
    void push(const Operand &value);
    void push(int64_t intValue);
    void pushVariable(Symbol *v);
    void pushString(StringObject *s);
    void pop(std::shared_ptr<Symbol> &resultVariable);
    void pop();
    void switchToTempBuffer();
    void switchToMainBuffer();
    void writeTempBuffer();
    /**
     * @brief Вызов метода AutomationObject
     * @param leftVariable переменная у которой вызывается метод
     * @param propName название метода или свойства (начинается с get_)
     * @param resultVariable (временная) переменная, куда пометсить результат
     * @param nArgs число аргементов (0 для свойства)
     */
    void callAOMethod(std::shared_ptr<Symbol> &leftVariable, StringObject *propName, std::shared_ptr<Symbol> &resultVariable, int nArgs);
    void allocArray(std::shared_ptr<Symbol> &arrVariable);
    /**
     * @brief извлечь из стека, увеличить, засунуть обратно
     */
    void increment();
    void decrement();
};

} // namespace escript

#endif // ICODEEMITTER_H
