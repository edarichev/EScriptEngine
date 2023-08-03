#include "stdafx.h"
#include "automation_test.h"

#include <map>
#include "escript.h"

using namespace std;

/**
 * @brief Базовый класс для всех обёрток, в т.ч. классов в виртуальной машине.
 */
class AutomationObject
{
protected:
    escript::EScript *_engine = nullptr;
public:

    virtual bool call([[maybe_unused]] const std::string &method)
    {
        return false;
    }
    void setEngine(escript::EScript *newEngine)
    {
        _engine = newEngine;
    }
};

#define CALL_AUTO_METHOD1(returnType, name, param1type) \
    if (method == #name) { \
        param1type param1 = 0; /* из стека извлечь */ \
        returnType result = name(param1); \
        /*cout << result << endl;*/ /*поместить результат в стек*/\
        return true; \
    } \


class String : public AutomationObject
{
    std::u32string _s;
    using BaseClass = AutomationObject;
public:
    String() { }

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

    String (const std::u32string &str)
    {
        _s = str;
    }

    String (const char32_t *str)
    {
        _s = str;
    }

    String &operator=(const std::u32string &str)
    {
        _s = str;
        return *this;
    }
};

void Automation_Test::run()
{
    initTestCase();
    test_auto1();
    cleanupTestCase();
}

#include <regex>
void Automation_Test::initTestCase()
{

}

void Automation_Test::cleanupTestCase()
{

}

void Automation_Test::test_auto1()
{
    String str = U"Hello";
    escript::EScript engine;
    str.setEngine(&engine);
    str.call("at");
}


