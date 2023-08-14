#include "stdafx.h"
#include "stringobject.h"

namespace escript {

StringObject::StringObject() { }

StringObject::StringObject(const std::u32string &s) : _s(s) {}

bool StringObject::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    //CALL_AUTO_METHOD1(int64_t, at, int64_t);
    if (method == U"get_length") {
        // вытащить из стека аргументы
        // число аргументов, здесь д.б. 0, просто пропустить
        p->popFromStack();
        int64_t p1 = get_length();
        p->pushValue(p1); // результат или 0
        return true;
    }
    if (method == U"at") {
        // метод с аргументами
        // вытащить из стека аргументы
        // число аргументов, пока не нужно, просто пропустить
        p->popFromStack();
        int64_t param1 = p->popFromStack().value;
        int64_t c = at(param1);
        p->pushValue(c); // результат или 0
        return true;
    }
    throw std::domain_error("Call of unknown method: " + to_utf8(method));
}

int64_t StringObject::at(int64_t i)
{
    return _s.at(i);
}

int64_t StringObject::get_length()
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

bool StringObject::operator==(const std::u32string &s)
{
    return s == _s;
}

bool StringObject::operator==(const char32_t *s)
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




} // namespace escript
