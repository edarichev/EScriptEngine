/**
 * @file processor.cpp
 * @brief Процессор (выполняет команды)
 */
#ifndef ESCRIPT_PROCESSOR_H
#define ESCRIPT_PROCESSOR_H

#include "EScript_global.h"
#include "opcode.h"
#include "assembler.h"
#include <stack>
#include "symbol.h"
#include "storage.h"
#include <cstring>

namespace escript {

/**
 * @brief Процессор (выполняет команды)
 */
class ESCRIPT_EXPORT Processor
{
    uint8_t *_memory = nullptr;
    // указатель, синхронный с PC
    uint8_t *_p = nullptr;
    size_t _memoryLimit = 0;
    // PC - Program Counter, счётчик команд, позиция, где находится команда,
    // которая должна выполнятся следующей
    size_t _pc = 0;
    // тип засунутого в стек элемента
    std::stack<SymbolType> _itemType;
    // стек для значений (собственно, сам стек)
    std::stack<uint64_t> _stack;
    Storage *_storage = nullptr;
public:
    /**
     * @brief "Присоединить" к памяти
     * @param memory указатель на память машины
     * @param limit сколько байт
     */
    void setMemory(uint8_t *memory, size_t limit, Storage *storage)
    {
        _memory = memory;
        _memoryLimit = limit;
        _storage = storage;
    }
    /**
     * @brief PC - Program Counter, счётчик команд, позиция, где находится
     *        команда, которая должна выполнятся следующей
     */
    size_t PC() const { return _pc; }
    void setPC(size_t newPc)
    {
        _pc = newPc;
    }
    /**
     * @brief Запуск выполнения с текущей позиции PC
     */
    void run()
    {
        _p = _memory + _pc;
        while (_pc < _memoryLimit) {
            OpCode opCode = (OpCode) *((Assembler::OpCodeType*)(_p));
            auto shift = Assembler::instructionSize(opCode);
            switch (opCode) {
            case OpCode::HALT:
                _pc += shift;
                return;
            case OpCode::NOP:
                break;
            case OpCode::ADDST:
                addst();
                break;
            case OpCode::MULST:
                mulst();
                break;
            case OpCode::LDC_INT64_DATA64:
                ldc_int64_data64();
                break;
            case OpCode::LDLOC_M:
                ldloc_m();
                break;
            case OpCode::STLOC_M:
                stloc_m();
                break;
            case OpCode::NEG:
                neg();
                break;
            default:
                std::cout << "Unsupported instruction: " << (int)opCode << std::endl;
                next();
            }
        }
    }
private:

    void next()
    {
        next(sizeof (Assembler::OpCodeType));
    }

    void next(size_t shift)
    {
        _pc += shift;
        _p += shift;
    }

    void neg()
    {
        next();
        auto item = popFromStack();
        if (item.first == SymbolType::Variable) {
            ObjectRecord *ptr = (ObjectRecord*)item.second;
            int64_t value = -(int64_t)ptr->data;
            pushToStack(SymbolType::Integer, value);
        }
    }

    void mulst()
    {
        next();
        auto item2 = popFromStack();
        auto item1 = popFromStack();
        if (item1.first == SymbolType::Integer && item2.first == SymbolType::Integer) {
            int64_t v1 = (int64_t)item1.second;
            int64_t v2 = (int64_t)item2.second;
            pushToStack(SymbolType::Integer, v1 * v2);
        } else if (item1.first == SymbolType::Integer && item2.first == SymbolType::Variable) {
            ObjectRecord *ptr = (ObjectRecord*)item2.second;
            // здесь проверка, как сложить два разных типа
            if (ptr->type == SymbolType::Integer) {
                int64_t value = (int64_t)ptr->data;
                value *= (int64_t)item1.second;
                pushToStack(SymbolType::Integer, value);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Integer) {
            ObjectRecord *ptr = (ObjectRecord*)item1.second;
            if (ptr->type == SymbolType::Integer) {
                int64_t value = (int64_t)ptr->data;
                value *= (int64_t)item2.second;
                pushToStack(SymbolType::Integer, value);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Variable) {
            ObjectRecord *ptr1 = (ObjectRecord*)item1.second;
            ObjectRecord *ptr2 = (ObjectRecord*)item2.second;
            if (ptr1->type == SymbolType::Integer && ptr2->type == SymbolType::Integer) {
                int64_t result = (int64_t)ptr1->data * (int64_t)ptr2->data;
                pushToStack(SymbolType::Integer, result);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        }
    }

    void addst()
    {
        next();
        auto item2 = popFromStack();
        auto item1 = popFromStack();
        if (item1.first == SymbolType::Integer && item2.first == SymbolType::Integer) {
            int64_t v1 = (int64_t)item1.second;
            int64_t v2 = (int64_t)item2.second;
            pushToStack(SymbolType::Integer, v1 + v2);
        } else if (item1.first == SymbolType::Integer && item2.first == SymbolType::Variable) {
            ObjectRecord *ptr = (ObjectRecord*)item2.second;
            // здесь проверка, как сложить два разных типа
            if (ptr->type == SymbolType::Integer) {
                int64_t value = (int64_t)ptr->data;
                value += (int64_t)item1.second;
                pushToStack(SymbolType::Integer, value);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Integer) {
            ObjectRecord *ptr = (ObjectRecord*)item1.second;
            if (ptr->type == SymbolType::Integer) {
                int64_t value = (int64_t)ptr->data;
                value += (int64_t)item2.second;
                pushToStack(SymbolType::Integer, value);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        } else if (item1.first == SymbolType::Variable && item2.first == SymbolType::Variable) {
            ObjectRecord *ptr1 = (ObjectRecord*)item1.second;
            ObjectRecord *ptr2 = (ObjectRecord*)item2.second;
            if (ptr1->type == SymbolType::Integer && ptr2->type == SymbolType::Integer) {
                int64_t result = (int64_t)ptr1->data + (int64_t)ptr2->data;
                pushToStack(SymbolType::Integer, result);
            } else {
                // здесь проверка, как сложить два разных типа
                pushToStack(SymbolType::Integer, 0); // пока 0
            }
        }
    }

    void ldloc_m()
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

    ObjectRecord *readRecord(Symbol /*out*/ *&symbol)
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

    void ldc_int64_data64()
    {
        next();
        _itemType.push(SymbolType::Integer);
        _stack.push(*(uint64_t*)_p);
        next(sizeof (uint64_t));
    }
    // несколько типизированных вариантов
    void setValue(ObjectRecord *ptr, int64_t value)
    {
        // TODO: обработать смену типа
        ptr->type = SymbolType::Integer;
        memcpy(&ptr->data, &value, sizeof(value));
    }

    int64_t getIntValue(ObjectRecord *ptr)
    {
        int64_t v;
        memcpy(&v, &ptr->data, sizeof (int64_t));
        return v;
    }

    ObjectRecord *installRecord(Symbol *symbol)
    {
        return _storage->installRecord(symbol);
    }

    std::pair<SymbolType, uint64_t> popFromStack()
    {
        auto type = _itemType.top();
        _itemType.pop();
        uint64_t item = _stack.top();
        _stack.pop();
        return std::make_pair(type, item);
    }

    void pushToStack(SymbolType type, uint64_t value)
    {
        _itemType.push(type);
        _stack.push(value);
    }

    void stloc_m()
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
};


} // namespace escript

#endif // ESCRIPT_PROCESSOR_H
