#ifndef ESCRIPT_STRINGOBJECT_H
#define ESCRIPT_STRINGOBJECT_H

#include "automationobject.h"

namespace escript {

class StringObject : public AutomationObject
{
    std::u32string _s;
    using BaseClass = AutomationObject;
public:
    StringObject() { }
    StringObject(const std::u32string &s) : _s(s) {}

    virtual bool call(const std::u32string &method, Processor *p) override
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
        }
        return false;
    }

    int64_t at(int64_t i)
    {
        return _s.at(i);
    }

    int64_t get_length()
    {
        return _s.length();
    }

    StringObject (const char32_t *str)
    {
        _s = str;
    }

    StringObject &operator=(const std::u32string &str)
    {
        _s = str;
        return *this;
    }

    bool operator==(const std::u32string &s)
    {
        return s == _s;
    }

    bool operator==(const char32_t *s)
    {
        return s == _s;
    }

    const std::u32string &uString() const
    {
        return _s;
    }

    static StringObject *concat(StringObject *s1, StringObject *s2)
    {
        StringObject *newString = new StringObject(s1->_s + s2->_s);
        return newString;
    }
};

} // namespace escript

#endif // ESCRIPT_STRINGOBJECT_H
