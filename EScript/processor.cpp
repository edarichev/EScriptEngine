/**
 * @file processor.cpp
 * @brief Процессор (выполняет команды)
 */
#include "stdafx.h"
#include "processor.h"
#include "pvalue.h"

namespace escript {

// для конвертации операций
std::map<OpCode, ArithmeticOperation> Processor::optypes;

Processor::Processor()
{
    if (optypes.empty()) {
        optypes = {
            {OpCode::ADDST, ArithmeticOperation::Add },
            {OpCode::SUBST, ArithmeticOperation::Sub },
            {OpCode::MULST, ArithmeticOperation::Mul },
            {OpCode::DIVST, ArithmeticOperation::Div },
            {OpCode::STLESS, ArithmeticOperation::BoolLess },
            {OpCode::STLESS_EQ, ArithmeticOperation::BoolLessOrEqual },
            {OpCode::STGT, ArithmeticOperation::BoolGreater },
            {OpCode::STGT_EQ, ArithmeticOperation::BoolGreaterOrEqual },
            {OpCode::STEQ, ArithmeticOperation::BoolEqual },
        };
    }
}

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

void Processor::jmp_m()
{
    next();
    uint64_t jumpTo = *(uint64_t*)_p;
    setPC(jumpTo);
    // не считывать - здесь уже новое значение PC
}

void Processor::ldc_bool_data8()
{
    next();
    PValue result((bool)((*_p) ? true : false));
    pushToStack(result.type, result.value64());
    next(sizeof (uint8_t));
}

void Processor::iffalse_m()
{
    next();
    // в стеке что-то есть
    auto item = popFromStack();
    PValue value = PValue::getValue(item);
    if (!value.asBoolean()) {
        uint64_t jumpTo = *(uint64_t*)_p;
        setPC(jumpTo);
        // сразу выйти - здесь уже новое значение PC
        return;
    }
    next(sizeof (uint64_t));
}

void Processor::stless()
{
    binaryStackOp(OpCode::STLESS);
}

void Processor::stless_eq()
{
    binaryStackOp(OpCode::STLESS_EQ);
}

void Processor::stgt()
{
    binaryStackOp(OpCode::STGT);
}

void Processor::stgt_eq()
{
    binaryStackOp(OpCode::STGT_EQ);
}

void Processor::steq()
{
    binaryStackOp(OpCode::STEQ);
}

void Processor::ldargs()
{
    // это первая команда в функции, переменные расположена сразу перед ней.
    // двигаемся по 8 байт назад, пока == 0 (там нули везде)
    PtrIntType *ptr = (PtrIntType*)_p;
    ptr --;
    // последним записано число всех переменных, оно не менее 1, т.к. параметры
    // тоже являются локальными переменными
    int64_t numOfVariables = *ptr;
    ptr --;
    // предпоследним - число параметров в функции
    int64_t numOfParameters = *ptr;
    PtrIntType *pend = ptr;
    while (numOfVariables > 0) {
        ptr--;
        numOfVariables--;
    }
    // по идее, эту область теперь надо занулить, т.к. при каждом вызове
    // будет всё с чистого листа
    memset(ptr, 0, (pend - ptr) * sizeof (PtrIntType));
    // теперь мы на переменной arguments, оставим её пока
    ptr++;
    numOfParameters--;
    // теперь в каждую запись нужно вставить либо указатель на ObjectRecord,
    // если это объект, либо сдлеать "по значению": создать ещё одну
    // запись в ObjectRecord

    // здесь первым идёт число аргументов
    auto item = popFromStack();
    assert(item.first == SymbolType::Integer);
    int nArgs = item.second;
    while (nArgs > 0) {
        auto arg = popFromStack();
        // если тут Variable, то в стеке - ObjectRecord, и сюда заносим
        // это значение
        // иначе - просто число/float/bool
        ObjectRecord *rec = nullptr;
        auto argumentType = arg.first;
        switch (argumentType) {
        case SymbolType::Integer:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Integer;
            rec->data = arg.second;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Real:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Real;
            rec->data = arg.second;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Boolean:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Boolean;
            rec->data = arg.second;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Variable: {
            auto refRec = (ObjectRecord*)arg.second;
            // для простых типов делаем прстое копирование
            switch (refRec->type) {
            case SymbolType::Integer:
            case SymbolType::Boolean:
            case SymbolType::Real:
                rec = _storage->installRecord(nullptr);
                rec->type = refRec->type;
                rec->data = refRec->data;
                *ptr = bit_cast<uint64_t>(rec);
                break;
            default:
                throw std::domain_error("Not supported (parameter type)");
            }
            break;
        }
        default:
            throw std::domain_error("type not impl.");
        }
        nArgs--;
        ptr++;
    }
    next();
}

void Processor::ret()
{
    next();
    setPC(_pcStack.top());
    _pcStack.pop();
}

void Processor::call()
{
    next();
    PtrIntType addr = *(PtrIntType*)_p;
    next(sizeof (uint64_t)); // сначала сдвинуть
    _pcStack.push(_pc);      // теперь сохранить адрес возврата
    setPC(addr);             // перейти к функции
}

void Processor::binaryStackOp(OpCode opCode)
{
    next();
    auto item2 = popFromStack();
    auto item1 = popFromStack();
    PValue value1 = PValue::getValue(item1);
    PValue value2 = PValue::getValue(item2);
    // это должно быть гарантировано, если это не так, пусть вылетит
    ArithmeticOperation op = optypes.find(opCode)->second;
    PValue result = PValue::binaryOpValues(value1, value2, op);
    pushToStack(result.type, result.value64());
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

void Processor::setValue(ObjectRecord *ptr, bool value)
{
    ptr->type = SymbolType::Boolean;
    ptr->data = value ? 1 : 0;
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
    bool bValue = false;
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
    case SymbolType::Boolean:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, item.second ? true : false);
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
        case SymbolType::Boolean:
            bValue = ptrRValue->data ? true : false;
            setValue(ptrLValue, bValue);
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
