#ifndef ESCRIPT_AUTOMATIONOBJECT_H
#define ESCRIPT_AUTOMATIONOBJECT_H

#include "EScript_global.h"
#include "escript.h"

namespace escript {

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 */
class AutomationObject
{
protected:
    EScript *_engine = nullptr;
public:
    AutomationObject(){};
    virtual ~AutomationObject(){}
    virtual bool call([[maybe_unused]] const std::string &method)
    {
        return false;
    }
    void setEngine(escript::EScript *newEngine)
    {
        _engine = newEngine;
    }
};

#if 0
// пока заглушка
#define CALL_AUTO_METHOD1(returnType, name, param1type)

#else
// TODO: Открыть позже, когда будет разработка объектов. Это работает:
#define CALL_AUTO_METHOD1(returnType, name, param1type) \
    if (method == #name) { \
        param1type param1 = 0; /* из стека извлечь */ \
        returnType result = name(param1); \
        std::cout << result << std::endl; /*поместить результат в стек*/\
        return true; \
    } \

#endif

class StringObject : public AutomationObject
{
    std::u32string _s;
    using BaseClass = AutomationObject;
public:
    StringObject() { }
    StringObject(const std::u32string &s) : _s(s) {}

    virtual bool call(const std::string &method) override
    {
        if (BaseClass::call(method))
            return true;
        CALL_AUTO_METHOD1(int64_t, at, int64_t);
        return false;
    }

    int64_t at(int64_t i)
    {
        return _s.at(i);
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

    static StringObject *concat(StringObject *s1, StringObject *s2)
    {
        StringObject *newString = new StringObject(s1->_s + s2->_s);
        return newString;
    }
};


} // namespace escript

#endif // ESCRIPT_AUTOMATIONOBJECT_H