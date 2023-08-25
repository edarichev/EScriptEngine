/**
 * @file processor.cpp
 * @brief Процессор (выполняет команды)
 */
#include "stdafx.h"
#include "processor.h"
#include "pvalue.h"
#include "stringobject.h"
#include "function.h"
#include "numberobject.h"

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
            {OpCode::LSH, ArithmeticOperation::LShift },
            {OpCode::RSH, ArithmeticOperation::RShift },
            {OpCode::RSHZ, ArithmeticOperation::RShiftZero },
            {OpCode::BIT_AND, ArithmeticOperation::BITAND },
            {OpCode::BIT_OR, ArithmeticOperation::BITOR },
            {OpCode::BIT_XOR, ArithmeticOperation::BITXOR },
            {OpCode::LOG_AND, ArithmeticOperation::LOGAND },
            {OpCode::LOG_OR, ArithmeticOperation::LOGOR },
            {OpCode::MODST, ArithmeticOperation::Mod },
        };
    }
}

std::stack<StackValue> Processor::loadArguments()
{
    int64_t nArgs = popFromStack().getIntValue();
    std::stack<StackValue> args;
    while (nArgs-- > 0)
        args.push(popFromStack());
    return args;
}

void Processor::neg()
{
    next();
    auto item = popFromStack();
    if (item.type == SymbolType::Variable) {
        ObjectRecord *ptr = (ObjectRecord*)item.value;
        int64_t value = -(int64_t)ptr->data;
        pushToStack(SymbolType::Integer, value);
    }
}

void Processor::jmp_m()
{
    next();
    uint64_t jumpTo = *(uint64_t*)_p;
    setPC(jumpTo);
    // не считывать, т.к. здесь уже новое значение PC
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
    // сдвинем в конце, т.к. для отсчёта нужен этот адрес
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
    //PtrIntType *pend = ptr;
    while (numOfVariables > 0) {
        ptr--;
        numOfVariables--;
    }
    // по идее, эту область теперь надо занулить, т.к. при каждом вызове
    // будет всё с чистого листа, но её обнулять нельзя:
    // 1. здесь могут быть указатели на переменные-функции
    // 2. в этом нет смысла.

    // теперь мы на переменной arguments, пропустим её пока
    // потом можно заполнить её значениями
    ptr++;
    numOfParameters--;
    // теперь в каждую запись нужно вставить либо указатель на ObjectRecord,
    // если это объект, либо сдлеать "по значению": создать ещё одну
    // запись в ObjectRecord

    // здесь первым идёт число аргументов
    auto item = popFromStack();
    assert(item.type == SymbolType::Integer);
    int nArgs = item.value;
    // записываем в обратном порядке
    // (снизу вверх, т.к. переданы они тоже в обратном порядке)
    ptr += nArgs - 1;
    while (nArgs > 0) {
        auto arg = popFromStack();
        // если тут Variable, то в стеке - ObjectRecord, и сюда заносим
        // это значение
        // иначе - просто число/float/bool
        ObjectRecord *rec = nullptr;
        auto argumentType = arg.type;
        switch (argumentType) {
        case SymbolType::Integer:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Integer;
            rec->data = arg.value;
            // записать адрес переменной в секции DATA:
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Real:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Real;
            rec->data = arg.value;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Boolean:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Boolean;
            rec->data = arg.value;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::String:
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::String;
            rec->data = arg.value;
            *ptr = bit_cast<uint64_t>(rec);
            break;
        case SymbolType::Variable: {
            auto refRec = (ObjectRecord*)arg.value;
            // для простых типов делаем простое копирование
            switch (refRec->type) {
            case SymbolType::Integer:
            case SymbolType::Boolean:
            case SymbolType::Real:
                rec = _storage->installRecord(nullptr);
                rec->type = refRec->type;
                rec->data = refRec->data;
                *ptr = bit_cast<uint64_t>(rec);
                break;
            case SymbolType::Array: {
                rec = _storage->findRecord(refRec->data);
                if (!rec) {
                    rec = _storage->installRecord(nullptr);
                    //rec->reference = true;
                    rec->type = refRec->type;
                    rec->data = refRec->data;
                }
                *ptr = bit_cast<uint64_t>(rec);
                ((AutomationObject*)rec->data)->addRef();
                break;
            }
            case SymbolType::String:
                rec = _storage->installRecord(nullptr);
                rec->type = refRec->type;
                rec->data = refRec->data;
                *ptr = bit_cast<uint64_t>(rec);
                break;
            case SymbolType::Function:
                rec = _storage->findRecord(refRec->data);
                if (!rec) {
                    rec = _storage->installRecord(nullptr);
                    //rec->reference = true;
                    rec->type = refRec->type;
                    rec->data = refRec->data;
                }
                *ptr = bit_cast<uint64_t>(rec);
                ((AutomationObject*)refRec->data)->addRef();
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
        ptr--;
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
    Symbol *symbol;
    auto rec = readRecord(symbol);
    assert(rec->type == SymbolType::Function);
    Function *fn = (Function*)rec->data;
    uint64_t addr = fn->callAddress();
    next(sizeof (uint64_t)); // сначала сдвинуть
    _pcStack.push(_pc);      // теперь сохранить адрес возврата
    setPC(addr);             // перейти к функции
}

void Processor::ldstring()
{
    next();
    _stack.emplace(SymbolType::String, bit_cast<uint64_t>(*(uint64_t*)_p));
    next(sizeof (uint64_t));
}

std::unique_ptr<Number> Processor::packNumber(const StackValue &v)
{
    switch (v.type) {
    case SymbolType::Integer:
        return std::make_unique<Number>(v.getIntValue());
    case SymbolType::Real:
        return std::make_unique<Number>((double)v.getRealValue());
    default:
        throw std::domain_error("Only simple types can be boxed.");
    }
}



void Processor::callm()
{
    next();
    // сверху - указатель на сам объект
    auto objItem = popFromStack();
    AutomationObject *obj = nullptr;
    ObjectRecord *rec = nullptr;
    std::unique_ptr<Number> packedNumber;
    bool box = false;
    StackValue valueToBox;

    switch (objItem.type) {
    case SymbolType::Variable:
        rec = (ObjectRecord*)objItem.value;
        switch (rec->type) {
        case SymbolType::String:
            obj = (StringObject*)rec->data;
            break;
        case SymbolType::Array:
            obj = (Array*)rec->data;
            break;
        case SymbolType::Function:
            obj = (Function*)rec->data;
            break;
        case SymbolType::Object:
            obj = (AutomationObject*)rec->data;
            break;
        case SymbolType::Integer:
        case SymbolType::Real:
            box = true;
            valueToBox.type = rec->type;
            valueToBox.value = rec->data;
            break;
        default:
            throw std::domain_error("Can not call method of unsupported type");
        }
        break;
    default:
        throw std::domain_error("Can not call method of unsupported type");
    }
    // название метода - это следующая переменная в стеке
    auto methodNameItem = popFromStack();
    StringObject *method = nullptr;
    std::u32string methodName;
    // проверка типа
    switch (methodNameItem.type) {
    case SymbolType::String:
        method = (StringObject*)methodNameItem.value; // он напрямую в таблице строк
        methodName = method->uString();
        break;
    default:
        throw std::domain_error("Expected a string at top of stack");
    }
    // если нужна упаковка, пакуем число и вызываем метод у временного объекта
    if (box) {
        packedNumber = packNumber(valueToBox);
        obj = packedNumber.get();
    }
    obj->call(methodName, this);
    // теперь в стеке возвращённое значение (одно или нет)
    // проверим, какой это тип и установим его в таблицу строк или объектов:
    auto result = popFromStack();
    switch (result.type) {
    case SymbolType::String: {
        auto str = _strings->add((StringObject*)result.value);
        result.value = (uint64_t)str; // могло измениться
        break;
    }
    case SymbolType::Array: {
        // Сначала установить сам массив.
        // Затем надо перебрать каждый элемент.
        // Если это строка - установить в таблицу строк
        auto rec = _storage->findRecord(result.value);
        if (!rec) {
            // создать новый массив
            rec = _storage->installRecord(nullptr);
            rec->type = SymbolType::Array;
            rec->data = result.value;
            Array *arr = (Array*)result.value;
            //arr->addRef();
            for (auto &c : *arr) {
                switch (c.type) {
                case SymbolType::String:
                    c.strValue = _strings->add(c.strValue);
                    break;
                case SymbolType::Array:
                    c.arrValue->addRef();
                    break;
                default:
                    break;
                }
            }
        }
        break;
    }
    default: // ничего не делать
        break;
    }
    // тип проверен, вернуть обратно
    pushToStack(result);
}

void Processor::allocarray()
{
    next();
    uint64_t symbolAddress = *(uint64_t*)_p;
    Symbol *symbol = (Symbol*)symbolAddress;
    // адрес указателя на переменную в секции DATA
    uint64_t address = symbol->location();
    // создаём новый массив
    ObjectRecord *rec = installRecord(symbol);
    rec->type = SymbolType::Array;
    Array *arr = new Array();
    arr->addRef();
    rec->data = (PtrIntType)arr;
    *(uint64_t*)(_memory + address) = (uint64_t)rec;

    next(sizeof (uint64_t));
}

void Processor::inc()
{
    next();
    auto item = popFromStack();

    switch (item.type) {
    case SymbolType::Integer:
        item.value = bit_cast<int64_t>(item.value) + 1;
        break;
    case SymbolType::Real:
        item.value = bit_cast<uint64_t>(bit_cast<double>(item.value) + 1);
        break;
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)item.value;
        switch (rec->type) {
        case SymbolType::Integer:
            rec->data = bit_cast<int64_t>(rec->data) + 1;
            break;
        case SymbolType::Real:
            rec->data = bit_cast<uint64_t>(bit_cast<double>(rec->data) + 1);
            break;
        default:
            throw std::domain_error("Unable to increment");
        }
        break;
    }
    default:
        throw std::domain_error("Unable to increment");
    }
    pushToStack(item.type, item.value);
}

void Processor::dec()
{
    next();
    auto item = popFromStack();

    switch (item.type) {
    case SymbolType::Integer:
        item.value = bit_cast<int64_t>(item.value) - 1;
        break;
    case SymbolType::Real:
        item.value = bit_cast<uint64_t>(bit_cast<double>(item.value) - 1);
        break;
    case SymbolType::Variable: {
        ObjectRecord *rec = (ObjectRecord*)item.value;
        switch (rec->type) {
        case SymbolType::Integer:
            rec->data = bit_cast<int64_t>(rec->data) - 1;
            break;
        case SymbolType::Real:
            rec->data = bit_cast<uint64_t>(bit_cast<double>(rec->data) - 1);
            break;
        default:
            throw std::domain_error("Unable to increment");
        }
        break;
    }
    default:
        throw std::domain_error("Unable to increment");
    }
    pushToStack(item.type, item.value);
}

void Processor::pop()
{
    next();
    popFromStack();
}

void Processor::rsh()
{
    binaryStackOp(OpCode::RSH);
}

void Processor::rshz()
{
    binaryStackOp(OpCode::RSHZ);
}

void Processor::lsh()
{
    binaryStackOp(OpCode::LSH);
}

void Processor::bit_and()
{
    binaryStackOp(OpCode::BIT_AND);
}

void Processor::bit_or()
{
    binaryStackOp(OpCode::BIT_OR);
}

void Processor::bit_xor()
{
    binaryStackOp(OpCode::BIT_XOR);
}

void Processor::log_and()
{
    binaryStackOp(OpCode::LOG_AND);
}

void Processor::log_or()
{
    binaryStackOp(OpCode::LOG_OR);
}

void Processor::bit_not()
{
    next();
    auto item = popFromStack();
    if (item.type == SymbolType::Variable) {
        ObjectRecord *ptr = (ObjectRecord*)item.value;
        int64_t value = ~(int64_t)ptr->data;
        pushToStack(SymbolType::Integer, value);
    }
}

void Processor::log_not()
{
    next();
    auto item = popFromStack();
    if (item.type == SymbolType::Variable) {
        ObjectRecord *ptr = (ObjectRecord*)item.value;
        bool value = (int64_t)ptr->data ? false : true;
        pushToStack(SymbolType::Boolean, value);
    }
}

void Processor::modst()
{
    binaryStackOp(OpCode::MODST);
}

void Processor::st_ar()
{
    // смещения таблицы с перменными относительно адреса текущей команды
    uint64_t ar = *(uint64_t*)(_p + sizeof (OpCodeType));
    uint32_t endOfActivationRecord = 0xFFFFFFFF & ar;
    ar >>= 32;
    uint32_t startOfActivationRecord = ar;
    uint64_t *pStartAR = (uint64_t*)(_p - startOfActivationRecord);
    uint64_t *pEndAR = (uint64_t*)(_p - endOfActivationRecord);
    // сохранить запись активации - все указатели на переменные и параметры
    // в прямом порядке
    while (pStartAR < pEndAR) {
        _activationRecords.push(*pStartAR);
        pStartAR++;
    }
    next();
    next(sizeof (uint64_t));
}

void Processor::ld_ar()
{
    // смещения таблицы с перменными относительно адреса текущей команды
    uint64_t ar = *(uint64_t*)(_p + sizeof (OpCodeType));
    // |...32bit start of AR...|...32bit end of AR...|
    uint32_t endOfActivationRecord = 0xFFFFFFFF & ar;
    ar >>= 32;
    uint32_t startOfActivationRecord = ar;
    uint64_t *pStartAR = (uint64_t*)(_p - startOfActivationRecord);
    uint64_t *pEndAR = (uint64_t*)(_p - endOfActivationRecord);
    // сохранить запись активации - все указатели на переменные и параметры
    // в обратном порядке
    pEndAR--;
    while (pStartAR <= pEndAR) {
        auto rec = _activationRecords.top();
        _activationRecords.pop();
        *pEndAR = rec;
        pEndAR--;
    }
    next();
    next(sizeof (uint64_t));
}

void Processor::pushToStack(int64_t value)
{
    pushToStack(SymbolType::Integer, value);
}

void Processor::pushBooleanToStack(bool value)
{
    pushToStack(SymbolType::Boolean, (uint64_t)value);
}

void Processor::pushToStack(const std::u32string &value)
{
    auto prevOrNew = _strings->add(value);
    pushToStack(SymbolType::String, (uint64_t)prevOrNew);
}

void Processor::pushToStack(std::u32string &&value)
{
    auto prevOrNew = _strings->add(value);
    pushToStack(SymbolType::String, (uint64_t)prevOrNew);
}

void Processor::pushStringToStack(StringObject *strValue)
{
    auto prevOrNew = _strings->add(strValue);
    pushToStack(SymbolType::String, (uint64_t)prevOrNew);
}

void Processor::pushArrayToStack(Array *arrValue)
{
    pushToStack(SymbolType::Array, (uint64_t)arrValue);
}

void Processor::binaryStackOp(OpCode opCode)
{
    next();
    auto item2 = popFromStack();
    auto item1 = popFromStack();
    PValue value1 = PValue::getValue(item1);
    PValue value2 = PValue::getValue(item2);
    // это должно быть гарантировано, если это не так, пусть вылетит
    auto its = optypes.find(opCode);
    if (its == optypes.end())
        throw std::domain_error("Processor: Operation not found");
    ArithmeticOperation op = its->second;
    PValue result = PValue::binaryOpValues(value1, value2, op);
    if (result.type == SymbolType::String) {
        // это всегда новая строка, её нужно установить в таблицу строк
        result.strValue = _strings->add(result.strValue);
    }
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
    // значение/указатель, в зависимости от типа
    _stack.emplace(SymbolType::Variable, (uint64_t)ptr);
    // сдвиг на следующую команду
    next(sizeof (uint64_t));
}

void Processor::ldc_double_data64()
{
    next();
    _stack.emplace(SymbolType::Real, bit_cast<uint64_t>(*(double*)_p));
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
    _stack.emplace(SymbolType::Integer, *(uint64_t*)_p);
    next(sizeof (uint64_t));
}

void Processor::ldc_int64_data64()
{
    next();
    _stack.emplace(SymbolType::Integer, *(uint64_t*)_p);
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

void Processor::setValue(ObjectRecord *ptr, StringObject *value)
{
    ptr->type = SymbolType::String;
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

StackValue Processor::popFromStack()
{
    auto item = _stack.top();
    _stack.pop();
    return item;
}

void Processor::pushToStack(SymbolType type, uint64_t value)
{
    _stack.emplace(type, value);
}

void Processor::pushToStack(StackValue value)
{
    _stack.push(value);
}

void Processor::pushRealToStack(double value)
{
    pushToStack(SymbolType::Real, bit_cast<uint64_t>(value));
}

void Processor::pushObjectToStack(AutomationObject *obj)
{
    pushToStack(SymbolType::Object, bit_cast<uint64_t>(obj));
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
    StringObject *stringValue = nullptr;
    ptrLValue = readRecord(symbol);
    if (!ptrLValue) { // это lvalue, его нужно установить, если его ещё нет
        switch (item.type) {
        case SymbolType::Array:
        case SymbolType::Object:
        case SymbolType::Function:
//            ptrLValue = _storage->findRecord(item.value);
//            assert(ptrLValue);
//            break;
        case SymbolType::String:
            ptrLValue = _storage->findRecord(item.value);
            if (!ptrLValue)
                ptrLValue = installRecord(symbol);
            assert(ptrLValue);
            break;
        default:
            ptrLValue = installRecord(symbol);
            break;
        }
        // имеем адрес ObjectRecord
        // этот адрес нужно записать в секции DATA:
        uint8_t *addr = _memory + symbol->location();
        uint64_t ptrAsAddr = (uint64_t)ptrLValue;
        memcpy(addr, &ptrAsAddr, sizeof(uint64_t));
    }
    switch (item.type) {
    case SymbolType::Integer:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, (int64_t)item.value);
        break;
    case SymbolType::Real:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, bit_cast<double>(item.value));
        break;
    case SymbolType::Boolean:
        // в операнде находится указатель на запись в таблице символов
        setValue(ptrLValue, item.value ? true : false);
        break;
    case SymbolType::String: // проверено выше
        ptrLValue->type = SymbolType::String;
        ptrLValue->data = item.value;
        break;
    case SymbolType::Array: // проверено выше
        //ptrLValue->type = SymbolType::Array;
        //ptrLValue->data = item.value;
        //ptrLValue->reference = true; // TODO: нужно проверить все присваивания
        break;
    case SymbolType::Object:
        ptrLValue->type = SymbolType::Object;
        ptrLValue->data = item.value;
        ptrLValue->managed = ((AutomationObject*)item.value)->managed();
        break;
    case SymbolType::Variable:
        // здесь находится указатель на запись в таблице объектов
        // в зависимости от типа, мы либо меняем ссылку, либо присваиваем по значению
        // пока целые - меняем по значению
        ptrRValue = (ObjectRecord*)item.value;
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
        case SymbolType::String:
            stringValue = (StringObject*)ptrRValue->data;
            setValue(ptrLValue, stringValue);
            *(uint64_t*)(_memory + symbol->location()) = (uint64_t)ptrRValue;
            break;
        case SymbolType::Array:
            // здесь нужно заменить само значение указателя в секции DATA
            // а не просто сменить значение, теперь переменная указывает на
            // нужный объект, а тот оказывается в мусоре
            *(uint64_t*)(_memory + symbol->location()) = (uint64_t)ptrRValue;
            break;
        case SymbolType::Function:
            *(uint64_t*)(_memory + symbol->location()) = (uint64_t)ptrRValue;
            break;
        case SymbolType::Object:
            *(uint64_t*)(_memory + symbol->location()) = (uint64_t)ptrRValue;
            break;
        default:
            throw std::domain_error("Unsupported type of variable :" + std::to_string((uint8_t)item.type));
        }
        break;
    default:
        throw std::domain_error("Unsupported type :" + std::to_string((uint8_t)item.type));
    }
    next(sizeof (uint64_t));//размер операнда-указателя
}






} // namespace escript
