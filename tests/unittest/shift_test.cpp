#include "stdafx.h"
#include "shift_test.h"



void Shift_Test::run()
{
    initTestCase();
    test_lshift();
    test_rshift();
    test_rshiftz();
    cleanupTestCase();
}

void Shift_Test::initTestCase()
{

}

void Shift_Test::cleanupTestCase()
{

}

void Shift_Test::test_lshift()
{
    const u32string code1 = U"a = 3; b = a << 3;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3 << 3, record->data));

    const u32string code2 = U"a = 3; c = 2; b = a << c;";
    engine.eval(code2);
    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3 << 2, record->data));
}

void Shift_Test::test_rshift()
{
    const u32string code1 = U"a = 300; b = a >> 3;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(300 >> 3, record->data));

    const u32string code2 = U"a = 345; c = 2; b = a >> c;";
    engine.eval(code2);
    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(345 >> 2, record->data));

    const u32string code3 = U"a = -1; c = 6; b = a >> c;";
    engine.eval(code3);
    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1 >> 6, record->data));
}


void Shift_Test::test_rshiftz()
{
    const u32string code1 = U"a = 300; b = a >>> 3;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto b = mainTable->find(U"b");
    auto record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64((uint64_t)300 >> 3, record->data));

    const u32string code2 = U"a = 345; c = 2; b = a >>> c;";
    engine.eval(code2);
    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64((uint64_t)345 >> 2, record->data));

    const u32string code3 = U"a = -1; c = 6; b = a >>> c;";
    engine.eval(code3);
    b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64((uint64_t)-1 >> 6, record->data));
}

