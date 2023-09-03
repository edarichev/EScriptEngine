#include "stdafx.h"
#include "newexpression_test.h"



void NewExpression_Test::run()
{
    initTestCase();
    test_newArray();
    test_newArrayArgs();
    test_newDateTime();
    test_newDateTimeExpr();
    cleanupTestCase();
}

void NewExpression_Test::initTestCase()
{

}

void NewExpression_Test::cleanupTestCase()
{

}

void NewExpression_Test::test_newArray()
{
    const u32string code1 = UR"(
a = new Array();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    auto record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
}

void NewExpression_Test::test_newArrayArgs()
{
    const u32string code1 = UR"(
a = new Array(12, 34, 56, "hello");
x = a.length;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    assert(a != nullptr);
    auto record = engine.getObjectRecord(a);
    assert(record != nullptr);
    assert(record->type == SymbolType::Array);
    Array *arr = (Array*)record->data;
    assert(arr->get(0).intValue == 12);
    assert(arr->get(1).intValue == 34);
    assert(arr->get(2).intValue == 56);
    assert(arr->get(3).strValue->refString() == U"hello");
    record = engine.getObjectRecord(mainTable->find(U"x"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));
}

void NewExpression_Test::test_newDateTime()
{
    const u32string code1 = UR"(
x = new DateTime(1995, 12, 4, 1, 12, 45, 123);
year = x.year();
mon = x.month();
day = x.day();
hour = x.hour();
minute = x.minute();
sec = x.second();
ms = x.ms();
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto record = engine.getObjectRecord(mainTable->find(U"year"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1995, record->data));

    record = engine.getObjectRecord(mainTable->find(U"mon"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));

    record = engine.getObjectRecord(mainTable->find(U"day"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(4, record->data));

    record = engine.getObjectRecord(mainTable->find(U"hour"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, record->data));

    record = engine.getObjectRecord(mainTable->find(U"minute"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(12, record->data));

    record = engine.getObjectRecord(mainTable->find(U"sec"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(45, record->data));

    record = engine.getObjectRecord(mainTable->find(U"ms"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(123, record->data));
}

void NewExpression_Test::test_newDateTimeExpr()
{
    const u32string code1 = UR"(
year = (new DateTime(1995, 12, 4, 1, 12, 45, 123)).year() + 12;
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();

    auto record = engine.getObjectRecord(mainTable->find(U"year"));
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(1995 + 12, record->data));
}
