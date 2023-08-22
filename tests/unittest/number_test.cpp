#include "stdafx.h"
#include "number_test.h"


void Number_Test::run()
{
    initTestCase();
    test_parseInt();
    test_parseFloat();
    test_toString10();
    test_toString2();
    test_toString8();
    test_toString16();
    test_realToString();
    test_realParenthExprToString();
    test_intParenthExprToString();
    cleanupTestCase();
}

void Number_Test::initTestCase()
{

}

void Number_Test::cleanupTestCase()
{

}

void Number_Test::test_parseInt()
{
    const u32string code1 =
UR"(
str = "12345";
x = Number.parseInt(str);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12345, record->data));
}

void Number_Test::test_parseFloat()
{
    const u32string code1 =
UR"(
str = "3.14159";
x = Number.parseFloat(str);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Real);
    assert(Compare::equals_double(3.14159, record->data));
}

void Number_Test::test_toString10()
{
    const u32string code1 =
UR"(
x = 32;
s = x.toString();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"32" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_toString2()
{
    const u32string code1 =
UR"(
x = 387;
s = x.toString(2);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"110000011" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_toString8()
{
    const u32string code1 =
UR"(
x = 387;
s = x.toString(8);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"603" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_toString16()
{
    const u32string code1 =
UR"(
x = 387;
s = x.toString(16);
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"183" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_realToString()
{
    const u32string code1 =
UR"(
x = 387.561;
s = x.toString();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"387.561" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_realParenthExprToString()
{
    const u32string code1 =
UR"(
s = (387.561).toString();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"387.561" == ((StringObject*)record->data)->uString());
}

void Number_Test::test_intParenthExprToString()
{
    const u32string code1 =
UR"(
s = (165).toString();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    assert(U"165" == ((StringObject*)record->data)->uString());
}
