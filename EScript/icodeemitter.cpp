/**
 * @file icodeemitter.cpp
 * @brief Генератор промежуточного кода - реализация
 */
#include "stdafx.h"
#include "icodeemitter.h"

using std::cout;
using std::endl;

namespace escript {

ICodeEmitter::ICodeEmitter(std::vector<TCode> &buffer)
    : _mainBuffer(&buffer), _buffer(&buffer)
{

}

void ICodeEmitter::binaryOp(OperationType operationType,
                            Symbol *resultVariable,
                            const Operand &operand1,
                            const Operand &operand2)
{
    switch (operationType) {
    case OperationType::Multiply:
    case OperationType::Add:
    case OperationType::Div:
    case OperationType::Minus:
    case OperationType::Less:
    case OperationType::LessOrEqual:
    case OperationType::Greater:
    case OperationType::GreaterOrEqual:
    case OperationType::Equal:
        break;
    default:
        throw std::domain_error("Invalid binary operation");
    }
    TCode code;
    code.lvalue = resultVariable;
    code.operand1 = operand1;
    code.operand2 = operand2;
    code.operation = operationType;
    _buffer->push_back(code);
}

void ICodeEmitter::assign(Symbol *lvalue,
                          const Operand &operand1)
{
    TCode code;
    code.lvalue = lvalue;
    code.operation = OperationType::Assign;
    code.operand1 = operand1;
    _buffer->push_back(code);
}

void ICodeEmitter::unaryOp(OperationType operationType, Symbol *resultVariable, const Operand &operand1)
{
    switch (operationType) {
    case OperationType::UMinus:
        break;
    default:
        throw std::domain_error("Invalid unary operation");
    }
    TCode code;
    code.lvalue = resultVariable;
    code.operand1 = operand1;
    code.operation = operationType;
    _buffer->push_back(code);
}

void ICodeEmitter::iffalse(Symbol *variableToTest, int exitOrFalseLabelId)
{
    TCode code;
    code.operation = OperationType::IfFalse;
    code.operand1.type = SymbolType::Variable;
    code.operand1.variable = variableToTest;
    // номер метки:
    code.operand2.type = SymbolType::Integer;
    code.operand2.intValue = exitOrFalseLabelId;
    _buffer->push_back(code);
}

void ICodeEmitter::goToLabel(int labelId)
{
    TCode code;
    code.operation = OperationType::Goto;
    code.operand1.type = SymbolType::Integer;
    code.operand1.intValue = labelId;
    _buffer->push_back(code);
}

void ICodeEmitter::label(int labelId)
{
    TCode code;
    code.operation = OperationType::Label;
    code.operand1.type = SymbolType::Integer;
    code.operand1.intValue = labelId;
    _buffer->push_back(code);
}

void ICodeEmitter::fnStart(std::shared_ptr<Symbol> &func)
{
    TCode code;
    code.operation = OperationType::FunctionStart;
    code.operand1.type = SymbolType::Function;
    code.operand1.function = func.get();
    _buffer->push_back(code);
}

void ICodeEmitter::fnCode(std::shared_ptr<Symbol> &func)
{
    TCode code;
    code.operation = OperationType::FunctionCode;
    code.operand1.type = SymbolType::Function;
    code.operand1.function = func.get();
    _buffer->push_back(code);
}

void ICodeEmitter::fnLoadArgs()
{
    TCode code;
    code.operation = OperationType::LoadArguments;
    code.operand1.type = SymbolType::Undefined;
    _buffer->push_back(code);
}

void ICodeEmitter::fnEnd()
{
    TCode code;
    code.operation = OperationType::FunctionEnd;
    code.operand1.type = SymbolType::Undefined;
    _buffer->push_back(code);
}

void ICodeEmitter::fnArg(std::shared_ptr<Symbol> &argument)
{
    TCode code;
    code.operation = OperationType::FunctionArgument;
    code.operand1.type = SymbolType::Variable;
    code.operand1.variable = argument.get();
    _buffer->push_back(code);
}

void ICodeEmitter::fnReturn(std::shared_ptr<Symbol> &func)
{
    TCode code;
    code.operation = OperationType::Ret;
    code.operand1.type = SymbolType::Function;
    code.operand1.function = func.get();
    code.operand2.type = SymbolType::Integer;
    code.operand2.intValue = 1; // 1 возвращаемых значений в стеке
    _buffer->push_back(code);
}

void ICodeEmitter::fnEmptyReturn(std::shared_ptr<Symbol> &func)
{
    TCode code;
    code.operation = OperationType::Ret;
    code.operand1.type = SymbolType::Function;
    code.operand1.function = func.get();
    code.operand2.type = SymbolType::Integer;
    code.operand2.intValue = 0; // 0 возвращаемых значений в стеке

    _buffer->push_back(code);
}

void ICodeEmitter::call(std::shared_ptr<Symbol> &func, int nArgs,
                        std::shared_ptr<Symbol> &resultVariable)
{
    // TODO: нужна вторая версия для вызова из функции
    // для сохранения записи активации.
    TCode codeArgs;
    codeArgs.operation = OperationType::Push;
    codeArgs.operand1.type = SymbolType::Integer;
    codeArgs.operand1.intValue = nArgs;
    _buffer->push_back(codeArgs);

    TCode codeFunc;
    codeFunc.operation = OperationType::Call;
    codeFunc.operand1.type = SymbolType::Function;
    codeFunc.operand1.function = func.get();
    _buffer->push_back(codeFunc);
    // извлечение результата:
    pop(resultVariable);
}

void ICodeEmitter::startBlock(std::shared_ptr<Block> &block)
{
    TCode code;
    code.operation = OperationType::BlockStart;
    code.operand1.type = SymbolType::Block;
    code.operand1.block = block.get();
    _buffer->push_back(code);
}

void ICodeEmitter::endBlock(std::shared_ptr<Block> &block)
{
    TCode code;
    code.operation = OperationType::BlockEnd;
    code.operand1.type = SymbolType::Block;
    code.operand1.block = block.get();
    _buffer->push_back(code);
}

void ICodeEmitter::push(const Operand &value)
{
    TCode code;
    code.operation = OperationType::Push;
    code.operand1 = value;
    _buffer->push_back(code);
}

void ICodeEmitter::push(int64_t intValue)
{
    TCode code;
    code.operation = OperationType::Push;
    code.operand1.type = SymbolType::Integer;
    code.operand1.intValue = intValue;
    _buffer->push_back(code);
}

void ICodeEmitter::pop(std::shared_ptr<Symbol> &resultVariable)
{
    TCode code;
    code.operation = OperationType::Pop;
    code.lvalue = resultVariable.get();
    _buffer->push_back(code);
}

void ICodeEmitter::switchToTempBuffer()
{
    _buffer = &_tmpBuffer;
}

void ICodeEmitter::switchToMainBuffer()
{
    _buffer = _mainBuffer;
}

void ICodeEmitter::writeTempBuffer()
{
    _buffer->insert(_buffer->end(), _tmpBuffer.begin(), _tmpBuffer.end());
    _tmpBuffer.clear();
}

} // namespace escript

