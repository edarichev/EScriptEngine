#include "stdafx.h"
#include "array.h"
#include "pvalue.h"
#include "processor.h"

namespace escript {

std::map<std::u32string, Array::pFn> Array::_fn;

Array::Array()
{
    _managed = true;
    buildFunctionsMap();
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
    setKeyValue(key, value);
}

int64_t Array::length() const { return _items.size(); }

void Array::add(PValue value)
{
    set(_items.size(), value);
}

std::u32string Array::uString() const
{
    // вывести все элементы
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

void Array::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"get_length"] = &Array::call_get_length;
    _fn[U"get"] = &Array::call_get;
    _fn[U"set"] = &Array::call_set;
    _fn[U"push"] = &Array::call_push;
    _fn[U"add"] = &Array::call_push;
}

void Array::call_get_length(Processor *p)
{
    // вытащить из стека аргументы
    // число аргументов, здесь д.б. 0, просто пропустить
    auto args = loadArguments(p);
    int64_t p1 = length();
    p->pushToStack(p1); // результат
}

void Array::call_push(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack((int64_t)0);
        return;
    }
    auto arg = args.top();
    // пусть только переменные типа Integer
    ObjectRecord *rec = (ObjectRecord*)(arg.value);
    add(PValue(rec->type, rec->data));
    p->pushToStack((int64_t)0); // OK
}

void Array::call_get(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty()) {
        p->pushToStack((int64_t)0); // OK
        return;
    }
    auto index = args.top().getStringValue();
    PValue v = this->get(index);
    p->pushToStack(v.type, v.value64()); // OK
}

void Array::call_set(Processor *p)
{
    auto args = loadArguments(p);
    auto argIndex = args.top(); // индекс/ключ
    args.pop();
    auto argValue = args.top(); // значение
    // индекс может не существовать, как ключ он будет добавлен
    // если пришла переменная, вынуть из неё значение и поместить его
    // так чтобы тип Variable В массиве не хранился
    // но это могут быть массивы, объекты, функции и т.п.
    PValue v = PValue::getValue(argValue);
    set(argIndex.getStringValue(), v);
    p->pushToStack((int64_t)0); // OK
}

bool Array::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyClass." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}

} // namespace escript
