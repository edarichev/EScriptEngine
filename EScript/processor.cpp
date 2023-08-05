/**
 * @file processor.cpp
 * @brief Процессор (выполняет команды)
 */
#include "stdafx.h"
#include "processor.h"

namespace escript {

void Processor::neg()
{
    next();
    auto item = popFromStack();
    if (item.first == SymbolType::Variable) {
        ObjectRecord *ptr = (ObjectRecord*)item.second;
        int64_t value = -(int64_t)ptr->data;
        pushToStack(SymbolType::Integer, value);
    }
}

/**
 * @brief Структура для упаковки тип+значение (как variant)
 */
struct PValue
{
    SymbolType type;
    union {
        int64_t intValue;
        double realValue;
    };
    PValue operator=(int64_t rhs)
    {
        type = SymbolType::Integer;
        intValue = rhs;
        return *this;
    }
    PValue operator=(double rhs)
    {
        type = SymbolType::Real;
        realValue = rhs;
        return *this;
    }
    /**
     * @brief Возвращает значение как uint64_t
     * @return
     */
    uint64_t value() const
    {
        switch (type) {
        case SymbolType::Integer:
            return bit_cast<uint64_t>(intValue);
        case SymbolType::Real:
            return bit_cast<uint64_t>(realValue);
        default:
            throw std::domain_error("unsupproted type by PValue");
        }
    }
};

PValue getValue(ObjectRecord *ptr)
{
    PValue val;
    switch (ptr->type) {
    case SymbolType::Integer:
        val = bit_cast<int64_t>(ptr->data);
        break;
    case SymbolType::Real:
        val = bit_cast<double>(ptr->data);
        break;
    default:
        throw std::domain_error("Unsupported type: getValue");
    }
    return val;
}

PValue getValue(const std::pair<SymbolType, uint64_t> &item)
{
    PValue val;
    switch (item.first) {
    // тип переменной
    case SymbolType::Integer:
        val = bit_cast<int64_t>(item.second);
        break;
    case SymbolType::Real:
        val = bit_cast<double>(item.second);
        break;
    case SymbolType::Variable:
        return getValue((ObjectRecord*)item.second);
    default:
        throw std::domain_error("Unsupported type: getValue(pair)");
    }
    return val;
}

// для строк и неприводимых типов - другая функция
template<typename T1, typename T2>
decltype(auto) opValues(T1 v1, T2 v2, OpCode opCode)
{
    switch (opCode) {
    case OpCode::ADDST:
        return v1 + v2;
    case OpCode::MULST:
        return v1 * v2;
    case OpCode::DIVST:
        return v1 / v2;
    case OpCode::SUBST:
        return v1 - v2;
    default:
        throw std::domain_error("Unsupprted bin.op");
    }
}

void Processor::binaryStackOp(OpCode opCode)
{
    next();
    auto item2 = popFromStack();
    auto item1 = popFromStack();
    PValue value1 = getValue(item1);
    PValue value2 = getValue(item2);
    PValue result;
    switch (value1.type) {
    case SymbolType::Integer:
        switch (value2.type) {
        case SymbolType::Integer:
            result = opValues(value1.intValue, value2.intValue, opCode);
            break;
        case SymbolType::Real:
            result = opValues(value1.intValue, value2.realValue, opCode);
            break;
        default:
            throw std::domain_error("Unsupported type: binaryStackOp");
        }
        break;
    case SymbolType::Real:
        switch (value2.type) {
        case SymbolType::Integer:
            result = opValues(value1.realValue, value2.intValue, opCode);
            break;
        case SymbolType::Real:
            result = opValues(value1.realValue, value2.realValue, opCode);
            break;
        default:
            throw std::domain_error("Unsupported type: binaryStackOp");
        }
        break;
    default:
        throw std::domain_error("Unsupported type: binaryStackOp");
    }
    pushToStack(result.type, result.value());
}

void Processor::addst()
{
    binaryStackOp(OpCode::ADDST);
}

void Processor::mulst()
{
    binaryStackOp(OpCode::MULST);
}

void Processor::divst()
{
    binaryStackOp(OpCode::DIVST);
}

void Processor::subst()
{
    binaryStackOp(OpCode::SUBST);
}

void Processor::ldloc_m()
{
    // пропустить код команды
    next();
    Symbol *symbol = nullptr;
    ObjectRecord *ptr = readRecord(symbol);
    // добавим счётчик ссылок, т.к. теперь объект используется в стеке
    ptr->counter++;
    // тип объекта - тут всегда переменная, такова команда ldloc m
    _itemType.push(SymbolType::Variable);
    // значение/указатель, в зависимости от типа
    _stack.push((uint64_t)ptr);
    // сдвиг на следующую команду
    next(sizeof (uint64_t));
}

void Processor::ldc_double_data64()
{
    next();
    _itemType.push(SymbolType::Real);
    _stack.push(bit_cast<uint64_t>(*(double*)_p));
    next(sizeof (double));
}

ObjectRecord *Processor::readRecord(Symbol *&symbol)
{
    // Адрес объекта в таблице символов
    uint64_t symbolAddress = *(uint64_t*)_p;
    symbol = (Symbol*)symbolAddress;
    // адрес указателя на переменную в секции DATA
    uint64_t address = symbol->location();
    // указатель на запись в списке объектов
    // он всегда должен существовать
    uint64_t recValue = *(uint64_t*)(_memory + address);
    ObjectRecord *ptr = (ObjectRecord*)(recValue);
    return ptr;
}

void Processor::nop()
{
    next();
}

void Processor::ldc_uint64_data64()
{
    next();
    _itemType.push(SymbolType::Integer);
    _stack.push(*(uint64_t*)_p);
    next(sizeof (uint64_t));
}

void Processor::ldc_int64_data64()
{
    next();
    _itemType.push(SymbolType::Integer);
    _stack.push(*(uint64_t*)_p);
    next(sizeof (uint64_t));
}

void Processor::setValue(ObjectRecord *ptr, int64_t value)
{
    // TODO: обработать смену типа
    ptr->type = SymbolType::Integer;
    memcpy(&ptr->data, &value, sizeof(value));
}

void Processor::setValue(ObjectRecord *ptr, double value)
{
    // TODO: обработать смену типа
    ptr->type = SymbolType::Real;
    memcpy(&ptr->data, &value, sizeof(value));
}

int64_t Processor::getIntValue(ObjectRecord *ptr)
{ // можно bit_cast, но оставим как второй вариант, без лишнего вызова.
    int64_t v;
    memcpy(&v, &ptr->data, sizeof (int64_t));
    return v;
}

double Processor::getRealValue(ObjectRecord *ptr)
{
    return bit_cast<double>(ptr->data);
}

ObjectRecord *Processor::installRecord(Symbol *symbol)
{
    return _storage->installRecord(symbol);
}

std::pair<SymbolType, uint64_t> Processor::popFromStack()
{
    auto type = _itemType.top();
    _itemType.pop();
    uint64_t item = _stack.top();
    _stack.pop();
    return std::make_pair(type, item);
}

void Processor::pushToStack(SymbolType type, uint64_t value)
{
    _itemType.push(type);
    _stack.push(value);
}

void Processor::stloc_m()
{
    next();
    // что в стеке?
    auto item = popFromStack();
    ObjectRecord *ptrLValue = nullptr;
    ObjectRecord *ptrRValue = nullptr;
    Symbol *symbol = nullptr;
    int64_t iValue = 0;
    double dValue = 0;
    ptrLValue = readRecord(symbol);
    if (!ptrLValue) { // это lvalue, его нужно установить, если его ещё нет
        ptrLValue = installRecord(symbol);
        // имеем адрес ObjectRecord
        // этот адрес нужно записать в секции DATA:
        uint8_t *addr = _memory + symbol->location();
        uint64_t ptrAsAddr = (uint64_t)ptrLValue;
        memcpy(addr, &ptrAsAddr, sizeof(uint64_t));
    }
    switch (item.first) {
    case SymbolType::Integer:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, (int64_t)item.second);
        break;
    case SymbolType::Real:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, bit_cast<double>(item.second));
        break;
    case SymbolType::Variable:
        // здесь находится указатель на запись в таблице объектов
        // в зависимости от типа, мы либо меняем ссылку, либо присваиваем по значению
        // пока целые - меняем по значению
        ptrRValue = (ObjectRecord*)item.second;
        switch (ptrRValue->type) {
        case SymbolType::Integer:
            iValue = bit_cast<int64_t>(ptrRValue->data);
            setValue(ptrLValue, iValue);
            break;
        case SymbolType::Real:
            dValue = bit_cast<double>(ptrRValue->data);
            setValue(ptrLValue, dValue);
            break;
        default:
            throw std::domain_error("Unsupported type of variable :" + std::to_string((uint8_t)item.first));
        }
        break;
    default:
        throw std::domain_error("Unsupported type :" + std::to_string((uint8_t)item.first));
    }
    next(sizeof (uint64_t));//размер операнда-указателя
}






} // namespace escript
