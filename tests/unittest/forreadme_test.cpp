#include "stdafx.h"
#include "forreadme_test.h"


void ForReadme_Test::run()
{
    initTestCase();
    test_consoleLog();
    test_functionDecl();
    test_functionRecursionExample();
    test_arrLengthGet();
    test_forTmp();
    cleanupTestCase();
}

void ForReadme_Test::initTestCase()
{

}

void ForReadme_Test::cleanupTestCase()
{

}

void ForReadme_Test::test_functionDecl()
{
    const std::string macro1 = R"(
function doSomething(msg, x, y) {
    result = msg + (x + y);
    return result;
}

x = doSomething("Result=", 123, 456);
console.log(x);
)";
    const std::u32string code1 = to_u32string(macro1);
    EScript engine;
    std::stringstream ss;
    engine.setOutStream(ss);
    engine.eval(code1);
}

void ForReadme_Test::test_functionRecursionExample()
{
    const u32string code1 =
U"function factorial(i) { "
"    if (i == 0)"
"        return 1;"
    "return i * factorial(i - 1);"
"}"
"y = factorial(10);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3628800, record->data));
}

void ForReadme_Test::test_arrLengthGet()
{
    const u32string code1 =
// U"a = [1,2,3]; x = a.length;";
U"a = [1,2,3]; x = a.get_length();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3, record->data));
}

void ForReadme_Test::test_forTmp()
{
    const std::u32string code1 = UR"(
s = 'Hello';
s[1] = 'a'; // Hello -> Hallo
//console.log(s);
)";
    EScript engine;
    engine.eval(code1);
}

void ForReadme_Test::test_consoleLog()
{
    const std::string macro1 = R"(
        z = 123.45;
        console.log("z=", z);
        )";
    const std::u32string code1 = to_u32string(macro1);
    EScript engine;
    std::stringstream ss;
    engine.setOutStream(ss);
    engine.eval(code1);
    auto s1 = ss.str();
    assert("z= 123.45" + eol == s1);
    ss.clear();
    ss.str(""); // сброс
}

