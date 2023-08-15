#include "stdafx.h"
#include "array.h"
#include "pvalue.h"
#include "processor.h"

namespace escript {

Array::Array()
{

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
    _items[to_utf8(index)] = value;
}

void Array::set(const std::u32string &index, PValue value)
{
    _items[to_utf8(index)] = value;
}

int64_t Array::length() const { return _items.size(); }

void Array::add(PValue value)
{
    set(_items.size(), value);
}

} // namespace escript


bool escript::Array::call(const std::u32string &method, Processor *p)
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
    if (method == U"add") {
        p->popFromStack(); // число аргументов
        auto arg = p->popFromStack();
        // пусть только переменные типа Integer
        ObjectRecord *rec = (ObjectRecord*)(arg.value);
        add(PValue((int64_t)rec->data));
        p->pushToStack(0); // OK
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
    throw std::domain_error("Call of unknown method: array." + to_utf8(method));
}
