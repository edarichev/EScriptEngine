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
    StringObject str = U"Hello";
    escript::EScript engine;
    auto p = engine.machine().cpu();
    str.call(U"at", &p);
}


