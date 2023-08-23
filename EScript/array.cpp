#include "stdafx.h"
#include "array.h"
#include "pvalue.h"
#include "processor.h"

namespace escript {

Array::Array()
{
    _managed = true;
}

Array::~Array()
{

}

PValue Array::get(int64_t index)
{
    return _items[to_utf8(index)];
}

PValue Array::get(const std::u32string &index)
{
    return _items[to_utf8(index)];
}

void Array::set(int64_t index, PValue value)
{
    auto key = to_utf8(index);
    setKeyValue(key, value);
}

void Array::set(int64_t index, SymbolType t, uint64_t value)
{
    set(index, PValue(t, value));
}

void Array::set(const std::u32string &index, PValue value)
{
    auto key = to_utf8(index);
    // TODO: извлечь значение, если это переменная простого типа
    setKeyValue(key, value);
}

int64_t Array::length() const { return _items.size(); }

void Array::add(PValue value)
{
    set(_items.size(), value);
}

std::u32string Array::uString() const
{
    // TODO: вывести все элементы
    std::u32string s(U"[");
    for (size_t i = 0; i < _keys.size(); i++) {
        auto key = _keys[i];
        auto value = _items.find(key)->second;
        s += value.uString();
        if (i < _keys.size() - 1)
            s.append(U", ");
    }
    s += U"]";
    return s;
}

void Array::setKeyValue(const std::string &key, const PValue &value)
{
    if (_items.find(key) == _items.end())
        _keys.push_back(key);
    _items[key] = value;
}

bool Array::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"get_length") {
        // вытащить из стека аргументы
        // число аргументов, здесь д.б. 0, просто пропустить
        p->popFromStack();
        int64_t p1 = length();
        p->pushToStack(p1); // результат или 0
        return true;
    }
    if (method == U"add" || method == U"push") {
        p->popFromStack(); // число аргументов
        auto arg = p->popFromStack();
        // пусть только переменные типа Integer
        ObjectRecord *rec = (ObjectRecord*)(arg.value);
        add(PValue(rec->type, rec->data));
        p->pushToStack((int64_t)0); // OK
        return true;
    }
    if (method == U"get") {
        auto argCount = p->popFromStack().value; // число аргументов == 1
        assert(argCount == 1);
        auto arg = p->popFromStack();
        // пусть только переменные типа Integer и тип Integer
        int index = -1;
        if (arg.type == SymbolType::Integer)
            index = arg.value;
        else if (arg.type == SymbolType::Variable) {
            ObjectRecord *rec = (ObjectRecord*)(arg.value);
            index = rec->data;
        }
        PValue v = this->get(index);
        p->pushToStack(v.type, v.value64()); // OK
        return true;
    }
    if (method == U"set") {
        auto argCount = p->popFromStack().value; // число аргументов == 2
        assert(argCount == 2);
        auto argValue = p->popFromStack(); // сначала значение
        auto argIndex = p->popFromStack(); // теперь индекс/ключ
        // пусть только переменные типа Integer и тип Integer
        int index = -1;
        if (argIndex.type == SymbolType::Integer)
            index = argIndex.value;
        else if (argIndex.type == SymbolType::Variable) {
            ObjectRecord *rec = (ObjectRecord*)(argIndex.value);
            index = rec->data;
        }
        // индекс может не существовать, как ключ он будет добавлен
        if (argValue.type == SymbolType::Variable) {
            ObjectRecord *rec = (ObjectRecord*)argValue.value;
            switch (rec->type) {
            case SymbolType::Boolean:
            case SymbolType::Integer:
            case SymbolType::Real:
            case SymbolType::String:
                argValue.type = rec->type;
                argValue.value = rec->data;
                break;
            default:
                throw std::domain_error("Not supported type of array item");
            }
        }
        set(index, argValue.type, argValue.value);
        p->pushToStack((int64_t)0); // OK
        return true;
    }
    throw std::domain_error("Call of unknown method: array." + to_utf8(method));
}

} // namespace escript
