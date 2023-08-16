#include "stdafx.h"
#include "console_test.h"

std::string eol = "\n";

void Console_Test::run()
{
    initTestCase();
    test_consoleLogSimpleTypes();
    test_consoleLogVariables();
    cleanupTestCase();
}

void Console_Test::initTestCase()
{

}

void Console_Test::cleanupTestCase()
{

}

void Console_Test::test_consoleLogSimpleTypes()
{
    stringstream ss;
    const u32string code1 = U"i = 0; console.log(123);";
    EScript engine;
    //engine.setShowDisassembleListing(true);
    //engine.setShowTCode(true);
    engine.setOutStream(ss);
    engine.eval(code1);
    string s1 = "123" + eol;
    assert(s1 == ss.str());
    ss.clear();
    ss.str("");

    const u32string code2 = U"console.log(45.67);";
    engine.eval(code2);
    assert("45.67" + eol == ss.str());
    ss.clear();
    ss.str("");

    const u32string code3 = U"console.log('hello');";
    engine.eval(code3);
    assert("hello" + eol == ss.str());
    ss.clear();
    ss.str("");

    const u32string code4 = U"console.log(true);";
    engine.eval(code4);
    string  sa = ss.str();
    assert("true" + eol == ss.str());
    ss.clear();
    ss.str("");
}

void Console_Test::test_consoleLogVariables()
{
    stringstream ss;
    const u32string code1 = U"i = 123; console.log(i);";
    EScript engine;
    //engine.setShowDisassembleListing(true);
    //engine.setShowTCode(true);
    engine.setOutStream(ss);
    engine.eval(code1);
    string s1 = "123" + eol;
    assert(s1 == ss.str());
    ss.clear();
    ss.str("");

    const u32string code2 = U"i = 45.67; console.log(i);";
    engine.eval(code2);
    assert("45.67" + eol == ss.str());
    ss.clear();
    ss.str("");

    const u32string code3 = U"i = 'hello'; console.log(i);";
    engine.eval(code3);
    assert("hello" + eol == ss.str());
    ss.clear();
    ss.str("");

    const u32string code4 = U"i = true; console.log(i);";
    engine.eval(code4);
    string  sa = ss.str();
    assert("true" + eol == ss.str());
    ss.clear();
    ss.str("");
}
