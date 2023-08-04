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

void Processor::binaryStackOp(OpCode opCode)
{
    next();
    BinaryIntFnPtr fn = nullptr;
    switch (opCode) {
    case OpCode::SUBST:
        fn = _sub;
        break;
    case OpCode::MULST:
        fn = _mul;
        break;
    case OpCode::ADDST:
        fn = _add;
        break;
    case OpCode::DIVST:
        fn = _div;
        break;
    default:
        throw std::domain_error("Unsupported binary operation");
    }
    auto item2 = popFromStack();
    auto item1 = popFromStack();
    int64_t v1{}, v2{};
    if (item1.first == SymbolType::Integer && item2.first == SymbolType::Integer) {
        v1 = (int64_t)item1.second;
        v2 = (int64_t)item2.second;
    } else if (item1.first == SymbolType::Integer && item2.first == SymbolType::Variable) {
        ObjectRecord *ptr = (ObjectRecord*)item2.second;
        if (ptr->type == SymbolType::Integer) {
            v1 = (int64_t)item1.second;
            v2 = (int64_t)ptr->data;
        } else {
            // здесь проверка, как сложить два разных типа
            pushToStack(SymbolType::Integer, 0); // пока 0
            return;
        }
    } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Integer) {
        ObjectRecord *ptr = (ObjectRecord*)item1.second;
        if (ptr->type == SymbolType::Integer) {
            v1 = (int64_t)ptr->data;
            v2 = (int64_t)item2.second;
        } else {
            // здесь проверка, как сложить два разных типа
            pushToStack(SymbolType::Integer, 0); // пока 0
            return;
        }
    } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Variable) {
        ObjectRecord *ptr1 = (ObjectRecord*)item1.second;
        ObjectRecord *ptr2 = (ObjectRecord*)item2.second;
        if (ptr1->type == SymbolType::Integer && ptr2->type == SymbolType::Integer) {
            v1 = (int64_t)ptr1->data;
            v2 = (int64_t)ptr2->data;
        } else {
            // здесь проверка, как сложить два разных типа
            pushToStack(SymbolType::Integer, 0); // пока 0
            return;
        }
    }
    pushToStack(SymbolType::Integer, fn(v1, v2));
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

int64_t Processor::getIntValue(ObjectRecord *ptr)
{
    int64_t v;
    memcpy(&v, &ptr->data, sizeof (int64_t));
    return v;
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
    ObjectRecord *ptr = nullptr;
    Symbol *symbol = nullptr;
    int64_t iValue = 0;
    ptr = readRecord(symbol);
    if (!ptr) { // это lvalue, его нужно установить, если его ещё нет
        ptr = installRecord(symbol);
        // имеем адрес ObjectRecord
        // этот адрес нужно записать в секции DATA:
        uint8_t *addr = _memory + symbol->location();
        uint64_t ptrAsAddr = (uint64_t)ptr;
        memcpy(addr, &ptrAsAddr, sizeof(uint64_t));
    }
    switch (item.first) {
    case SymbolType::Integer:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptr, (int64_t)item.second);
        break;
    case SymbolType::Variable:
        // здесь находится указатель на запись в таблице объектов
        // в зависимости от типа, мы либо меняем ссылку, либо присваиваем по значению
        // пока целые - меняем по значению
        iValue = (int64_t)((ObjectRecord*)item.second)->data;
        setValue(ptr, iValue);
        break;
    default:
        std::cout << "Unsupported type :" << (uint8_t)item.first << std::endl;
    }
    next(sizeof (uint64_t));
}






} // namespace escript
