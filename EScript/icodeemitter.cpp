/**
 * @file icodeemitter.cpp
 * @brief Генератор промежуточного кода - реализация
 */
#include "stdafx.h"
#include "icodeemitter.h"

using std::cout;
using std::endl;

namespace escript {

void ICodeEmitter::multiply(std::shared_ptr<Symbol> resultVariable,
                            SymbolType operand1Type, void *operand1,
                            SymbolType operand2Type, void *operand2)
{
    cout << resultVariable->utf8Name() << " := ";
    // приведение типов:
    // int+int->int
    // real+real->real
    // int+real->real
    // string+любой->string
    if (operand1Type == SymbolType::Integer) {
        cout << *(IntType*)operand1;
    } else if (operand1Type == SymbolType::Variable) {
        cout << ((Symbol*)operand1)->utf8Name();
    }
    cout << " * ";
    if (operand2Type == SymbolType::Integer) {
        cout << *(IntType*)operand2;
    } else if (operand2Type == SymbolType::Variable) {
        cout << ((Symbol*)operand2)->utf8Name();
    }
    cout << endl;
}
void ICodeEmitter::add(std::shared_ptr<Symbol> resultVariable,
                            SymbolType operand1Type, void *operand1,
                            SymbolType operand2Type, void *operand2)
{
    cout << resultVariable->utf8Name() << " := ";
    if (operand1Type == SymbolType::Integer) {
        cout << *(IntType*)operand1;
    } else if (operand1Type == SymbolType::Variable) {
        cout << ((Symbol*)operand1)->utf8Name();
    }
    cout << " + ";
    if (operand2Type == SymbolType::Integer) {
        cout << *(IntType*)operand2;
    } else if (operand2Type == SymbolType::Variable) {
        cout << ((Symbol*)operand2)->utf8Name();
    }
    cout << endl;
}

void ICodeEmitter::binaryOp(OperationType operationType,
                            std::shared_ptr<Symbol> resultVariable,
                            SymbolType operand1Type, void *operand1,
                            SymbolType operand2Type, void *operand2)
{
    switch (operationType) {
    case OperationType::Multiply:
        multiply(resultVariable, operand1Type, operand1, operand2Type, operand2);
        break;
    case OperationType::Add:
        add(resultVariable, operand1Type, operand1, operand2Type, operand2);
        break;
    }
}

void ICodeEmitter::assign(Symbol *lvalue, SymbolType rvalueType, void *rvalue)
{
    cout << lvalue->utf8Name() << " := ";
    switch (rvalueType) {
    case SymbolType::Integer:
        cout << *(IntType*)rvalue;
        break;
    case SymbolType::Variable:
        cout << ((Symbol*)rvalue)->utf8Name();
        break;
    }
    cout << endl;
}

} // namespace escript

