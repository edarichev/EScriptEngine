#include "stdafx.h"
#include "stringobject.h"
#include "processor.h"

namespace escript {

StringObject::StringObject() { }

StringObject::StringObject(const std::u32string &s) : _s(s) {}

bool StringObject::call(const std::u32string &method, Processor *p)
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
    if (method == U"at") {
        // метод с аргументами
        // вытащить из стека аргументы
        // число аргументов, пока не нужно, просто пропустить
        p->popFromStack();
        int64_t param1 = p->popFromStack().getIntValue();
        int64_t c = at(param1);
        p->pushToStack(c); // результат или 0
        return true;
    }
    if (method == U"substring") {
        // метод с аргументами
        // вытащить из стека аргументы
        // число аргументов, пока не нужно, просто пропустить
        auto countOfArgs = p->popFromStack().getIntValue();
        int64_t param2 = _s.length(), param1 = 0;
        // аргументы вытаскиваются в обратном порядке
        // сначала последний
        if (countOfArgs == 1) {
            param1 = p->popFromStack().getIntValue();
        } else if (countOfArgs == 2) {
            param2 = p->popFromStack().getIntValue();
            param1 = p->popFromStack().getIntValue();
        }
        int64_t fromIndex = param1, toIndex = param2;
        StringObject *newString = substring(fromIndex, toIndex);
        p->pushToStack(SymbolType::String, (uint64_t)newString);
        return true;
    }
    throw std::domain_error("Call of unknown method: string." + to_utf8(method));
}

int64_t StringObject::at(int64_t i) const
{
    return _s.at(i);
}

int64_t StringObject::length() const
{
    return _s.length();
}

StringObject::StringObject(const char32_t *str)
{
    _s = str;
}

StringObject &StringObject::operator=(const std::u32string &str)
{
    _s = str;
    return *this;
}

bool StringObject::operator==(const std::u32string &s) const
{
    return s == _s;
}

bool StringObject::operator==(const char32_t *s) const
{
    return s == _s;
}

const std::u32string &StringObject::uString() const
{
    return _s;
}

StringObject *StringObject::concat(StringObject *s1, StringObject *s2)
{
    StringObject *newString = new StringObject(s1->_s + s2->_s);
    return newString;
}

StringObject *StringObject::substring(int64_t fromIndex, int64_t toIndex) const
{
    if (fromIndex < 0)
        fromIndex = 0;
    if (toIndex > (int64_t) _s.length() || toIndex < fromIndex)
        toIndex = _s.length();
    return new StringObject(_s.substr(fromIndex, toIndex - fromIndex));
}




} // namespace escript
