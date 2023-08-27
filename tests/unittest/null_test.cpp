#include "stdafx.h"
#include "null_test.h"


void Null_Test::run()
{
    initTestCase();
    test_compareWithNull();
    test_assignToNull();
    test_arrayFillEmptyItemsWithNull();
    cleanupTestCase();
}

void Null_Test::initTestCase()
{

}

void Null_Test::cleanupTestCase()
{

}

void Null_Test::test_compareWithNull()
{
    const u32string code1 = UR"(
var a;
var x = 10;
b = a == null;
c = x != null;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Null_Test::test_assignToNull()
{
    const u32string code1 = UR"(
var x = 10;
c = x != null;
x = null;
b = x == null;
y = x;
d = y == null;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto d = mainTable->find(U"d");
    record = engine.getObjectRecord(d);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));
}

void Null_Test::test_arrayFillEmptyItemsWithNull()
{
    const u32string code1 = UR"(
var x = [];
x[5] = 23;
b = x[2] == null;
console.log(x); // -> [null, null, null, null, null, 23];
)";
    EScript engine;
    stringstream ss;
    engine.setOutStream(ss);
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    std::string str = ss.str();
    assert(str == "[null, null, null, null, null, 23]" + eol);
    ss.str("");
}
